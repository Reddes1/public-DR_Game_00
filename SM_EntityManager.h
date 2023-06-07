#pragma once

#include "EntityPool.h"	//Pooling System
#include "RapidJSONLoaderUtils.h"	//File Loading
#include "EngineDefinitions.h" //Draw Depth

//Game Entities (Parent classes, child classes in .cpp)
#include "EM_EntityInterface.h"
#include "Entity_Player.h"
#include "Projectile_Interface.h"
#include "Loot_Interface.h"
#include "Effects_AnimSprite.h"

/*
	One stop shop for all your entity holding needs.
	Stores the numerous types of entities used in the game, holding a limited pool of a available entities.
*/
class SM_EntityManager
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class PoolID
	{
		ENTITY_00,
		ENTITIES_01_02_03,
		PLAYER_PROJ,
		ENEMY_PROJ,
		LOOT,
		EFFECTS_00,
		COUNT
	};


	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	SM_EntityManager() { Init(); }
	~SM_EntityManager() { Release(); }


	//////////////////
	/// Operations ///
	//////////////////

	//Primary init function for class, loading pool setup from file (CALL ONCE!)
	void InitialiseEntityManager(std::string& fp);

	//
	// Main Operations
	//

	//Update Player
	void UpdatePlayer(GameTimer& gt);
	//Render Player
	void RenderPlayer(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);

	//Granular control on updating individual pool using PoolID
	void UpdatePool(PoolID poolID, GameTimer& gt);
	//Granular control on rendering indivdual pool using PoolID
	void RenderPool(PoolID poolID, DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);
	//Support function that calls FlagAllObjects(false)
	void TurnOffAllPools();

	///////////
	/// Get ///
	///////////

	Entity_Player* GetPlayer() { return m_Player; }

	//Get Entity Pool hosting "EM_EntityInterface2D" derived by ID (Relates to PoolID enum)
	EntityPool<EM_EntityInterface2D*>* GetEntityPoolAtID(PoolID poolID);
	//Get Entity Pool hosting "Projectile_Interface" derived classes by ID (Relates to PoolID enum)
	EntityPool<Projectile_Interface*>* GetProjectilePoolAtID(PoolID poolID);
	//Get Loot pool
	EntityPool<Loot_Interface*>* GetLootPool() { return m_Loot; };
	//Get Effects Pool
	EntityPool<Effects_AnimSprite*>* GetEffectsPool() { return m_EffectsPool00; }

	//Gets any random, active entity from target entity pool
	EM_EntityInterface2D* GetRandomActiveEntityFromPool(PoolID poolID);
	//Gets any random, active entity from target entity pool that must be on screen (with loop duration limitations).
	EM_EntityInterface2D* GetRandomActiveEntityFromPoolScreenCheck(PoolID poolID);

	bool GetSleep() { return m_SleepActive; }

	///////////
	/// Set ///
	///////////

	void SetSleep(bool isSleep) { m_SleepActive = isSleep; }

private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//Runs offscreen check
	bool IsOffScreen(EM_EntityInterface2D* const ent);

	//
	//Pool Inits
	//

	//Initialises the main entity pools + player
	void InitialiseEnemyPools(rapidjson::Document& doc);
	//Initialises the player projectile pool with given file
	void InitialisePlayerProjectiles(rapidjson::Document& doc);
	//Initialises enemy projectile pool
	void InitialiseEnemyProjectiles(rapidjson::Document& doc);
	//Initialises Loot Pool
	void InitialiseLoot(rapidjson::Document& doc);
	//Initialised Effect Pool(s)
	void InitialiseEffects(rapidjson::Document& doc);

	//
	//Init Supports
	//

	//Use in player projectile setup
	template<class PTR_TYPE>
	void SetupPlayerProjectile(PTR_TYPE ptr, rapidjson::Value& arr, unsigned index, unsigned short count);
	//Support function for setting up enemy pools
	template<class PTR_TYPE>
	void SetupEnemyEntry(PTR_TYPE ptr, unsigned entityID, unsigned poolID, unsigned short count);

	////////////
	/// Data ///
	////////////

	//
	//Pools
	//

	//Several different pools of objects used in game	//Pool Usages
	EntityPool<EM_EntityInterface2D*>* m_EnemyPool00;	//Enemy Type 00 (Most Common Enemy)
	EntityPool<EM_EntityInterface2D*>* m_EnemyPool01;	//Enemy Types 01, 02, 03 (Least Common Enemies)
	EntityPool<Projectile_Interface*>* m_PlayerProj00;	//Player Projectiles (For All Weapons)
	EntityPool<Projectile_Interface*>* m_EnemyProj00;	//Enemy Projectiles
	EntityPool<Loot_Interface*>* m_Loot;				//Loot

	//
	//Particles/Effects
	//

	EntityPool<Effects_AnimSprite*>* m_EffectsPool00; 

	//Player Entity
	Entity_Player* m_Player = nullptr;


	//Tracks how long the enemy should sleep for
	float m_Sleep = 0.f;
	float m_MaxSleep = 30.f;

	bool m_SleepActive = false;
};

template<class PTR_TYPE>
inline void SM_EntityManager::SetupPlayerProjectile(PTR_TYPE ptr, rapidjson::Value& arr, unsigned index, unsigned short count)
{
	//Initialise pool data and IDs
	ptr->InitPoolData((unsigned short)PoolID::PLAYER_PROJ, count);
	ptr->SetWeaponID(arr[index]["Weapon ID"].GetUint());

	//Texture/Animation Data
	if (arr[index]["Textures"][0]["Animated"].GetBool() == true)
		ptr->InitialiseVisuals(
			std::string(arr[index]["Textures"][0]["Texture Name"].GetString()),
			arr[index]["Textures"][0]["Frame ID"].GetUint(),
			true, true, false
		);
	else
		ptr->InitialiseVisuals(
			std::string(arr[index]["Textures"][0]["Texture Name"].GetString()),
			arr[index]["Textures"][0]["Frame ID"].GetUint()
		);

	//Colision Data + Misc
	ptr->GetGeneralData().m_CollisionID = arr[index]["Misc"]["Collision Type ID"].GetUint();
	ptr->GetGeneralData().m_CollisionRadius = arr[index]["Misc"]["Collision Radius"].GetFloat();
	ptr->GetGeneralData().m_CollisionBox = {
		arr[index]["Misc"]["Collision Box"][0].GetFloat(),
		arr[index]["Misc"]["Collision Box"][1].GetFloat()
	};
	ptr->GetGeneralData().m_Scale = {
		arr[index]["Misc"]["Scale Adjustments"][0].GetFloat(),
		arr[index]["Misc"]["Scale Adjustments"][1].GetFloat()
	};
	ptr->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_0);

	//Store at pool index
	m_PlayerProj00->GetObjectAtIndex(count) = ptr;
}

template<class PTR_TYPE>
inline void SM_EntityManager::SetupEnemyEntry(PTR_TYPE ptr, unsigned entityID, unsigned poolID, unsigned short count)
{
	//Setup IDs
	ptr->GetPoolData().m_PoolID = (unsigned short)poolID;
	ptr->GetPoolData().m_PoolIndex = (unsigned short)count;
	ptr->SetID(entityID);
	
	//Determine what pool to store in
	switch (poolID)
	{
	case 0:
		m_EnemyPool00->GetObjectAtIndex(count) = ptr;
		ptr->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_7);
		break;
	case 1:
		m_EnemyPool01->GetObjectAtIndex(count) = ptr;
		ptr->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_8);
		break;
	}
}
