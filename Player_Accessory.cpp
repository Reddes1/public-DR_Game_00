#include "Player_Accessory.h"
#include "Game.h"

void Player_Accessory::Update(GameTimer& gt)
{

}

void Player_Accessory::InitialiseAccessory(AccessoryTypeID accessoryID, AccessoryCompositeData& statPack)
{
	//Default state is undefined, so shouldn't be re-defined as that
	msg_assert((accessoryID != AccessoryTypeID::UNDEFINED) && (accessoryID != AccessoryTypeID::COUNT),
		"InitialiseAccessory(): Accessory being init as invalid type");

	m_AccessoryID = accessoryID;
	m_GenData = statPack.m_GeneralData;
	m_LevelSys = statPack.m_LevelData;
	m_IconData = statPack.m_IconData;

	//Force level up due to current accessory structure
	LevelUpAccessory();
}

bool Player_Accessory::LevelUpAccessory()
{
	//Check that accessory is not already equal to max
	if (m_LevelSys.m_WeaponLevel < m_LevelSys.m_MaxLevel)
	{
		//Debug checking
		msg_assert(!m_LevelSys.m_Levels.empty(), "LevelUpAccessory(): No leveling data found!");
		msg_assert(&m_LevelSys.m_Levels[m_LevelSys.m_WeaponLevel], "LevelUpAccessory(): No level data found at index!");

		//Apply each effect related to the level at the index to the accessory stats
		for (auto& a : m_LevelSys.m_Levels[m_LevelSys.m_WeaponLevel].m_LevelData)
		{
			m_Player->SetPlayerStats(a);
		}

		//As index 0 points to Lv 2, increment after the fact
		++m_LevelSys.m_WeaponLevel;

		return true;
	}
	//Accessory Maxed, just exit out
	return false;
}

void Player_Accessory::SetPlayerPointer(Entity_Player* playerPtr)
{
	assert(playerPtr);
	m_Player = playerPtr;
}

void Player_Accessory::Init()
{

}

void Player_Accessory::Release()
{
	m_Player = nullptr;
}
