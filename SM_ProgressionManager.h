#pragma once

#include "EM_Sprite.h"	
#include "UI_LevelUpSelector.h"

//Foward Dec
class Entity_Player;

/*
	Progress tracker and manager for player leveling, and generating level up options for the player.
*/
class SM_ProgressionManager
{
public:

	/////////////
	/// Types ///
	/////////////

	struct SelectionData
	{
		unsigned m_EquipmentType = 0;  //Is this a weapon or accessory
		unsigned m_EquipmentID = 0;	   //What type of equipment (dagger, armor etc)
		bool m_PlayerHas = false;	   //Flag if the player owns this
		bool m_Maxed = false;		   //Flag if weapon is maxed
	};

	////////////////////
	/// Constructors ///
	////////////////////

	SM_ProgressionManager() {}
	~SM_ProgressionManager() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Update internal functionality 
	void Update();

	//Adds an experience amount to current, flagging for a levelup if condition met
	void AddExp(unsigned exp);

	//Debug operation to force level
	void ForceLevelUp();

	//Player level increase operation via loot chest
	bool ApplyChestUpgrade();

	void IncrementKillCount() { ++m_KillCount; }
	void ResetKillCount() { m_KillCount = 0; }

	//
	//Inits
	//

	//Initialise UI_LevelSelector pointer (Call ONCE)
	void BindLevelSelectorUI(UI_LevelUpSelector* ptr);
	//Resets progression stats (level, xp etc)
	void ResetProgress();


	///////////
	/// Get ///
	///////////

	unsigned GetCurrentLevel() { return m_CurrentLevel; }
	unsigned GetExperienceForNextLevel() { return m_ExperienceRequired; }
	unsigned GetExperienceForLevel(unsigned level);
	unsigned GetCurrentExperience() { return m_CurrentExperience; }
	bool GetLevelPendingFlag() { return m_LevelPending; }

	int GetKillCount() { return m_KillCount; }

	//Sets ptr to player (call ONCE)
	void SetPlayerPointer(Entity_Player* playerPtr) { m_Player = playerPtr; }

private:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();

	//Runs leveling up process for player (managing XP overflow, reflagging if required, and calling level selection process)
	void LevelUp();

	//Runs a selection process for the player, determining what possible items/upgrades they qualify for, then randomly selecting them.
	bool RunSelectionProcess();

	//Runs a selection process for the player, determining what possible items/upgrades they qualify for, then randomly selecting them.
	void FindAvailableUpgrades(std::vector<SelectionData>& dataContainer);
	//
	//Selection Process
	//

	//
	//Filtering Methods
	//

	//Filters out maxed equipment
	void FilterMaxedEquipment(std::vector<SelectionData>& arr);
	//Filters out unowned weapons
	void FilterUnownedWeapons(std::vector<SelectionData>& arr);
	//Filters out unowned accessories
	void FilterUnownedAccessories(std::vector<SelectionData>& arr);

	//
	//Selection Methods
	//

	//Selects options completely at random
	bool SelectionRandom(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections);
	//Shuffles and selects the viable weapon (if applicable)
	bool SelectionWeaponBias(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections);
	//Shuffles and selects the first viable accessory (if applicable)
	bool SelectionAccessoryBias(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections);

	//
	//Actioning Methods
	//

	//Builds Level Selector UI
	void BuildLevelSelector(std::vector<SelectionData>& options);

	//Automatically applies the first option to the player, displaying the results
	//NYI

	////////////
	/// Data ///
	////////////

	//Quick access to player data
	Entity_Player* m_Player = nullptr;

	//Track current level
	unsigned m_CurrentLevel = 1;
	//Tracks currently held XP
	unsigned m_CurrentExperience = 0;
	//Tracks experience required for next level
	unsigned m_ExperienceRequired = 0;
	//Tracking bool for if a level is pending
	bool m_LevelPending = false;

	//Track kill count
	int m_KillCount = 0;

	//Due to proximity to Selection Process, host a ptr to this UI element (hosted in Mode_MainGame)
	UI_LevelUpSelector* m_LevelSelectorUI = nullptr;

};

