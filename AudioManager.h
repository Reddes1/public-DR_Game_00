#pragma once

#include "D3DUtils.h"				  //Get Types
#include "AudioDefinitions.h"		  //Wavebank defintions
#include "GameVolume.h"				  //Volume Controls


#define SOUND_EFFECT_CHANNEL_COUNT 16
#define SOUND_EFFECT_INSTANCE_CHANNEL_COUNT 16



/*
	DirectXTK based audio manager, wrapping up and managing behaviours required to get audio in game.
*/
class AudioManager
{
public:
	
	////////////
	/// Data ///
	////////////

	struct SoundInstance
	{
		std::shared_ptr<DirectX::SoundEffectInstance> m_Sound;
		unsigned short m_VolID;
	};

	////////////////////
	/// Constructors ///
	////////////////////

	AudioManager();
	~AudioManager();

	//////////////////
	/// Operations ///
	//////////////////


	void Update();

	//Initialise engine to existing window for game
	void InitAudioEngine(HWND& window);

	//Call these between tabbing in and out, resizing etc etc
	void ResumeAudioEngine() { m_AudioEngine->Resume(); }
	void SuspendAudioEngine() { m_AudioEngine->Suspend(); }
	//Called when a new device is found (if prev is lost/in silent mode)
	void OnNewAudioDevice() { m_RetryAudio = true; }

		

	//Plays a single sfx from a given wavebank
	void PlayOneShotFromWaveBank(unsigned wbIndex, unsigned sfxIndex, unsigned short soundChannelID = 0);


	///////////
	/// Get ///
	///////////

	//Get current manager
	GameVolumes& GetVolumeControls() { return m_Volumes; }

	//Gets access to a currently loaded sfx instance
	std::shared_ptr<DirectX::SoundEffectInstance> GetSoundEffectInstanceAtIndex(unsigned index);


	///////////
	/// Set ///
	///////////

	//Creates a sound effect instance from from a loaded wavebank by index. Returns access index
	//Create SFX instance from a wavebank, and set vol using ID. Return access index
	size_t LoadSoundEffectInstanceFromWaveBank(unsigned wbIndex, unsigned sfxIndex, unsigned short volID = 0);
	//Loads a single WB into vector, returns soundbank index
	size_t LoadSingleWaveBank(const wchar_t* fp);

private:


	////////////
	/// Data ///
	////////////

	//Loaded Wavebanks
	std::vector<std::shared_ptr<DirectX::WaveBank>> m_WaveBanks;
	//Looping container
	std::vector<std::shared_ptr<DirectX::SoundEffectInstance>> m_Loops;

	//Audio volume control
	GameVolumes m_Volumes;
	//Main Engine
	std::unique_ptr<DirectX::AudioEngine> m_AudioEngine;

	//Used for detecting new audio devices found during "silent mode"
	HDEVNOTIFY m_NewAudio = nullptr;
	//Check flag if device lost and need to look it again
	bool m_RetryAudio = false;
	//Mute audio flag on minimise/tab out etc
	bool m_MuteWhenUnfocused = false;

};

