#include "Loot_Wipe.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils


#define GET_ENTITY_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)
#define GET_ENTITY_POOL_OTHER Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITIES_01_02_03)
void Loot_Wipe::Update(GameTimer& gt)
{

	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));

	if (m_Active) {
		GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());
		if (!GetBaseAnimator().GetPlayState()) {	
			InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::NECKLACE_ICON_00);
			GetBaseSprite().SetFrame((int)GameUI00Desc::NECKLACE_ICON_00);
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->SetObjectStatus(GetPoolData().m_PoolIndex, false);
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->GetObjectAtIndex(GetPoolData().m_PoolIndex)->SetCollisionFlag(true);
			Reset();
		}
	}

	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Loot_Wipe::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Wipe::Reset()
{
	SetPassingValue(0);
	SetFollowPlayer(false);
	m_Active = false;
	return true;
}
void Loot_Wipe::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	for (unsigned j(0); j < GET_ENTITY_POOL_00->GetPoolSize(); j++)
	{
		if (GET_ENTITY_POOL_00->GetUseFlagAtIndex(j)) {
			GET_ENTITY_POOL_00->GetObjectAtIndex(j)->ProcessDamageEvent(m_Damage, 0);

			//Call FCT manager and display damage done
			Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
				DirectX::XMFLOAT2(
					GET_ENTITY_POOL_00->GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.posX,
					GET_ENTITY_POOL_00->GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.posY),
				m_Damage,
				-30.f
			);
		}
	}
	for (unsigned j(0); j < GET_ENTITY_POOL_OTHER->GetPoolSize(); j++)
	{
		if (GET_ENTITY_POOL_OTHER->GetUseFlagAtIndex(j)) {
			GET_ENTITY_POOL_OTHER->GetObjectAtIndex(j)->ProcessDamageEvent(m_Damage, 0);

			//Call FCT manager and display damage done
			Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
				DirectX::XMFLOAT2(
					GET_ENTITY_POOL_OTHER->GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.posX,
					GET_ENTITY_POOL_OTHER->GetObjectAtIndex(j)->GetCollisionData().m_CollisonBox.posY),
				m_Damage,
				-30.f
			);
		}
	}

	m_Active = true;
	GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());
	SetCollisionFlag(false);
	GetBaseSprite().SetScale(m_ScaleUp, m_ScaleUp);
	// For Change into Clock Sprite
	InitialiseVisuals(std::string("Effects_SS_00"), (int)Weapon00Desc::BOMB_EXP_00);
	GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);

	// Set Animation Properties
	GetBaseAnimator().SetAnimation((int)Weapon00Desc::BOMB_EXP_00, true, false, false, true);

	//Play Aura weapon effect (for now)
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::AURA, GameVolumes::AMBIENT);

}
void Loot_Wipe::Init()
{
	Reset();
}

void Loot_Wipe::Release()
{
	//Release code here (deletes, clears etc)
}

