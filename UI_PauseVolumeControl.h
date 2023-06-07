#pragma once
#include "UITypes.h"
#include "GameConstants.h"

class UI_PauseVolumeControl
{
public:
	///////////////////
	/// Definitions ///
	///////////////////

	enum class Actions : unsigned
	{
		NO_ACTION,
		SELECTION_MADE
	};

	////////////////////
	/// Constructors ///
	////////////////////

	UI_PauseVolumeControl() { }
	~UI_PauseVolumeControl() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Clear current functions and disable associated flags
	void Reset();

	//Load custom UI setup from JSON file
	void InitialiseUI(std::string& fp, unsigned index);

	void Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap);

	void Update();
	Actions ProcessInput(char key);


	///////////
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

	void SetMaxIndex(unsigned max) { if (max >= MAX_OPTION_MENU_ENTRIES) m_MaxIndex = MAX_OPTION_MENU_ENTRIES - 1; else m_MaxIndex = max; }
private:

	//////////////////
	/// Operations ///
	//////////////////
	void DecrementVolumes();
	void IncrementVolumes();
	////////////
	/// Data ///
	////////////

	//UI Elements
	
	//Volume Bars
	EM_Sprite m_VolumeBars[MAX_OPTION_MENU_ENTRIES];
	//Volume bar name frames
	EM_Sprite m_VolumeNameFrames[MAX_OPTION_MENU_ENTRIES];
	//Volume bar name text
	NonNavUI::Data m_VolumeNameText[MAX_OPTION_MENU_ENTRIES];

	//Selection frame
	EM_Sprite m_VolumeBarOutline;

	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu indexing
	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;
};

