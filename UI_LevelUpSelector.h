#pragma once

#include <functional>	//Function Pointer

#include "UITypes.h"
#include "GameConstants.h"
#include "Player_Weapon.h"
#include "Player_Accessory.h"

/*
	Menu class for level selection. Uses custom behaviour (seperate from UI class)
*/
class UI_LevelUpSelector
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	typedef std::function<bool()> LevelFunc;
	typedef std::function<bool(unsigned)> NewEquipFunc;
	
	enum class Actions : unsigned
	{
		NO_ACTION,
		SELECTION_MADE
	};

	enum class FrameIDs
	{
		SELECTION_FRAME,
		HIGHLIGHT_FRAME,
		COUNT
	};

	enum class TextIDs
	{
		NAME,
		LEVEL,
		DESCRIPTION,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	UI_LevelUpSelector() {  }
	~UI_LevelUpSelector() {  }

	//////////////////
	/// Operations ///
	//////////////////

	//Clears current functions, disabling associated flags
	void Reset();

	//Loads custom UI setup from file
	void InitialiseUI(std::string& fp, unsigned index);

	void Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap);

	Actions ProcessInput(char key);

	//Initialise an option at index as owned weapon
	void InitialiseOptionAsOwnedWeapon(unsigned index, Player_Weapon* weapon);
	//Initialises an option at index as unowned weapon
	void InitialiseOptionAsUnownedWeapon(unsigned index, unsigned weaponID);
	//Initialise an option at index as owned accessory
	void InitialiseOptionAsOwnedAccessory(unsigned index, Player_Accessory* accessory);
	//Initialise an option at index as unowned accessory
	void InitialiseOptionAsUnownedAccessory(unsigned index, unsigned accessoryID);


	///////////
	/// Get ///
	///////////



	///////////
	/// Set ///
	///////////

	void SetMaxIndex(unsigned max) { if (max > 4) m_MaxIndex = 3; else m_MaxIndex = max; }


private:

	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//UI Elements
	EM_Sprite m_OptionFrames[MAX_LEVEL_OPTIONS];
	EM_Sprite m_OptionIcons[MAX_LEVEL_OPTIONS];
	NonNavUI::Data m_OptionName[MAX_LEVEL_OPTIONS];
	NonNavUI::Data m_OptionLevel[MAX_LEVEL_OPTIONS];
	NonNavUI::Data m_OptionDescription[MAX_LEVEL_OPTIONS];
	
	//UI Functionality
	LevelFunc m_LevelFunctions[MAX_LEVEL_OPTIONS];
	NewEquipFunc m_NewEquipFunctions[MAX_LEVEL_OPTIONS];
	bool m_OptionEnabled[MAX_LEVEL_OPTIONS] = {false, false, false, false};

	//Selection Frame
	EM_Sprite m_SelectionFrame;
	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu Indexing
	DirectX::XMFLOAT2 m_Offsets = { 0.f, 0.f };
	unsigned m_CurrentIndex = 0;
	unsigned m_MaxIndex = 0;
};