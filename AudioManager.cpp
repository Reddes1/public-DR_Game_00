#include "AudioManager.h"
#include <assert.h>

using namespace DirectX;

AudioManager::AudioManager()
	:m_RetryAudio(false)
{
	m_WaveBanks.reserve(8);
	m_Loops.reserve(SOUND_EFFECT_INSTANCE_CHANNEL_COUNT);
}

AudioManager::~AudioManager()
{
	SuspendAudioEngine();

	m_Loops.clear();
	m_WaveBanks.clear();
}

void AudioManager::InitAudioEngine(HWND& window)
{
	// Listen for new audio devices
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;

	m_NewAudio = RegisterDeviceNotification(window, &filter,
		DEVICE_NOTIFY_WINDOW_HANDLE);


	//Init audio engine to default spec, with extra debug checking
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_AudioEngine = std::make_unique<AudioEngine>(eflags);

	//Check if the audo device is present and signal one way or another (signal use of subtitles here maybe)
	if (!m_AudioEngine->IsAudioDevicePresent())
	{
		DBOUT(">> No Audio Device found. Running program in Silent Mode.")
	}
	else
		DBOUT("Audio Device found.");
}

void AudioManager::Update()
{
	//Check for new device
	if (m_NewAudio)
	{
		UnregisterDeviceNotification(m_NewAudio);
		m_NewAudio = nullptr;
	}

	if (m_RetryAudio)
	{
		//Reset flag
		m_RetryAudio = false;
		if (m_AudioEngine->Reset())
		{
			//Restart loops here
		}
	}
	else if (!m_AudioEngine->Update())
	{
		//Check if the audio device has been lost
		if (m_AudioEngine->IsCriticalError())
		{
			//Device Lost, flag for reset
			m_RetryAudio = true;
		}
	}
}

size_t AudioManager::LoadSingleWaveBank(const wchar_t* fp)
{
	std::shared_ptr<DirectX::WaveBank> newWB = std::make_shared<DirectX::WaveBank>(m_AudioEngine.get(), fp);
	m_WaveBanks.push_back(newWB);
	return m_WaveBanks.size() - 1;
}

void AudioManager::PlayOneShotFromWaveBank(unsigned wbIndex, unsigned sfxIndex, unsigned short soundChannelID)
{
	assert(wbIndex <= m_WaveBanks.size());
	m_WaveBanks[wbIndex]->Play(sfxIndex, m_Volumes.GetAdjustedVolumeByIndex(soundChannelID), 0, 0);
}

size_t AudioManager::LoadSoundEffectInstanceFromWaveBank(unsigned wbIndex, unsigned sfxIndex, unsigned short volID)
{
	assert(wbIndex <= m_WaveBanks.size());

	//Get the sfx being instanced
	std::shared_ptr<DirectX::SoundEffectInstance> newSFX;
	newSFX = m_WaveBanks[wbIndex]->CreateInstance(sfxIndex);
	newSFX->SetVolume(m_Volumes.GetAdjustedVolumeByIndex(volID));

	//Store and return index to effect
	m_Loops.push_back(newSFX);
	return m_Loops.size() - 1;
}

std::shared_ptr<DirectX::SoundEffectInstance> AudioManager::GetSoundEffectInstanceAtIndex(unsigned index)
{
	assert(index <= m_Loops.size());
	return m_Loops[index];
}

