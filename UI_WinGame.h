#pragma once
#include "UITypes.h"
#include "GameConstants.h"

class UI_WinGame
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

	UI_WinGame() { }
	~UI_WinGame() { }

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

	void SetMaxIndex(unsigned max) { if (max >= MAX_GAMEOVER_MENU_INDEXES) m_MaxIndex = MAX_GAMEOVER_MENU_INDEXES - 1; else m_MaxIndex = max; }
private:
	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//UI Elements

	//Menu Option Frames
	EM_Sprite m_MenuFrames[MAX_GAMEOVER_MENU_INDEXES];
	//Menu Option Texts
	NonNavUI::Data m_MenuText[MAX_GAMEOVER_MENU_INDEXES];
	//Selection frame
	EM_Sprite m_MenuSelection;
	//Main frame text
	NonNavUI::Data m_WindowText;

	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	//Menu indexing
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;
};

