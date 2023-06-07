#pragma once
#include "UITypes.h"
#include "GameConstants.h"
class UI_PauseMenu
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

	UI_PauseMenu() { }
	~UI_PauseMenu() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Gets the players stats and sets the equivalent stat text number to it
	void SetStatNumbers();
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

	void SetMaxIndex(unsigned max) { if (max >= MAX_PAUSE_MENU_INDEXES) m_MaxIndex = MAX_PAUSE_MENU_INDEXES - 1; else m_MaxIndex = max; }
private:
	//////////////////
	/// Operations ///
	//////////////////
	std::string StringStreamStats(float stat);
	float RoundedStat(float stat);
	////////////
	/// Data ///
	////////////

	//UI Elements

	//Menu option frames
	EM_Sprite m_MenuFrames[MAX_PAUSE_MENU_INDEXES];
	//Menu option texts
	NonNavUI::Data m_MenuText[MAX_PAUSE_MENU_INDEXES];
	//Stat Texts Name
	NonNavUI::Data m_StatTexts[MAX_STAT_TEXTS];
	NonNavUI::Data m_StatNumber[MAX_STAT_TEXTS];
	//Selection frame
	EM_Sprite m_MenuSelection;
	//Stats frame
	EM_Sprite m_StatsFrame;
	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu indexing

	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;

};

