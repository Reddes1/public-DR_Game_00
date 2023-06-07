#pragma once

/*
	Wrapper class for typical volume channel and supporting control functions
*/
class GameVolumes
{
public:

	////////////
	/// Data ///
	////////////

	struct Volumes
	{
		float m_Master = 1.0f;
		float m_Music = 1.0f;
		float m_SFX = 1.0f;
		float m_Ambient = 1.0f;
		float m_Dialog = 1.0f;
	};

	enum VolumeIDs : unsigned short
	{
		MASTER,
		MUSIC,
		SFX,
		AMBIENT,
		DIALOG
	};

	////////////////////
	/// Constructors ///
	////////////////////

	GameVolumes() {}
	GameVolumes(float masterVol, float musicVol, float sfxVol, float ambVol, float dialogVol);
	~GameVolumes() {}


	//////////////////
	/// Operations ///
	//////////////////



	///////////
	/// Get ///
	///////////


	//Gets un-adjusted volumes
	
	float GetMasterVolume()				{ return m_Volumes.m_Master; }
	float GetMusicVolume()				{ return m_Volumes.m_Music; }
	float GetSFXVolume()				{ return m_Volumes.m_SFX; }
	float GetAmbientVolume()			{ return m_Volumes.m_Ambient; }
	float GetDialogVolume()				{ return m_Volumes.m_Dialog; }

	//Returns master adjusted volumes
	float GetMusicAdjustedVolume()		{ return m_Volumes.m_Master * m_Volumes.m_Music; }
	float GetSFXAdjustedVolume()		{ return m_Volumes.m_Master * m_Volumes.m_SFX; }
	float GetAmbientAdjustedVolume()	{ return m_Volumes.m_Master * m_Volumes.m_Ambient; }
	float GetDialogAdjustedVolume()		{ return m_Volumes.m_Master * m_Volumes.m_Dialog; }

	//Gets a volume by index
	float GetVolumeByIndex(unsigned short volID);
	float GetAdjustedVolumeByIndex(unsigned short volID);

	///////////
	/// Set ///
	///////////

	//Set volume to number (clamps to 0-1)
	void SetMasterVolume(float vol);
	void SetMusicVolume(float vol);
	void SetSFXVolume(float vol);
	void SetAmbientVolume(float vol);
	void SetDialogVolume(float vol);

	void SetAllVolumes(float masterVol, float musicVol, float sfxVol, float ambVol, float dialogVol);

	//Increments by value (clamps to 0-1)
	void IncMasterVolume(float vol);
	void IncMusicVolume(float vol);
	void IncSFXVolume(float vol);
	void IncAmbientVolume(float vol);
	void IncDialogVolume(float vol);


private:

	//////////////////
	/// Operations ///
	//////////////////

	void ClampNumber(float& vol);

	////////////
	/// Data ///
	////////////

	Volumes m_Volumes;

};