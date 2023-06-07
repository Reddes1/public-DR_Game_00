#include "GameVolume.h"

GameVolumes::GameVolumes(float masterVol, float musicVol, float sfxVol, float ambVol, float dialogVol)
{
	SetAllVolumes(masterVol, musicVol, sfxVol, ambVol, dialogVol);
}

float GameVolumes::GetVolumeByIndex(unsigned short volID)
{
	switch (volID)
	{
	case MUSIC:
		return GetMusicVolume();
		break;
	case SFX:
		return GetSFXVolume();
		break;
	case AMBIENT:
		return GetAmbientVolume();
		break;
	case DIALOG:
		return GetDialogVolume();
		break;
	default:
		return GetMasterVolume();
		break;
	}

	//Should never hit this
	return 0.0f;
}

float GameVolumes::GetAdjustedVolumeByIndex(unsigned short volID)
{
	switch (volID)
	{
	case MUSIC:
		return GetMusicAdjustedVolume();
		break;
	case SFX:
		return GetSFXAdjustedVolume();
		break;
	case AMBIENT:
		return GetAmbientAdjustedVolume();
		break;
	case DIALOG:
		return GetDialogAdjustedVolume();
		break;
	default:
		return GetMasterVolume();
		break;
	}

	//Should never hit this
	return 0.0f;
}

void GameVolumes::SetMasterVolume(float vol)
{
	m_Volumes.m_Master += vol;
	ClampNumber(m_Volumes.m_Master);
}

void GameVolumes::SetMusicVolume(float vol)
{
	m_Volumes.m_Music = vol;
	ClampNumber(m_Volumes.m_Music);
}

void GameVolumes::SetSFXVolume(float vol)
{
	m_Volumes.m_SFX = vol;
	ClampNumber(m_Volumes.m_SFX);
}

void GameVolumes::SetAmbientVolume(float vol)
{
	m_Volumes.m_Ambient = vol;
	ClampNumber(m_Volumes.m_Ambient);
}

void GameVolumes::SetDialogVolume(float vol)
{
	m_Volumes.m_Dialog = vol;
	ClampNumber(m_Volumes.m_Dialog);
}

void GameVolumes::SetAllVolumes(float masterVol, float musicVol, float sfxVol, float ambVol, float dialogVol)
{
	m_Volumes.m_Master = masterVol;
	m_Volumes.m_Music = musicVol;
	m_Volumes.m_SFX = sfxVol;
	m_Volumes.m_Ambient = ambVol;
	m_Volumes.m_Dialog = dialogVol;
}

void GameVolumes::IncMasterVolume(float vol)
{
	m_Volumes.m_Master += vol;
	ClampNumber(m_Volumes.m_Master);
}

void GameVolumes::IncMusicVolume(float vol)
{
	m_Volumes.m_Music += vol;
	ClampNumber(m_Volumes.m_Music);
}

void GameVolumes::IncSFXVolume(float vol)
{
	m_Volumes.m_SFX += vol;
	ClampNumber(m_Volumes.m_SFX);
}

void GameVolumes::IncAmbientVolume(float vol)
{
	m_Volumes.m_Ambient += vol;
	ClampNumber(m_Volumes.m_Ambient);
}

void GameVolumes::IncDialogVolume(float vol)
{
	m_Volumes.m_Dialog += vol;
	ClampNumber(m_Volumes.m_Dialog);
}

void GameVolumes::ClampNumber(float& vol)
{
	if (vol > 1.0f)
		vol = 1.0f;
	else if (vol < 0.0f)
		vol = 0.0f;
}
