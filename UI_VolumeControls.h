#pragma once
#include "UITypes.h"
#include "GameConstants.h"
class UI_VolumeControls
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

	UI_VolumeControls() { }
	~UI_VolumeControls() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Clear current functions and disable associated flags
	void Reset();

	//Load custom UI setup from JSON file
	void InitialiseUI(std::string& fp, unsigned index);

	void Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap);

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
	
	//Background
	EM_Sprite m_Background;
	//Volume Bars
	EM_Sprite m_VolumeBars[MAX_OPTION_MENU_ENTRIES];
	//Volume bar name frames
	EM_Sprite m_VolumeNameFrames[MAX_OPTION_MENU_ENTRIES];
	//Volume bar name text
	NonNavUI::Data m_VolumeNameText[MAX_OPTION_MENU_ENTRIES];

	//Selection frame
	EM_Sprite m_VolumeBarOutline;
	//Title Frame
	EM_Sprite m_TitleFrame;
	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu indexing
	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;

};

