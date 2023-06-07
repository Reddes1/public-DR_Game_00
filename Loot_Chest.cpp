#include "Loot_Chest.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils
void Loot_Chest::Update(GameTimer& gt)
{
	//Based on state, operate differently

	switch (m_State)
	{
	case StateID::STABLE:
		//Update collision data
		SetCollisionData(CollisionArea({
			GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
			GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
			}));

		break;

	case StateID::DESPAWNING:

		//Downtick elapsed, and apply it to alpha
		m_Elapsed -= gt.DeltaTime();
		if (m_Elapsed > 0.f)
		{
			DirectX::SimpleMath::Vector4 colour = DirectX::Colors::White;
			colour.w = m_Elapsed;
			GetBaseSprite().SetColour(colour);
		}
		//Disable when invisible
		else
		{
			Reset();
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}

		break;
	}
}

void Loot_Chest::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Chest::Reset() 
{
	m_State = StateID::STABLE;
	m_Elapsed = 1.f;
	
	return true;
}
void Loot_Chest::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	// CHeck for available upgrades

	//If an upgrade found, change game state
	if (Game::GetGame()->GetGameplayManager().GetProgressManager().ApplyChestUpgrade())
	{
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::LEVEL_SELECTION_OPEN);
		//Play audio
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::CHEST, GameVolumes::SFX);
	}
	//Else, heal the player an amount
	else 
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->ApplyHealingEffect(HEAL_AMOUNT, true);

	//Free from pool
	loot00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	Reset();
}
void Loot_Chest::Init()
{
	Reset();
}

void Loot_Chest::Release()
{
	//Release code here (deletes, clears etc)
}

