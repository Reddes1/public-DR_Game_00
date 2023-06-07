#include "SM_ProgressionManager.h"

#include <algorithm> //C++17 - std::sample
#include <random>    //std::mt19937

#include "Game.h"					//Manager Access
#include "GameConstants.h"			//Game info

//Level Barriers and Increment amounts
#define LEVEL_BARRIER_00 1
#define LEVEL_BARRIER_01 10
#define LEVEL_BARRIER_02 20
#define LEVEL_BARRIER_03 40
#define LEVEL_BARRIER_04 60
#define LEVEL_BARRIER_05 80

#define XP_INC_00 25
#define XP_INC_01 55
#define XP_INC_02 80
#define XP_INC_03 110
#define XP_INC_04 130
#define XP_INC_05 250

#define BASE_XP 5
#define XP_BARRIER_MOD 3.0f

void SM_ProgressionManager::BindLevelSelectorUI(UI_LevelUpSelector* ptr)
{
	assert(ptr);
	m_LevelSelectorUI = ptr;
}

void SM_ProgressionManager::ResetProgress()
{
	m_CurrentLevel = 1;
	m_ExperienceRequired = BASE_XP;
	m_CurrentExperience = 0;
	ResetKillCount();
}

unsigned SM_ProgressionManager::GetExperienceForLevel(unsigned level)
{
	//Based on level, return the amount of XP required to hit next level		
	if (level > LEVEL_BARRIER_05)	  //Lv81+
		return BASE_XP + (XP_INC_05 * (level - 1)); 

	else if (level > LEVEL_BARRIER_04) //Lv61-Lv80
		return BASE_XP + (XP_INC_04 * (level - 1));

	else if (level > LEVEL_BARRIER_03) //Lv41-Lv60
		return BASE_XP + (XP_INC_03 * (level - 1));
	
	else if (level > LEVEL_BARRIER_02) //Lv21-Lv40
		return BASE_XP + (XP_INC_02 * (level - 1));
	
	else if (level > LEVEL_BARRIER_01) //Lv11-Lv20
		return BASE_XP + (XP_INC_01 * (level - 1));
	
	else if (level > LEVEL_BARRIER_00)	//Lv2-Lv10
		return BASE_XP + (XP_INC_00 * (level - 1));
	
	else if (level == 1)
		return BASE_XP;

	return 0;
}

void SM_ProgressionManager::Update()
{
	//Check for a pending level, and call level up if so
	if (m_LevelPending)
		LevelUp();
}

void SM_ProgressionManager::Release()
{
	//Let go of pointer (released elsewhere)
	m_LevelSelectorUI = nullptr;
}

void SM_ProgressionManager::AddExp(unsigned exp)
{
	//Play audio
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::EXP, GameVolumes::SFX);
		
	//Before applying experience, check to see player is currently on a breakpoint level (where a significant amount of experience is required)
	bool barrierLvl = (
		m_CurrentLevel == LEVEL_BARRIER_00 + 1 ||
		m_CurrentLevel == LEVEL_BARRIER_01 + 1 || 
		m_CurrentLevel == LEVEL_BARRIER_02 + 1 ||
		m_CurrentLevel == LEVEL_BARRIER_03 + 1 ||
		m_CurrentLevel == LEVEL_BARRIER_04 + 1 ||
		m_CurrentLevel == LEVEL_BARRIER_05 + 1);
	
	//If true, apply mod to experience
	if (barrierLvl)
		exp = static_cast<unsigned>(exp * XP_BARRIER_MOD);
	
	//If the experience would tick over, begin levelup prep
	if (m_CurrentExperience + exp >= m_ExperienceRequired)
	{
		//If the barrier bonus is active, we want to find the left over experience after level, and reduce it
		//back down to regular xp to prevent "xp gaming" with large quantities being applied at once
		if (barrierLvl)
		{
			//Get the difference
			unsigned diff = m_ExperienceRequired - m_CurrentExperience;
			//Add the difference
			m_CurrentExperience += diff;
			//Deduct the difference from exp, the add it after reduction
			exp -= diff;
			m_CurrentExperience = m_CurrentExperience + static_cast<unsigned>(exp * 0.5f);
		}
		//Add it normally
		else
		{
			m_CurrentExperience += exp;
		}

		//Signal leveling
		m_LevelPending = true;
	}
	//Just add it normally
	else
		m_CurrentExperience += exp;
}

void SM_ProgressionManager::ForceLevelUp()
{
	//Set current experience to current requirement
	m_CurrentExperience = m_ExperienceRequired;
	//Level up as usual
	LevelUp();
}

void SM_ProgressionManager::LevelUp()
{
	//Increment current level
	++m_CurrentLevel;

	//Calculate experience required for next level
	m_ExperienceRequired = GetExperienceForLevel(m_CurrentLevel);

	//If the new experience amount overflows , signal another level up
	if (m_CurrentExperience >= m_ExperienceRequired)
		m_LevelPending = true;
	else
		m_LevelPending = false;

	//If selection process found valid options, then just change game state
	if (RunSelectionProcess())
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::LEVEL_SELECTION_OPEN);
	//Else, just heal the player an amount
	else
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->ApplyHealingEffect(HEAL_AMOUNT, true);

	//Play audio
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::LEVEL_UP, GameVolumes::SFX);
}

void SM_ProgressionManager::FindAvailableUpgrades(std::vector<SelectionData>& dataContainer)
{
	///////////////////////////////////
	/// Selection Gathering Process ///
	///////////////////////////////////

	//Prime container with all weapons and accessories
	const unsigned maxWeapons = (unsigned)WeaponTypeID::UNDEFINED;
	const unsigned maxAccessories = (unsigned)AccessoryTypeID::UNDEFINED;
	for (unsigned i(0); i < maxWeapons + maxAccessories; ++i)
	{
		SelectionData data;
		if (i < maxWeapons)
		{
			data.m_EquipmentType = 0; //Relates to being a weapon
			data.m_EquipmentID = i;
		}
		else
		{
			data.m_EquipmentType = 1; //Relates to being a accessory
			data.m_EquipmentID = i - maxWeapons;
		}

		dataContainer.push_back(data);
	}
	//Go through weapon array and determine what the player currently owns
	auto weapons = m_Player->GetWeapons();
	for (unsigned i(0); i < maxWeapons; ++i)
	{
		//Check to see if any of the players weapons match the current slot
		for (unsigned j(0); j < MAX_WEAPON_COUNT; ++j)
		{
			if ((unsigned)weapons[j].GetWeaponID() == dataContainer[i].m_EquipmentID)
			{
				//Match found, so determine how to flag
				dataContainer[i].m_PlayerHas = true;
				dataContainer[i].m_Maxed = weapons[j].IsLevelMaxed();
			}
		}
	}
	//Go through accessory array and determine what the player currently owns
	auto accessories = m_Player->GetAccessories();
	for (unsigned i(maxWeapons); i < maxWeapons + maxAccessories; ++i)
	{
		//Check to see if any of the players accessories match the current slot
		for (unsigned j(0); j < MAX_ACCESSORY_COUNT; ++j)
		{
			if ((unsigned)accessories[j].GetAccessoryID() == dataContainer[i].m_EquipmentID)
			{
				//Match found, so determine how to flag
				dataContainer[i].m_PlayerHas = true;
				dataContainer[i].m_Maxed = accessories[j].IsLevelMaxed();
			}
		}
	}

}
bool SM_ProgressionManager::RunSelectionProcess()
{
	//Create and populate selection data
	std::vector<SelectionData> selectionData;
	FindAvailableUpgrades(selectionData);

	/////////////////
	/// Filtering ///
	/////////////////

	//In the case of weapon/accessory slots being filled out, filter out the options where the player doesn't own the weapon so they can't be offered later
	bool weaponsFull = !m_Player->AnyFreeWeaponSlots();
	bool accessoriesFull = !m_Player->AnyFreeAccessorySlots();

	FilterMaxedEquipment(selectionData);
	if (weaponsFull)
		FilterUnownedWeapons(selectionData);
	if (accessoriesFull)
		FilterUnownedAccessories(selectionData);

	/////////////////////////
	/// Selection Process ///
	/////////////////////////

	//Isolate the choices out into seperate container
	std::vector<SelectionData> options;
	options.reserve(MAX_LEVEL_OPTIONS);

	SelectionRandom(options, selectionData);

	//If no options found, return false
	if (options.empty())
		return false;

	/////////////////
	/// Build UIs ///
	/////////////////

	BuildLevelSelector(options);

	//At least one option found, so return true
	return true;
}

bool SM_ProgressionManager::ApplyChestUpgrade()
{
	//Create and populate selection data
	std::vector<SelectionData> selectionData;
	FindAvailableUpgrades(selectionData);

	/////////////////
	/// Filtering ///
	/////////////////

	FilterMaxedEquipment(selectionData);
	FilterUnownedWeapons(selectionData);
	FilterUnownedAccessories(selectionData);

	/////////////////////////
	/// Selection Process ///
	/////////////////////////

	// Exit if no upgrades are avaiable
	if (selectionData.size() == 0)
		return false;

	//Isolate the choices out into seperate container
	std::vector<SelectionData> options;
	options.reserve(MAX_LEVEL_OPTIONS);

	//Select randomly
	SelectionRandom(options, selectionData);

	//If no options found, return false
	if (options.empty())
		return false;

	//Till UI complete, pop options other than the first found, then pass to UI temp UI
	while (options.size() >= 2)
		options.pop_back();

	/////////////////
	/// Build UIs ///
	/////////////////

	BuildLevelSelector(options);

	//An option found, return true
	return true;
}
void SM_ProgressionManager::FilterMaxedEquipment(std::vector<SelectionData>& arr)
{
	auto it = std::remove_if(arr.begin(), arr.end(), [&](SelectionData& data) { return data.m_Maxed; });
	arr.erase(it, arr.end());
}

void SM_ProgressionManager::FilterUnownedWeapons(std::vector<SelectionData>& arr)
{
	auto it = std::remove_if(arr.begin(), arr.end(), [&](SelectionData& data) {return (data.m_EquipmentType == 0 && !data.m_PlayerHas); });
	arr.erase(it, arr.end());
}

void SM_ProgressionManager::FilterUnownedAccessories(std::vector<SelectionData>& arr)
{
	auto it = std::remove_if(arr.begin(), arr.end(), [&](SelectionData& data) {return (data.m_EquipmentType == 1 && !data.m_PlayerHas); });
	arr.erase(it, arr.end());
}

bool SM_ProgressionManager::SelectionRandom(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections)
{
	//Random Sample (Max - 1 till luck implemented)
	std::sample(
		selections.begin(),
		selections.end(),
		std::back_inserter(dst),
		MAX_LEVEL_OPTIONS - 1,
		std::mt19937{ std::random_device{}() }
	);

	//If at least one selection is made, return true
	if(!dst.empty())
		return true;
	
	//No selection made, return false
	return false;
}

bool SM_ProgressionManager::SelectionWeaponBias(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections)
{
	//Get rand device and shuffle the container
	auto rand = std::random_device();
	std::shuffle(selections.begin(), selections.end(), rand);

	//Look for first owned, non-maxed weapon
	for (auto& a : selections)
	{
		//If valid selection found, store it and return true
		if (a.m_EquipmentID == (unsigned)EquipmentTypeID::WEAPON && a.m_PlayerHas && !a.m_Maxed)
		{
			dst.push_back(a);
			return true;
		}
	}
	
	//No valid selection found, return false
	return false;
}

bool SM_ProgressionManager::SelectionAccessoryBias(std::vector<SelectionData>& dst, std::vector<SelectionData>& selections)
{
	//Get rand device and shuffle the container
	auto rand = std::random_device();
	std::shuffle(selections.begin(), selections.end(), rand);

	//Look for first owned, non-maxed accessory
	for (auto& a : selections)
	{
		//If valid selection found, store it and return true
		if (a.m_EquipmentID == (unsigned)EquipmentTypeID::ACCESSORY && a.m_PlayerHas && !a.m_Maxed)
		{
			dst.push_back(a);
			return true;
		}
	}
	
	//No valid selection found, return false
	return false;
}

void SM_ProgressionManager::BuildLevelSelector(std::vector<SelectionData>& options)
{
	//Clear UI before starting
	m_LevelSelectorUI->Reset();

	//Get size and align it
	unsigned size = (unsigned)options.size();
	if (size > 0) --size;
	m_LevelSelectorUI->SetMaxIndex(size);

	for (unsigned i(0); i < options.size(); ++i)
	{
		//Initialising weapon or accessory selection
		if (options[i].m_EquipmentType == 0)
		{
			//Determine if its a fresh weapon or player owned
			if (options[i].m_PlayerHas && !options[i].m_Maxed)
			{
				//Find player owned weapon
				Player_Weapon* const weapon = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetWeaponByTypeID(options[i].m_EquipmentID);
				msg_assert(weapon, "TestingSelectionSystem(): Weapon Not Found! Error in Selection Process!");

				//Pass weapon to UI to fill option
				m_LevelSelectorUI->InitialiseOptionAsOwnedWeapon(i, weapon);
			}
			else if (!options[i].m_PlayerHas && !options[i].m_Maxed)
			{
				//Find a free slot for the new weapon
				m_LevelSelectorUI->InitialiseOptionAsUnownedWeapon(i, options[i].m_EquipmentID);
			}
		}
		else if (options[i].m_EquipmentType == 1)
		{
			//Determine if its a fresh accessory or player owned
			if (options[i].m_PlayerHas && !options[i].m_Maxed)
			{
				//Find player owned accessory
				Player_Accessory* const accessory = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetAccessoryByTypeID(options[i].m_EquipmentID);
				msg_assert(accessory, "TestingSelectionSystem(): Accessory Not Found! Error in Selection Process!");

				//Pass accessory to UI to fill option entry out
				m_LevelSelectorUI->InitialiseOptionAsOwnedAccessory(i, accessory);
			}
			else if (!options[i].m_PlayerHas && !options[i].m_Maxed)
			{
				//Find a free slot for the new accessory
				m_LevelSelectorUI->InitialiseOptionAsUnownedAccessory(i, options[i].m_EquipmentID);
			}
		}
		//Something broke if it got here
		else
			msg_assert(false, "TestingSelectionSystem(): Equipment Type error in UI builder!");
	}
}
