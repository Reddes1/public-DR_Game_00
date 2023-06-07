#include "SM_LootManager.h"

#include "GameConstants.h"	//Global Info
#include "Game.h"			//Manager Access

#include "EM_EntityInterface.h"

//Loot Types
#include "Loot_Experience.h"
#include "Loot_Food.h"
#include "Loot_Magnet.h"
#include "Loot_Chest.h"
#include "Loot_Super.h"
#include "Loot_Freeze.h"
#include "Loot_Wipe.h"


void SM_LootManager::Init()
{

}

void SM_LootManager::Release()
{

}
void SM_LootManager::ProcessNewLootEvent(EM_EntityInterface2D* const ent)
{
	//Get entity manager
	SM_EntityManager& eMgr = Game::GetGame()->GetGameplayManager().GetEntityManager();

	//Add to kill count
	Game::GetGame()->GetGameplayManager().GetProgressManager().IncrementKillCount();

	//
	//XP
	//

	//Try to find a free XP gem
	Loot_Interface* xpGem = nullptr;
	if(ent->GetEnemyStats().m_XPValue > 0)
		xpGem = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::EXP);
	//If gem found
	if (xpGem != nullptr)
	{
		//Init position, scale and collision data
		xpGem->InitialisePosition(DirectX::XMFLOAT2(ent->GetCollisionData().m_CollisonBox.posX, ent->GetCollisionData().m_CollisonBox.posY));
		xpGem->GetBaseSprite().SetScale(0.40f, 0.40f);

		xpGem->SetCollisionData(CollisionArea{
			xpGem->GetBaseSprite().GetPosition().x,
			xpGem->GetBaseSprite().GetPosition().y,
			xpGem->GetBaseSprite().GetScaleAdjustedRectX(),
			xpGem->GetBaseSprite().GetActualRectY()
			});

		//Set XP
		xpGem->SetPassingValue(ent->GetEnemyStats().m_XPValue);
	}
	//No free gem found, so insert experience into the first indexed gem
	else
		eMgr.GetLootPool()->GetObjectAtIndex(0)->SetPassingValue(eMgr.GetLootPool()->GetObjectAtIndex(0)->GetPassingValue() + ent->GetEnemyStats().m_XPValue);

	//
	//Extra Loot
	//

	//Create holding container
	Loot_Interface* drop = nullptr;
	
	//If the entity already has a loot flag enabled, use that over the following
	if (ent->GetFlags().m_DropsChest)
		drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::CHEST);
	else if (ent->GetFlags().m_ForcedItemDrop)
	{
		int dropType = rand() % 100;
		//Based on value, drop a different item
		if (dropType >= 50) // 34%
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::FOOD);
		else if (dropType >= 25) // 33%
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::MAGNET);
		else if (dropType >= 0) // 33%
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::SUPER);
	}
	else
	{
		//Generates a random value between 0-5000
		int dropType = rand() % 5000;
		//Based on value, determine what to drop, if anything
		if (dropType >= 14) //Do Nothing
		{}
		else if (dropType >= 6) // 1:625  (8)
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::FOOD);
		else if (dropType >= 2)  // 1:1250 (4)
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::MAGNET);
		else if (dropType >= 0)  // 1:2500 (2)
			drop = eMgr.GetLootPool()->FindFreeObjectByInternalTypeID((unsigned)LootTypeID::SUPER);
	}
	
	//If any loot was found, setup
	if (drop != nullptr)
	{
		//Init position, scale and collision data
		drop->InitialisePosition(DirectX::XMFLOAT2(
			ent->GetCollisionData().m_CollisonBox.posX + MathHelper::RandF(-20.f, 20.f),
			ent->GetCollisionData().m_CollisonBox.posY + MathHelper::RandF(-20.f, 20.f)
		));
		drop->GetBaseSprite().SetScale(0.55f, 0.55f);

		drop->SetCollisionData(CollisionArea{
			drop->GetBaseSprite().GetPosition().x,
			drop->GetBaseSprite().GetPosition().y,
			drop->GetBaseSprite().GetScaleAdjustedRectX(),
			drop->GetBaseSprite().GetScaleAdjustedRectY()
			});
	}

	//Free the entity
	eMgr.GetEntityPoolAtID(
		(SM_EntityManager::PoolID)ent->GetPoolData().m_PoolID)->SetObjectStatus(ent->GetPoolData().m_PoolIndex,
		false
	);
}