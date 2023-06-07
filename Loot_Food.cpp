#include "Loot_Food.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils
void Loot_Food::Update(GameTimer& gt)
{

	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));
	SetPassingValue(m_Health);

	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Loot_Food::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Food::Reset()
{
	SetPassingValue(0);
	SetFollowPlayer(false);
	return true;
}
void Loot_Food::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	//Play Health potion sound
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::HEALTH_POTION, GameVolumes::SFX);

	//Adds the health from the loot object onto the players health
	player->ApplyHealingEffect((short)GetPassingValue(), true);
	loot00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	Reset();
}
void Loot_Food::Init()
{
	Reset();
}

void Loot_Food::Release()
{
	//Release code here (deletes, clears etc)
}

