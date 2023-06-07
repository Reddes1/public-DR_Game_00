#pragma once

#include "D3DUtils.h"	//DX Types
#include "GameTimer.h"	//Game Time
#include "GameTypes.h"  //Stats & Data

//Foward Dec
class Entity_Player;
class Projectile_Interface;

/*
	Multi-purpose class that acts as any defined weapon type, performing init and sustained init behaviour
	on projectiles related to class (See Projectile_Interface for projectile parent class).
*/
class Player_Weapon
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	Player_Weapon() { Init(); }
	~Player_Weapon() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////
		
	//Main update window through the weapons behaviour is updated
	void Update(GameTimer& gt);

	//Initialise the weapon with desired stat package (optionally overrideable)
	void InitialiseWeapon(WeaponTypeID weaponID, WeaponCompositeData& dataPack);

	//Calls internal calls to re-sync the weapon with player stats as required
	void ResyncWeapon();

	//Checks to see if weapon is maxed or not
	bool IsLevelMaxed() { return m_LevelSys.m_WeaponLevel >= m_LevelSys.m_MaxLevel; }

	//Disables the weapon by setting type to undefined
	void DisableWeapon();

	///////////
	/// Get ///
	///////////

	WeaponStats& GetStats() { return m_Stats; }
	WeaponTypeID GetWeaponID() { return m_WeaponID; }
	GeneralEntityData& GetGeneralData() { return m_GenData; }
	LevelingData& GetLevelingSystem() { return m_LevelSys; }
	IconData& GetIconData() { return m_IconData; }

	///////////
	/// Set ///
	///////////

	//Must be done ONCE to link this weapon to the player (so that the weapon can access stats during update cycle)
	void SetPlayerPointer(Entity_Player* playerPtr);
	void SetData(GeneralEntityData& data) { m_GenData = data; }
	void SetStats(WeaponStats& stats) { m_Stats = stats; }

	///////////////
	/// Testing ///
	///////////////

	//Signals to the weapon to level up (if possible) when called
	bool LevelUpWeapon();
	//Uses given ID figure out how to apply stored data to weapon stats
	void ApplyLevelEffect(LevelUpData& data);

	//Resets tracked projectile (use only for debug)
	void ResetTrackedProjectile() { m_TrackedProj = nullptr; }

private:

	//////////////////
	/// Operations ///
	//////////////////

	//
	//General Functions
	//

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();
	
	//For any weapon specific setup after main init function called
	void WeaponExtraInit();

	//For use with weapons that fire and forget projectiles, using an interval value between extra projectiles
	//Example: Dagger, Cross, Whip
	void DowntickProjectileCount();

	//
	//Stat Calculation Functions (Using Player and Weapon Stats)
	//

	//Returns the player/weapon adjusted calculations as a value to apply to weapons
	short CalculateDamage();
	short CalculateProjectileSpeed();
	short CalculateKnockbackEffect();
	float CalculateLifetime();
	DirectX::XMFLOAT2 CalculateAoEScale(Projectile_Interface* const proj);

	//Alternative calculation for lifetime where lower lifetime is the goal (i.e., bomb)
	float CalculateInverseLifetime();

	//Internal Calculations
	void CalcRemainingCD();
	void CalcProjCount();
	
	float CalcLifetime();

	//
	//Weapon Update Functions
	//

	void WeaponUpdate_Cross(GameTimer& gt);
	void WeaponUpdate_Whip(GameTimer& gt);
	void WeaponUpdate_Dagger(GameTimer& gt);
	void WeaponUpdate_Bomb(GameTimer& gt);
	void WeaponUpdate_Aura(GameTimer& gt);
	void WeaponUpdate_Book(GameTimer& gt);
	void WeaponUpdate_Axe(GameTimer& gt);
	void WeaponUpdate_Hex(GameTimer& gt);
	void WeaponUpdate_Lightning(GameTimer& gt);

	////////////
	/// Data ///
	////////////

	//General data
	GeneralEntityData m_GenData;
	//Basic weapon stats
	WeaponStats m_Stats;
	//Weapon typing - Defines the behaviour of the class
	WeaponTypeID m_WeaponID = WeaponTypeID::UNDEFINED;
	//Leveling up data
	LevelingData m_LevelSys;
	//Icon Data to pass to UIs
	IconData m_IconData;

	//Binding pointer to the player
	Entity_Player* m_Player = nullptr;

	//Anchor position for certain weapons
	//(Used with: Whip, Book) 
	DirectX::XMFLOAT2 m_AnchorPos = { 0.f, 0.f };

	//In the cases where the weapon needs to know about a specific projectile, and update it directly (like aura)
	Projectile_Interface* m_TrackedProj = nullptr;

	//Quick general purpose flags for the different weapons to use as needed
	struct
	{
		bool m_FreshWeaponCycle : 1;
		bool m_Flag01 : 1;
		bool m_Flag02 : 1;
		bool m_Flag03 : 1;
		bool m_Flag04 : 1;
	} GenFlags;

	//Current remaining CD on weapon
	float m_RemainingCD = 2.f;
	short m_RemainingProjCount = 0;
	float m_Aura = 0.f;
};