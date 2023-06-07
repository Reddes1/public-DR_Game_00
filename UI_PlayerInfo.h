#pragma once

#include "EM_Sprite.h"
#include "GameTimer.h"
#include "UITypes.h"

//Forward Dec
class Entity_Player;

/*
	UI Class for displaying player health, regen, equipment etc.
*/
class UI_PlayerInfo
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class HealthFrameIDs : unsigned
	{
		HEALTH_BAR,
		HEALTH_FRAME,
		HEALTH_HEART,
		COUNT
	};

	enum class AbilityFrameIDs : unsigned
	{
		ABILITY_BAR,
		ABILITY_FRAME,
		COUNT
	};

	enum class WeaponFrameIDs : unsigned
	{
		WEAPON_00,
		WEAPON_01,
		WEAPON_02,
		WEAPON_03,
		WEAPON_04,
		WEAPON_05,
		COUNT
	};

	enum class AccessoryFrameIDs : unsigned
	{
		ACCESSORY_00,
		ACCESSORY_01,
		ACCESSORY_02,
		ACCESSORY_03,
		ACCESSORY_04,
		ACCESSORY_05,
		COUNT
	};

	enum class TextIDs : unsigned
	{
		HEALTH,
		REGEN,
		ABILITY_CD,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	UI_PlayerInfo() { }
	~UI_PlayerInfo() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Main update window
	void Update(GameTimer& gt);

	//Main render window
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);

	//Initialise the UI from file
	void InitialiseUI(std::string& fp, unsigned index);

	//Syncronises this UI with the state the players weapon (call when an update has occured, like new weapon or leveling up)
	void SyncUIWithPlayer();

	///////////
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

	//Set player pointer ONCE
	void SetPlayerPointer(Entity_Player* player);

private:

	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//Sprites equal to the required frames
	EM_Sprite m_HealthFrames[(unsigned)HealthFrameIDs::COUNT];
	EM_Sprite m_AbilityFrames[(unsigned)AbilityFrameIDs::COUNT];
	EM_Sprite m_WeaponFrames[(unsigned)WeaponFrameIDs::COUNT];
	EM_Sprite m_AccessoryFrames[(unsigned)AccessoryFrameIDs::COUNT];
	
	//Icon Sprites
	EM_Sprite m_WeaponIcons[(unsigned)WeaponFrameIDs::COUNT];
	EM_Sprite m_AccessoryIcons[(unsigned)WeaponFrameIDs::COUNT];

	//Text entries equal to health frame requirements
	NonNavUI::Data m_Texts[(unsigned)TextIDs::COUNT];
	//Text entries equal the weapon/accessory frames
	NonNavUI::Data m_WeaponTexts[(unsigned)WeaponFrameIDs::COUNT];
	NonNavUI::Data m_AccessoryTexts[(unsigned)WeaponFrameIDs::COUNT];

	//Control flags for rendering
	bool m_WeaponFlags[(unsigned)WeaponFrameIDs::COUNT] = {};
	bool m_AccessoryFlags[(unsigned)WeaponFrameIDs::COUNT] = {};

	//Hold player pointer for quicker access to stats
	Entity_Player* m_Player  = nullptr;

};