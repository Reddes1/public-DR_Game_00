#pragma once
#include "UITypes.h"
#include "GameConstants.h"

class UI_MapSelection
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

	UI_MapSelection() { }
	~UI_MapSelection() { }

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

	void SetMaxIndex(unsigned max) { if (max >= MAX_MAP_INDEXES) m_MaxIndex = MAX_MAP_INDEXES - 1; else m_MaxIndex = max; }
private:

	//////////////////
	/// Operations ///
	//////////////////
	void SelectMap(unsigned index);
	////////////
	/// Data ///
	////////////

	//UI Elements

	//Background
	EM_Sprite m_Background;
	//Map option frames
	EM_Sprite m_MapOptionFrame[MAX_MAP_INDEXES];
	//Selection frame
	EM_Sprite m_MapSelection;
	//Map icons
	EM_Sprite m_MapIcons[MAX_MAP_INDEXES];
	//Title Frame
	EM_Sprite m_TitleFrame;
	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu indexing
	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	unsigned  m_CurrentIndex = 0;
	unsigned  m_MaxIndex = 0;
};

