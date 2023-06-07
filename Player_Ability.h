#pragma once

#include "GameTypes.h"
#include "GameTimer.h"

//Foward Dec
class Entity_Player;

/*
	Player ability module, allowing for powerful actions on a cooldown.s
*/
class Player_Ability
{
public:

	///////////////////
	/// Definitions ///
	///////////////////


	////////////////////
	/// Constructors ///
	////////////////////

	Player_Ability() { Init(); }
	~Player_Ability() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Inits the ability based on ID
	void InitialiseAbility(unsigned id);

	//Main Update Window
	void Update(GameTimer& gt);

	//Activates the ability (Check with IsAbilityUsable first).
	void ActivateAbility();

	//Resets data
	void Reset();

	///////////
	/// Get ///
	///////////

	GeneralEntityData& GetGeneralData() { return m_GenData; }
	WeaponStats& GetStats() { return m_Stats; }

	WeaponTypeID GetAbilityID() { return m_ID; }
	float GetCurrentCD() { return m_ElapsedCD; }
	bool IsAbilityUsable() { return m_ElapsedCD >= m_Stats.m_Cooldown; }

	///////////
	/// Set ///
	///////////

	//Must be done ONCE to link this to the player
	void SetPlayerPointer(Entity_Player* playerPtr);

	void SetCDToMax() { m_ElapsedCD = m_Stats.m_Cooldown; }
	void SetCDToHalfMax() { m_ElapsedCD = m_Stats.m_Cooldown * 0.5f; }

private:

	//////////////////
	/// Operations ///
	//////////////////
	
	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//
	//Ability Setups
	//

	void SetupDragon();
	void SetupBulletHell();
	void SetupHoH();

	////////////
	/// Data ///
	////////////

	//General data
	GeneralEntityData m_GenData;
	//Stats imparted to projectiles/controller classes
	WeaponStats m_Stats;
	//Type for determining what this ability represents
	WeaponTypeID m_ID = WeaponTypeID::UNDEFINED;

	//Binding pointer to the player
	Entity_Player* m_Player = nullptr;

	//Track the current CD and the max
	float m_ElapsedCD = 0.f;
};