#pragma once
#include "UITypes.h"
#include "GameConstants.h"

/*
	Menu class for the main menu. Uses custom behaviour
*/
class UI_MainMenu
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

	UI_MainMenu() { }
	~UI_MainMenu() { }

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

	void SetMaxIndex(unsigned max) { if (max >= MAX_MAIN_MENU_INDEXES) m_MaxIndex = MAX_MAIN_MENU_INDEXES - 1; else m_MaxIndex = max; }
private:

	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//UI Elements

	//Background
	EM_Sprite m_Background[2];
	//Menu option frames
	EM_Sprite m_MenuFrames[MAX_MAIN_MENU_INDEXES];
	//Menu option texts
	NonNavUI::Data m_MenuText[MAX_MAIN_MENU_INDEXES];

	//Selection frame
	EM_Sprite m_MenuSelection;

	//Menu indexing
	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;

};

