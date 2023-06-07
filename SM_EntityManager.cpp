#include "SM_EntityManager.h"
#include "CollisionTypes.h"

#include "GameConstants.h"	//Global Info
#include "Game.h"			//Manager Access
#include "AnimationDefinitions.h"

//Game Entities (Child Classes)
#include "Entity_EnemyType_00.h"
#include "Entity_EnemyType_01.h"
#include "Entity_EnemyType_02.h"
#include "Entity_EnemyType_03.h"

//Player Projectiles
#include "Projectile_Cross.h"
#include "Projectile_Whip.h"
#include "Projectile_Dagger.h"
#include "Projectile_Bomb.h"
#include "Projectile_Aura.h"
#include "Projectile_Book.h"
#include "Projectile_Axe.h"
#include "Projectile_HexSteps.h"
#include "Projectile_Lightning.h"

//Abilities
#include "Projectile_WaterDragon.h"
#include "Projectile_BulletHell.h"
#include "Projectile_LightBullet.h"
#include "Projectile_HoH.h"
#include "Projectile_HolyBeam.h"
#include "Projectile_Super.h"

//Loot Types
#include "Loot_Experience.h"
#include "Loot_Food.h"
#include "Loot_Magnet.h"
#include "Loot_Chest.h"
#include "Loot_Super.h"
#include "Loot_Wipe.h"
#include "Loot_Freeze.h"


void SM_EntityManager::Init()
{
	//Allocate for pools
	m_EnemyPool00 = new EntityPool<EM_EntityInterface2D*>();
	m_EnemyPool01 = new EntityPool<EM_EntityInterface2D*>();
	m_PlayerProj00 = new EntityPool<Projectile_Interface*>();
	m_EnemyProj00 = new EntityPool<Projectile_Interface*>();
	m_Loot = new EntityPool<Loot_Interface*>();
	m_EffectsPool00 = new EntityPool<Effects_AnimSprite*>();
}

void SM_EntityManager::Release()
{
	//Release Pools
	delete m_EnemyPool00;
	delete m_EnemyPool01;
	delete m_PlayerProj00;
	delete m_EnemyProj00;
	delete m_Loot;
	delete m_EffectsPool00;

	//Release Player
	delete m_Player;
	m_Player = nullptr;
}

bool SM_EntityManager::IsOffScreen(EM_EntityInterface2D* const ent)
{
	//Get data
	float winX = Game::GetGame()->GetWindowWidthF() * 0.5f;
	float winY = Game::GetGame()->GetWindowHeightF() * 0.5f;
	DirectX::XMFLOAT2 scrnCtr = m_Player->GetRelativePosition();
	DirectX::XMFLOAT2 pos = { ent->GetCollisionData().m_CollisonBox.posX, ent->GetCollisionData().m_CollisonBox.posY };

	//Check against the edges and return true if off any of the sides
	if ((scrnCtr.x - winX) > pos.x ||
		(scrnCtr.x + winX) < pos.x ||
		(scrnCtr.y - winY) > pos.y ||
		(scrnCtr.y + winY) < pos.y)
		return true;
 
	return false;
}

void SM_EntityManager::UpdatePlayer(GameTimer& gt)
{
	//Update Player
	m_Player->Update(gt);
}

void SM_EntityManager::RenderPlayer(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Render Player
	m_Player->Render(batch, heap);
}

void SM_EntityManager::UpdatePool(PoolID poolID, GameTimer& gt)
{
	if (GetSleep())
	{ 
		m_Sleep += gt.DeltaTime();
		if (m_Sleep >= m_MaxSleep)
		{
			SetSleep(false);
			m_Sleep = 0;
		}
	}
	//Render a pool via ID
	switch (poolID)
	{
	case PoolID::ENTITY_00:
		m_EnemyPool00->UpdatePool(gt);
		break;
	case PoolID::ENTITIES_01_02_03:
		m_EnemyPool01->UpdatePool(gt);
		break;
	case PoolID::PLAYER_PROJ:
		m_PlayerProj00->UpdatePool(gt);
		break;
	case PoolID::ENEMY_PROJ:
		m_EnemyProj00->UpdatePool(gt);
		break;
	case PoolID::LOOT:
		m_Loot->UpdatePool(gt);
		break;
	case PoolID::EFFECTS_00:
		m_EffectsPool00->UpdatePool(gt);
		break;
	}
}

void SM_EntityManager::RenderPool(PoolID poolID, DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Render a pool via ID
	switch (poolID)
	{
	case PoolID::ENTITY_00:
		m_EnemyPool00->RenderPool(batch, heap);
		break;
	case PoolID::ENTITIES_01_02_03:
		m_EnemyPool01->RenderPool(batch, heap);
		break;
	case PoolID::PLAYER_PROJ:
		m_PlayerProj00->RenderPool(batch, heap);
		break;
	case PoolID::ENEMY_PROJ:
		m_EnemyProj00->RenderPool(batch, heap);
		break;
	case PoolID::LOOT:
		m_Loot->RenderPool(batch, heap);
		break;
	case PoolID::EFFECTS_00:
		m_EffectsPool00->RenderPool(batch, heap);
		break;
	}
}

void SM_EntityManager::TurnOffAllPools()
{
	m_EnemyPool00->FlagAllObjects(false);
	m_EnemyPool01->FlagAllObjects(false);
	m_EnemyProj00->FlagAllObjects(false);
	m_PlayerProj00->FlagAllObjects(false);
	m_Loot->FlagAllObjects(false);
	m_EffectsPool00->FlagAllObjects(false);
}

EntityPool<EM_EntityInterface2D*>* SM_EntityManager::GetEntityPoolAtID(PoolID poolID)
{
	//Return access to pool via ID
	switch (poolID)
	{
	case PoolID::ENTITY_00:
		return m_EnemyPool00;
		break;
	case PoolID::ENTITIES_01_02_03:
		return m_EnemyPool01;
		break;
	}

	//An inappropriate ID was passed, and this should generally never happen so assert false and return false
	msg_assert(false, "GetEntityPoolAtID(): Inappropriate or NYI pool code passed, no returning pool.");
	return nullptr;
}

EntityPool<Projectile_Interface*>* SM_EntityManager::GetProjectilePoolAtID(PoolID poolID)
{
	switch (poolID)
	{
	case PoolID::PLAYER_PROJ:
		return m_PlayerProj00;
		break;
	case PoolID::ENEMY_PROJ:
		return m_EnemyProj00;
		break;
	}

	//An inappropriate ID was passed, and this should generally never happen so assert false and return false
	msg_assert(false, "GetProjectilePoolAtID(): Inappropriate or NYI pool code passed, no returning pool.");
	return nullptr;
}

EM_EntityInterface2D* SM_EntityManager::GetRandomActiveEntityFromPool(PoolID poolID)
{
	switch (poolID)
	{
	case PoolID::ENTITY_00:

		//As long as there any active entities, randomly select till active found
		if (m_EnemyPool00->GetInUseCount() != 0)
		{
			for (;;)
			{
				//Target random selection to last known highest active index
				unsigned index = MathHelper::Rand(0, m_EnemyPool00->GetHighestInUseIndex());
				assert(index < m_EnemyPool00->GetPoolSize());

				//If active index found
				if (m_EnemyPool00->GetUseFlagAtIndex(index))
				{
					return m_EnemyPool00->GetObjectAtIndex(index);
				}					
			}
		}
		//No active units
		else
			return nullptr;
		break;

	case PoolID::ENTITIES_01_02_03:
		
		//As long as there any active entities, randomly select till active found
		if (m_EnemyPool01->GetInUseCount() != 0)
		{
			for (;;)
			{
				//Target random selection to last known highest active index
				unsigned index = MathHelper::Rand(0, m_EnemyPool01->GetHighestInUseIndex());
				assert(index < m_EnemyPool01->GetPoolSize());

				if (m_EnemyPool01->GetUseFlagAtIndex(index))
					return m_EnemyPool01->GetObjectAtIndex(index);
			}
		}
		//No active units
		else
			return nullptr;		
		break;

	default:
		msg_assert(false, "GetRandomActiveEntityFromPool(): Invalid poolID passed, no entity will be found!");
	}

	//Shouldn't be hit
	return nullptr;
}

EM_EntityInterface2D* SM_EntityManager::GetRandomActiveEntityFromPoolScreenCheck(PoolID poolID)
{
	switch (poolID)
	{
	case PoolID::ENTITY_00:

		//As long as there any active entities, randomly select till active found
		if (m_EnemyPool00->GetInUseCount() != 0)
		{
			//As the screen check may cause scenario where the loop cannot end, impose a limitation on check amount
			for (unsigned i(0); i < m_EnemyPool00->GetPoolSize(); ++i)
			{
				//Target random selection to last known highest active index
				unsigned index = MathHelper::Rand(0, m_EnemyPool00->GetHighestInUseIndex());
				assert(index < m_EnemyPool00->GetPoolSize());

				//If active index found
				if (m_EnemyPool00->GetUseFlagAtIndex(index) && !(IsOffScreen(m_EnemyPool00->GetObjectAtIndex(index))))
				{
					return m_EnemyPool00->GetObjectAtIndex(index);
				}
			}

			//Loop ended, return nullptr
			return nullptr;
		}
		//No active units
		else
			return nullptr;
		break;

	case PoolID::ENTITIES_01_02_03:

		//As long as there any active entities, randomly select till active found
		if (m_EnemyPool01->GetInUseCount() != 0)
		{
			//As the screen check may cause scenario where the loop cannot end, impose a limitation on check amount
			for (unsigned i(0); i < m_EnemyPool01->GetPoolSize(); ++i)
			{
				//Target random selection to last known highest active index
				unsigned index = MathHelper::Rand(0, m_EnemyPool01->GetHighestInUseIndex());
				assert(index < m_EnemyPool01->GetPoolSize());

				//If active index found
				if (m_EnemyPool01->GetUseFlagAtIndex(index) && !(IsOffScreen(m_EnemyPool01->GetObjectAtIndex(index))))
				{
					return m_EnemyPool01->GetObjectAtIndex(index);
				}
			}
			
			//Loop ended, return nullptr
			return nullptr;
		}
		//No active units
		else
			return nullptr;
		break;

	default:
		msg_assert(false, "GetRandomActiveEntityFromPool(): Invalid poolID passed, no entity will be found!");
	}

	//Shouldn't be hit
	return nullptr;
}

void SM_EntityManager::InitialiseEntityManager(std::string& fp)
{
	//First, Allocate for player
	m_Player = new Entity_Player();

	//Next parse and create document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Init pools with document
	InitialiseEnemyPools(doc);
	InitialisePlayerProjectiles(doc);
	InitialiseEnemyProjectiles(doc);
	InitialiseLoot(doc);
	InitialiseEffects(doc);
}

void SM_EntityManager::InitialiseEnemyPools(rapidjson::Document& doc)
{
	//Get array for brevity
	rapidjson::Value& arr = doc["Enemy Pools"].GetArray();

	//Each entity gets a slice of the pool, so keep count of total allocations made
	unsigned count = 0;
	unsigned maxAlloc = 0;

	//Precount allocation amount, and check for overflow
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		//For each entity type being allocated for
		for (unsigned j(0); j < arr[i]["Allocations"].Size(); ++j)
		{
			maxAlloc += arr[i]["Allocations"][j]["Allocation Count"].GetUint();
		}
	
		//Based on current index, check against appropriate pool (even if pools are currently the same across all, if new base type used, this could change).
		switch (i)
		{
		case 0:
			msg_assert(maxAlloc <= m_EnemyPool00->GetPoolSize(), "InitialiseMainPools(): Init total count greater than pool size!");
			break;
		case 1:
			msg_assert(maxAlloc <= m_EnemyPool01->GetPoolSize(), "InitialiseMainPools(): Init total count greater than pool size!");
			break;
		}
		//Reset alloc for next loop
		maxAlloc = 0;
	}

	//For each created pool (use pool ID instead of arr size as it may contain more than the pools enabled)
	for (unsigned i(0); i < (unsigned)PoolID::ENTITIES_01_02_03+1; ++i)
	{
		//For each allocations entry, create that many entities
		for (unsigned j(0); j < arr[i]["Allocations"].Size(); ++j)
		{
			//For each allocation
			for (unsigned k(0); k < arr[i]["Allocations"][j]["Allocation Count"].GetUint(); ++k)
			{
				//Based on Type ID, pass appropriate data to support function

				switch ((EnemyTypeID)arr[i]["Allocations"][j]["Enemy Type ID"].GetUint())
				{
				case EnemyTypeID::CHASER_00:
				{
					Entity_EnemyType_00* ent = new Entity_EnemyType_00();
					SetupEnemyEntry(ent, (unsigned)EnemyTypeID::CHASER_00, i, count);
				}
					break;
				case EnemyTypeID::EXPLODER_00:
				{
					Entity_EnemyType_01* ent = new Entity_EnemyType_01();
					SetupEnemyEntry(ent, (unsigned)EnemyTypeID::EXPLODER_00, i, count);
				}
					break;
				case EnemyTypeID::SHOOTER_00:
				{
					Entity_EnemyType_02* ent = new Entity_EnemyType_02();
					SetupEnemyEntry(ent, (unsigned)EnemyTypeID::SHOOTER_00, i, count);
				}
					break;
				case EnemyTypeID::SWEEPER_00:
				{
					Entity_EnemyType_03* ent = new Entity_EnemyType_03();
					SetupEnemyEntry(ent, (unsigned)EnemyTypeID::SWEEPER_00, i, count);
				}
					break;
				default:
					msg_assert(false, "InitialiseMainPools(): Enemy ID passed invalid!");
				}

				++count;
			}
		}
		//Reset count for next pool
		count = 0;
	}
}

void SM_EntityManager::InitialisePlayerProjectiles(rapidjson::Document& doc)
{
	//Get array for brevity
	rapidjson::Value& arr = doc["Player Projectile Pool"].GetArray();
	
	//Each projectile gets a slice of the pool, so keep count of total allocations made
	unsigned count = 0;
	unsigned maxAlloc = 0;

	//Precount allocation amount, and check for overflow
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		maxAlloc += arr[i]["Allocation Count"].GetUint();
	}
	msg_assert(maxAlloc <= m_PlayerProj00->GetPoolSize(), "InitialisePlayerProjectiles(): Init total count greater than pool size!");

	//For each projectile, allocate based on count
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		for (unsigned j(0); j < arr[i]["Allocation Count"].GetUint(); ++j)
		{
			//Determine type, performing generic init process + any specific requirements to that weapon
			switch ((WeaponTypeID)arr[i]["Weapon ID"].GetUint())
			{
			case WeaponTypeID::CROSS:
			{
				Projectile_Cross* ent = new Projectile_Cross();
				SetupPlayerProjectile(ent, arr, i, count);	
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_5);
			}
				break;
			case WeaponTypeID::WHIP:
			{
				Projectile_Whip* ent = new Projectile_Whip();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseAnimator().SetLoop(false);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_3);
			}
				break;
			case WeaponTypeID::DAGGER:
			{
				Projectile_Dagger* ent = new Projectile_Dagger();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_4);
			}
				break;
			case WeaponTypeID::BOMB:
			{
				Projectile_Bomb* ent = new Projectile_Bomb();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_1);
			}
				break;
			case WeaponTypeID::AURA:
			{
				Projectile_Aura* ent = new Projectile_Aura();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_0);
			}
				break;
			case WeaponTypeID::BOOK:
			{
				Projectile_Book* ent = new Projectile_Book();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
				break;
			case WeaponTypeID::AXE:	
			{
				Projectile_Axe* ent = new Projectile_Axe();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
				break;

			case WeaponTypeID::HEX:
			{
				Projectile_HexStep* ent = new Projectile_HexStep();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_FOREGROUND_9);
			}
			break;

			case WeaponTypeID::LIGHTNING:
			{
				Projectile_Lightning* ent = new Projectile_Lightning();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
			break;
			case WeaponTypeID::SUPER:
			{
				Projectile_Super* ent = new Projectile_Super();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
			break;
			case WeaponTypeID::BULLET_HELL_GATE:
			{
				Projectile_BulletHell* ent = new Projectile_BulletHell();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_5);
			}
			break;

			case WeaponTypeID::BULLET_HELL_PROJECTILE:
			{
				Projectile_LightBullet* ent = new Projectile_LightBullet();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
			break;

			case WeaponTypeID::WATER_DRAGON:
			{
				Projectile_WaterDragon* ent = new Projectile_WaterDragon();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
				break;

			case WeaponTypeID::HEAVEN_ON_HIGH_ANGEL:
			{
				Projectile_HoH* ent = new Projectile_HoH();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_9);
			}
			break;

			case WeaponTypeID::HEAVEN_ON_HIGH_PROJECTILE:
			{
				Projectile_HolyBeam* ent = new Projectile_HolyBeam();
				SetupPlayerProjectile(ent, arr, i, count);
				ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_6);
			}
			break;

			}

			++count;
		}
	}

	//Junk allocate for the rest of the pool (safety net to prevent crashes)
	for (unsigned i(count); i < m_PlayerProj00->GetPoolSize(); ++i)
	{
		Projectile_Dagger* ent = new Projectile_Dagger();
		ent->InitPoolData((unsigned short)PoolID::PLAYER_PROJ, (unsigned short)i);
		ent->SetWeaponID((unsigned)WeaponTypeID::UNDEFINED);

		ent->InitialiseVisuals(std::string("Testing_Texture_00"), 5);
		ent->GetBaseSprite().SetScale(3.0f, 0.5f);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_0);

		m_PlayerProj00->GetObjectAtIndex(i) = ent;
	}
}

void SM_EntityManager::InitialiseEnemyProjectiles(rapidjson::Document& doc)
{
	//Each projectile gets a slice of the pool, so define that slice here + keep count of total allocations made
	unsigned enemyProj00 = 1024;
	unsigned enemyProj01 = 0;

	unsigned count = 0;
	unsigned oldCount = 0;

	//Temp Dagger
	msg_assert(count + enemyProj00 <= m_PlayerProj00->GetPoolSize(), "InitialiseEnemyProjectiles(): Allocation of projectiles will overflow!");
	for (unsigned i(0); i < enemyProj00; ++i)
	{
		Projectile_Dagger* ent = new Projectile_Dagger();
		ent->InitPoolData((unsigned short)PoolID::ENEMY_PROJ, (unsigned short)i);
		ent->SetWeaponID(0);

		ent->InitialiseVisuals(std::string("Weapon_SS_00"), (int)Weapon00Desc::BULLET_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_9);
		ent->GetGeneralData().m_CollisionBox =
		{ 
			ent->GetBaseSprite().GetScaleAdjustedRectX(),
			ent->GetBaseSprite().GetScaleAdjustedRectX()
		};
		ent->GetBaseSprite().SetScale(0.75f, 0.75f);
		ent->GetGeneralData().m_Scale = { 0.75f, 0.75f };

		m_EnemyProj00->GetObjectAtIndex(i) = ent;
	}

	msg_assert(enemyProj00 + enemyProj01 <= m_EnemyProj00->GetPoolSize(),
		"InitialiseEnemyProjectiles(): Init total count greater than pool size!");
}
void SM_EntityManager::InitialiseLoot(rapidjson::Document& doc)
{

	//Each loot object gets a slice of the pool, so define that slice here + keep count of total allocations made
	unsigned expAlloc = 512;
	unsigned foodAlloc = 12;
	unsigned magnetAlloc = 6;
	unsigned superAlloc = 6;
	unsigned freezeAlloc = 8;
	unsigned wipeAlloc = 8;
	unsigned chestAlloc = 8;
	unsigned count = 0;
	unsigned oldCount = 0;
	msg_assert(expAlloc + foodAlloc + magnetAlloc + superAlloc + freezeAlloc + wipeAlloc + chestAlloc <= m_Loot->GetPoolSize(),
		"InitialiseLoot(): Init total count greater than pool size!");

	//Experience
	msg_assert(count + expAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	for (unsigned i(0); i < expAlloc; ++i)
	{
		Loot_Experience* ent = new Loot_Experience();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::EXP);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::EXPERIENCE_GEM_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_0);
		ent->GetBaseSprite().SetScale(0.4f, 0.4f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;
	}
	//Food
	msg_assert(count + foodAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + foodAlloc; ++i)
	{
		Loot_Food* ent = new Loot_Food();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::FOOD);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::FOOD_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;
	}
	//Magnet
	msg_assert(count + magnetAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + magnetAlloc; ++i)
	{
		Loot_Magnet* ent = new Loot_Magnet();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::MAGNET);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::MAGNET_ITEM_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;
	}
	//Super
	msg_assert(count + superAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + superAlloc; ++i)
	{
		Loot_Super* ent = new Loot_Super();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::SUPER);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::SUPER_ITEM_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;

	}
	//Freeze
	msg_assert(count + freezeAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + freezeAlloc; ++i)
	{
		Loot_Freeze* ent = new Loot_Freeze();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::FREEZE);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::FOCUS_ICON_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);
		
		m_Loot->GetObjectAtIndex(i) = ent;

		++count;

	}
	//Wipe
	msg_assert(count + wipeAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + wipeAlloc; ++i)
	{
		Loot_Wipe* ent = new Loot_Wipe();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::WIPE);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::NECKLACE_ICON_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;

	}
	//Chest
	msg_assert(count + chestAlloc <= m_Loot->GetPoolSize(), "InitLoot(): Allocation of projectiles will overflow!");
	oldCount = count;
	for (unsigned i(count); i < oldCount + chestAlloc; ++i)
	{
		Loot_Chest* ent = new Loot_Chest();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::CHEST);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::LOOT_CHEST_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_2);
		ent->GetBaseSprite().SetScale(1.0f, 1.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;
	}
	//Junk allocate for the rest of the pool
	oldCount = count;
	for (unsigned i(count); i < m_Loot->GetPoolSize(); ++i)
	{
		Loot_Experience* ent = new Loot_Experience();
		ent->InitPoolData((unsigned short)PoolID::LOOT, (unsigned short)i);
		ent->SetID((unsigned)LootTypeID::EXP);

		ent->InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::EXPERIENCE_GEM_00);
		ent->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_0);
		ent->GetBaseSprite().SetScale(2.0f, 2.f);

		m_Loot->GetObjectAtIndex(i) = ent;

		++count;
	}
}

void SM_EntityManager::InitialiseEffects(rapidjson::Document& doc)
{
	//Just allocate blank effects (the should be setup by the customer entities)
	for (unsigned i(0); i < m_EffectsPool00->GetPoolSize(); ++i)
	{
		Effects_AnimSprite* eff = new Effects_AnimSprite();
		eff->InitPoolData((unsigned short)PoolID::EFFECTS_00, (unsigned short)i);
		eff->GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_9);

		m_EffectsPool00->GetObjectAtIndex(i) = eff;
	}
}
