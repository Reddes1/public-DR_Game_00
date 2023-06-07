#include "Loot_Freeze.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils

void Loot_Freeze::Update(GameTimer& gt)
{

	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));

	if (m_Active) {
		GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());
		if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep()) {
			InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::FOCUS_ICON_00);
			GetBaseSprite().SetFrame((int)GameUI00Desc::FOCUS_ICON_00);
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->SetObjectStatus(GetPoolData().m_PoolIndex, false);
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->GetObjectAtIndex(GetPoolData().m_PoolIndex)->SetCollisionFlag(true);
			Reset();
		}
	}
	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Loot_Freeze::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Freeze::Reset()
{
	SetPassingValue(0);
	SetFollowPlayer(false);
	m_Active = false;
	return true;
}
void Loot_Freeze::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	Game::GetGame()->GetGameplayManager().GetEntityManager().SetSleep(true);
	m_Active = true;
	GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());
	SetCollisionFlag(false);
	GetBaseSprite().SetScale(m_ScaleUp, m_ScaleUp);

	// For Change into Clock Sprite
	InitialiseVisuals(std::string("UI_00"), (int)GameUI00Desc::ARMOUR_ICON_00);
	GetBaseSprite().SetLayerDepth(DRAW_DEPTH_ENTITY_1);

	// Set Animation Properties
	//GetBaseAnimator().SetAnimation((int)Weapon00Desc::BOMB_EXP_00, true, false, false, true);

	//Play Aura weapon effect (for now)
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::AURA, GameVolumes::AMBIENT);
}
void Loot_Freeze::Init()
{
	Reset();
}

void Loot_Freeze::Release()
{
	//Release code here (deletes, clears etc)
}

