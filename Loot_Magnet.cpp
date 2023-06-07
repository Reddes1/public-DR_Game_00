#include "Loot_Magnet.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils
void Loot_Magnet::Update(GameTimer& gt)
{

	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));

	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Loot_Magnet::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Magnet::Reset()
{
	SetPassingValue(0);
	SetFollowPlayer(false);
	return true;
}
void Loot_Magnet::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	// Loop Through all active Experience object, enabling follow player
	for (unsigned j(0); j < loot00->GetPoolSize(); j++)
	{
		if (loot00->GetUseFlagAtIndex(j) && loot00->GetObjectAtIndex(j)->GetID() == (unsigned)LootTypeID::EXP)
			loot00->GetObjectAtIndex(j)->SetFollowPlayer(true);
	}
	loot00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	Reset();
}
void Loot_Magnet::Init()
{
	Reset();
}

void Loot_Magnet::Release()
{
	//Release code here (deletes, clears etc)
}

