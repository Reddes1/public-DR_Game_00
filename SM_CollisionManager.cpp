#include "SM_CollisionManager.h"
#include "CollisionTypes.h"

#include "GameConstants.h"	//Global Info
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils

using namespace BK_Collision;

#define GET_ENEMY_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)
#define GET_PROJECTILE_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ)

#define GET_PLAYER Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()

void SM_CollisionManager::Init()
{

}

void SM_CollisionManager::Release()
{

}
void SM_CollisionManager::Update(GameTimer& gt)
{
	//Get EM for brevity
	SM_EntityManager* entMgr = &Game::GetGame()->GetGameplayManager().GetEntityManager();
	//Grab each pool/player being tested for collision
	auto enemyPool00 = entMgr->GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00);
	auto enemyPool01 = entMgr->GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITIES_01_02_03);
	auto playerProj = entMgr->GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ);
	auto enemyProj = entMgr->GetProjectilePoolAtID(SM_EntityManager::PoolID::ENEMY_PROJ);
	auto lootPool = entMgr->GetLootPool();
	auto player = entMgr->GetPlayer();

	//Run Collision Checks in Certain Order

	//Enemy Pool Collisions (For Nudging)
	SinglePoolCollision(*enemyPool00);
	SinglePoolCollision(*enemyPool01);
	EP00_EP01_Collision(*enemyPool00, *enemyPool01);

	//Player projectile collision against enemy pools
	PlayerProj_Enemy_Collision(*playerProj, *enemyPool00);
	PlayerProj_Enemy_Collision(*playerProj, *enemyPool01);

	//Player contact collision check against enemy pool memebers
	Player_EP00_Collision(*player, *enemyPool00);
	Player_EP01_Collision(*player, *enemyPool01);

	//Enemy projectile collision check against player
	Player_EnemyProj_Collision(*player, *enemyProj);

	PlayerProj_EnemyProj_Collision(*playerProj, *enemyProj);

	// Player Terrain Collision Check
	Player_Terrain_Collision(*player, Game::GetGame()->GetGameplayManager().GetTileManager().GetActiveMapIndex());

	// Player Loot Collision Check
	Player_Loot_Collision(*player, *lootPool);

	Terrain_Loot_Collision(Game::GetGame()->GetGameplayManager().GetTileManager().GetActiveMapIndex(), *lootPool);
}

bool SM_CollisionManager::SinglePoolCollision(EntityPool<EM_EntityInterface2D*>& pool)
{
	//Cycle through pool, testing for internal collision between entities
	for (unsigned i(0); i < pool.GetPoolSize(); ++i)
	{
		//Check for enabled flag for this entity before moving to test against rest of pool
		if (pool.GetUseFlagAtIndex(i) && pool.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			//Cycle through and check collsion against each other entity
			for (unsigned j(0); j < pool.GetPoolSize(); ++j)
			{
				//Check that this is not the same entity, and that entity is active
				if (i != j && pool.GetUseFlagAtIndex(j) && pool.GetObjectAtIndex(j)->GetCollisionCheck())
				{
					//Run collision check
					if (IsCircleInCircle(pool.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, pool.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
					{
						//Entity 1 nudges Entity 2
						EnemyToEnemyResolution(pool.GetObjectAtIndex(i), pool.GetObjectAtIndex(j));
					}
				}
			}
		}
	}

	return true;
}

bool SM_CollisionManager::EP00_EP01_Collision(EntityPool<EM_EntityInterface2D*>& pool00, EntityPool<EM_EntityInterface2D*>& pool01)
{
	//Cycle through pool, testing for internal collision between entities
	for (unsigned i(0); i < pool00.GetPoolSize(); ++i)
	{
		//Check for enabled flag for this entity before moving to test against rest of pool
		if (pool00.GetUseFlagAtIndex(i) && pool00.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			//Cycle through and check collsion against each other entity
			for (unsigned j(0); j < pool01.GetPoolSize(); ++j)
			{
				//Check that this is not the same entity, and that entity is active
				if (pool01.GetUseFlagAtIndex(j) && pool01.GetObjectAtIndex(j)->GetCollisionCheck())
				{
					//Run collision check
					if (IsCircleInCircle(pool00.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, pool01.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
					{
						//Entity 1 nudges Entity 2
						EnemyToEnemyResolution(pool00.GetObjectAtIndex(i), pool01.GetObjectAtIndex(j));
					}
				}
			}
		}
	}

	return true;
}

bool SM_CollisionManager::PlayerProj_Enemy_Collision(EntityPool<Projectile_Interface*>& proj00, EntityPool<EM_EntityInterface2D*>& pool00)
{
	//Cycle through projectile pool and test for collision against enemypool 00
	for (unsigned i(0); i < proj00.GetPoolSize(); ++i)
	{
		//Check for enabled flag for this entity before moving to test against rest of pool
		if (proj00.GetUseFlagAtIndex(i) && proj00.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			//Cycle through and check collsion against each other entity
			for (unsigned j(0); j < pool00.GetPoolSize(); ++j)
			{
				//Check that this is not the same entity, and that entity is active
				if (pool00.GetUseFlagAtIndex(j) && pool00.GetObjectAtIndex(j)->GetCollisionCheck())
				{
					//Run collision check based on projectile type
					if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::CIRCLE)					{
						if (IsCircleInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), pool00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
							PlayerProjToEnemyResolution(proj00.GetObjectAtIndex(i), pool00.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::OVAL) {
						if (IsOvalInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), pool00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
							PlayerProjToEnemyResolution(proj00.GetObjectAtIndex(i), pool00.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::RECT)
					{
						if (IsRectInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), pool00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
							PlayerProjToEnemyResolution(proj00.GetObjectAtIndex(i), pool00.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::NON_ALIGNED_RECT)
					{
						// TEST
						if (IsNonAxisRectInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), proj00.GetObjectAtIndex(i)->GetBaseSprite().GetRotation(), pool00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
							PlayerProjToEnemyResolution(proj00.GetObjectAtIndex(i), pool00.GetObjectAtIndex(j));
					}
				}
			}
		}
	}

	return true;
}

bool SM_CollisionManager::Player_EP00_Collision(Entity_Player& player, EntityPool<EM_EntityInterface2D*>& pool)
{
	for (unsigned i(0); i < pool.GetPoolSize(); ++i)
	{
		//If the entity is active, and it can attack 
		if (pool.GetUseFlagAtIndex(i) && pool.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			//Now check for collision
			if (IsCircleInCircle(pool.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, player.GetCollisionData().m_CollisonBox))
			{
				EnemyToPlayerResolution(pool.GetObjectAtIndex(i), &player);
			}
		}
	}

	return true;
}

bool SM_CollisionManager::Player_EP01_Collision(Entity_Player& player, EntityPool<EM_EntityInterface2D*>& pool)
{
	for (unsigned i(0); i < pool.GetPoolSize(); ++i)
	{
		//If the entity is active, and it can attack 
		if (pool.GetUseFlagAtIndex(i) && pool.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			//Now check for collision
			if (IsCircleInCircle(pool.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, player.GetCollisionData().m_CollisonBox))
			{
				EnemyToPlayerResolution(pool.GetObjectAtIndex(i), &player);
			}
		}
	}

	return true;
}

bool SM_CollisionManager::PlayerProj_EnemyProj_Collision(EntityPool<Projectile_Interface*>& proj00, EntityPool<Projectile_Interface*>& proj01)
{
	for (unsigned i(0); i < proj00.GetPoolSize(); ++i)
	{
		//If the entity is active, and it can attack 
		if (proj00.GetUseFlagAtIndex(i) && proj00.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			for (unsigned j(0); j < proj01.GetPoolSize(); ++j)
			{
				//If the entity is active, and it can attack 
				if (proj01.GetUseFlagAtIndex(j) && proj01.GetObjectAtIndex(i)->GetCollisionCheck())
				{
					//Run collision check based on projectile type
					if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::CIRCLE) {
						if (IsCircleInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), proj01.GetObjectAtIndex(j)->GetCollisionData()))
							PlayerProjToEnemyProjResolution(proj00.GetObjectAtIndex(i), proj01.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::OVAL) {
						if (IsOvalInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), proj01.GetObjectAtIndex(j)->GetCollisionData()))
							PlayerProjToEnemyProjResolution(proj00.GetObjectAtIndex(i), proj01.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::RECT)
					{
						if (IsRectInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), proj01.GetObjectAtIndex(j)->GetCollisionData()))
							PlayerProjToEnemyProjResolution(proj00.GetObjectAtIndex(i), proj01.GetObjectAtIndex(j));
					}
					else if (proj00.GetObjectAtIndex(i)->GetGeneralData().m_CollisionID == (unsigned)CollisionTypeID::NON_ALIGNED_RECT)
					{
						// TEST
						if (IsNonAxisRectInCircle(proj00.GetObjectAtIndex(i)->GetCollisionData(), proj00.GetObjectAtIndex(i)->GetBaseSprite().GetRotation(), proj01.GetObjectAtIndex(j)->GetCollisionData()))
							PlayerProjToEnemyProjResolution(proj00.GetObjectAtIndex(i), proj01.GetObjectAtIndex(j));
					}
				}
			}
		}
	}
	return true;
}

bool SM_CollisionManager::Player_EnemyProj_Collision(Entity_Player& player, EntityPool<Projectile_Interface*>& proj00)
{
	for (unsigned i(0); i < proj00.GetPoolSize(); ++i)
	{
		//If the entity is active, and it can attack 
		if (proj00.GetUseFlagAtIndex(i) && proj00.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			if (IsCircleInCircle(player.GetCollisionData().m_CollisonBox, proj00.GetObjectAtIndex(i)->GetCollisionData()))
				PlayerToEnemyProjResolution(&player, proj00.GetObjectAtIndex(i));
		}
	}
	return true;
}

bool SM_CollisionManager::Player_Terrain_Collision(Entity_Player& player, unsigned terrainIndex)
{

	for (unsigned i(0); i < Game::GetGame()->GetGameplayManager().GetDataManager().GetCollisionMap(terrainIndex)->size(); ++i)
	{
		//Now check for collision
		if (IsRectInCircle(Game::GetGame()->GetGameplayManager().GetDataManager().GetObjectCollisionAtID(terrainIndex, i)->m_CollisonBox, player.GetCollisionData().m_CollisonBox))
		{
			PlayerToTerrainResolution(Game::GetGame()->GetGameplayManager().GetDataManager().GetObjectCollisionAtID(terrainIndex, i)->m_CollisonBox, &player);

		}

	}

	return true;
}

bool SM_CollisionManager::Player_Loot_Collision(Entity_Player& player, EntityPool<Loot_Interface*>& loot00)
{
	for (unsigned i(0); i < loot00.GetPoolSize(); i++)
	{
		//If the entity is active, and it can attack 
		if (loot00.GetUseFlagAtIndex(i) && loot00.GetObjectAtIndex(i)->GetCollisionCheck())
		{
			// Check if loot is in magnet range
			CollisionArea playerPickUp = player.GetCollisionData().m_CollisonBox;
			playerPickUp.width += player.GetPlayerStats().m_PickupRange + player.GetPlayerStats().m_PickupRangeMod;
			if (loot00.GetObjectAtIndex(i)->GetID() == (unsigned)LootTypeID::EXP && IsCircleInCircle(loot00.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, playerPickUp)) {
				
				loot00.GetObjectAtIndex(i)->SetFollowPlayer(true);
			}
			// Check if loot is in pick up range
			if (IsCircleInCircle(loot00.GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox, player.GetCollisionData().m_CollisonBox))
			{
				loot00.GetObjectAtIndex(i)->ProcessCollisionEvent(&player, &loot00);

			}
		}
	}

	return true;
}
bool SM_CollisionManager::Terrain_Loot_Collision(unsigned terrainIndex, EntityPool<Loot_Interface*>& loot00)
{
	for (unsigned i(0); i < Game::GetGame()->GetGameplayManager().GetDataManager().GetCollisionMap(terrainIndex)->size(); ++i)
	{
		for (unsigned j(0); j < loot00.GetPoolSize(); j++)
		{
			//If the entity is active, and it can attack 
			if (loot00.GetUseFlagAtIndex(j) && loot00.GetObjectAtIndex(j)->GetCollisionCheck() && (loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonLayer != CollisionLayers::PLAYER_ONLY_COLLISION))
			{
				// Checks untill the loot has moved outside the terrain area
				if (IsRectInCircle(Game::GetGame()->GetGameplayManager().GetDataManager().GetObjectCollisionAtID(terrainIndex, i)->m_CollisonBox, loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox))
				{
					CollisionArea terrain = Game::GetGame()->GetGameplayManager().GetDataManager().GetObjectCollisionAtID(terrainIndex, i)->m_CollisonBox;
					DirectX::SimpleMath::Vector2 player = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();
					
					// Edge Check

					if (terrain.posX + (terrain.width/2) <= player.x) // Check if Right Bound
						loot00.GetObjectAtIndex(j)->GetBaseSprite().SetPositionX(terrain.posX + (terrain.width/2) + loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.width);
					else if (terrain.posX - (terrain.width/2) >= player.x) // Check if Left Bound
						loot00.GetObjectAtIndex(j)->GetBaseSprite().SetPositionX(terrain.posX - (terrain.width/2) - loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.width);

					else if (terrain.posY + (terrain.height/2) <= player.y) // Check if Bottom Bound
						loot00.GetObjectAtIndex(j)->GetBaseSprite().SetPositionY(terrain.posY + (terrain.height/2) + loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.height);
					else if (terrain.posY - (terrain.height/2) >= player.y) // Check if Top Bound
						loot00.GetObjectAtIndex(j)->GetBaseSprite().SetPositionY(terrain.posY - (terrain.height/2) - loot00.GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.height);
				}
			}
		}
	}


	return true;
}

void SM_CollisionManager::EnemyToEnemyResolution(EM_EntityInterface2D* ent00, EM_EntityInterface2D* ent01)
{
	//Apply nudging effect to ent01
	if (ent00->GetCollisionData().m_CollisonLayer > ent01->GetCollisionData().m_CollisonLayer)
	{
		ent01->SetNudge(true);
		ent01->SetNudgeDirection(DirectX::XMFLOAT2(
			ent01->GetCollisionData().m_CollisonBox.posX - ent00->GetCollisionData().m_CollisonBox.posX,
			ent01->GetCollisionData().m_CollisonBox.posY - ent00->GetCollisionData().m_CollisonBox.posY
		));
	}
	else {
		ent00->SetNudge(true);
		ent00->SetNudgeDirection(DirectX::XMFLOAT2(
			ent00->GetCollisionData().m_CollisonBox.posX - ent01->GetCollisionData().m_CollisonBox.posX,
			ent00->GetCollisionData().m_CollisonBox.posY - ent01->GetCollisionData().m_CollisonBox.posY
		));
	}
}

void SM_CollisionManager::EnemyToPlayerResolution(EM_EntityInterface2D* ent00, Entity_Player* player)
{
	if (ent00->CanCollisionDamageOccur())
	{
		//Enemy damages the player
		player->ProcessDamageEvent(ent00->GetDamageValue(), 0);
		//Reset attacker timer
		ent00->ResetCollisionTimer();
	}

	//Set entity to nudge away from player
	ent00->SetNudge(true);
	ent00->SetNudgeDirection(DirectX::XMFLOAT2(
		ent00->GetCollisionData().m_CollisonBox.posX - player->GetCollisionData().m_CollisonBox.posX,
		ent00->GetCollisionData().m_CollisonBox.posY - player->GetCollisionData().m_CollisonBox.posY
	));
}

void SM_CollisionManager::PlayerProjToEnemyProjResolution(Projectile_Interface* proj00, Projectile_Interface* proj01)
{
	// SET TO DO NOTHING CURRENTLY
}

void SM_CollisionManager::PlayerToEnemyProjResolution(Entity_Player* player, Projectile_Interface* proj)
{
	player->ProcessDamageEvent(proj->GetStats().m_Damage, 0);
	Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID((SM_EntityManager::PoolID)proj->GetPoolData().m_PoolID)->SetObjectStatus(proj->GetPoolData().m_PoolIndex, false);
}

void SM_CollisionManager::PlayerProjToEnemyResolution(Projectile_Interface* proj, EM_EntityInterface2D* ent00)
{
	//Pass entity to projectile, and process the follow events if entity is valid hit
	if (!proj->IsEntityInHitList(ent00))
	{
		//Apply damage from projectile to enemy
		ent00->ProcessDamageEvent(proj->GetStats().m_Damage, proj->GetStats().m_KnockbackEffect);
		//Call projectiles post collision function (may have limited hits, special functionality)
		proj->ProcessCollisionEvent(ent00);

		//Call FCT manager and display damage done
		Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
			DirectX::XMFLOAT2(
				ent00->GetCollisionData().m_CollisonBox.posX,
				ent00->GetCollisionData().m_CollisonBox.posY),
			proj->GetStats().m_Damage,
			-30.f
		);
	}
}
void SM_CollisionManager::PlayerToTerrainResolution(CollisionArea terrain, Entity_Player* player)
{
	CollisionArea playerColl = player->GetCollisionData().m_CollisonBox;
	auto& camera = Game::GetGame()->GetGameplayManager().GetCamera();
	if (playerColl.posX - playerColl.width< (terrain.posX + (terrain.width / 2)) - 10.f && playerColl.posX + playerColl.width >(terrain.posX - (terrain.width / 2)) + 10.f)
	{
		if (playerColl.posY + playerColl.width > terrain.posY - (terrain.height / 2) && !(playerColl.posY + playerColl.width > terrain.posY)) // Top Check
		{
			if (player->GetVelocity().y > 0.f)
				player->SetCollisionDirectionY(false);
			else
				player->SetCollisionDirectionY(true);
		}
		else if (playerColl.posY - playerColl.width < terrain.posY + (terrain.height / 2)) // Bottom Check
		{
			if (player->GetVelocity().y < 0.f)
				player->SetCollisionDirectionY(false);
			else
				player->SetCollisionDirectionY(true);
		}
	}
	else
	{
		if (playerColl.posX + playerColl.width < terrain.posX + (terrain.width / 2)) // LEFT Check
		{
			if (player->GetVelocity().x > 0.f)
				player->SetCollisionDirectionX(false);
			else
				player->SetCollisionDirectionX(true);
		}
		else if (playerColl.posX - playerColl.width < terrain.posX + (terrain.width / 2)) // RIGHT Check
		{
			if (player->GetVelocity().x < 0.f)
				player->SetCollisionDirectionX(false);
			else
				player->SetCollisionDirectionX(true);
		}

	}
}