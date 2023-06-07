#pragma once

#include "D3DUtils.h"	//DX Types
#include "GameTimer.h"	//Game Time

#include "GameTypes.h"  //Stats & Data

//Foward Dec
class Entity_Player;


class Player_Accessory
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	Player_Accessory() { Init(); }
	~Player_Accessory() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	void Update(GameTimer& gt);
	void InitialiseAccessory(AccessoryTypeID accessoryID, AccessoryCompositeData& statPack);
	bool LevelUpAccessory();

	//Checks to see if accessory is maxed or not
	bool IsLevelMaxed() { return m_LevelSys.m_WeaponLevel >= m_LevelSys.m_MaxLevel; }

	//Disables the accessory by setting type to undefined
	void DisableAccessory() { m_AccessoryID = AccessoryTypeID::UNDEFINED; }

	///////////
	/// Get ///
	///////////

	AccessoryTypeID GetAccessoryID() { return m_AccessoryID; }
	GeneralEntityData& GetGeneralData() { return m_GenData; }
	LevelingData& GetLevelingSystem() { return m_LevelSys; }
	IconData& GetIconData() { return m_IconData; }

	///////////
	/// Set ///
	///////////

	//Must be done ONCE to link this accessory to the player (so that level up effects can be applied to the players stats)
	void SetPlayerPointer(Entity_Player* playerPtr);
	void SetData(GeneralEntityData& data) { m_GenData = data; }

private:

	//////////////////
	/// Operations ///
	//////////////////

	//
	// General Functions
	//

	//Basic Init function
	void Init();
	//Basic Release function
	void Release();

	////////////
	/// Data ///
	////////////

	//General data
	GeneralEntityData m_GenData;
	//Leveling up data
	LevelingData m_LevelSys;
	//Icon Data to pass to UIs
	IconData m_IconData;
	//Binding pointer to the player
	Entity_Player* m_Player = nullptr;
	//Accessory typing - defines the behaviour of the class
	AccessoryTypeID m_AccessoryID = AccessoryTypeID::UNDEFINED;
};

