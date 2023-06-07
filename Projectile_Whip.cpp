#include "Projectile_Whip.h"
#include "CollisionTypes.h"
#include "Game.h" //Manager Access

void Projectile_Whip::Update(GameTimer& gt)
{
	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());

	switch (m_State)
	{
	case StateID::ACTIVE:

		//Update hit list
		UpdateHitList(gt);

		//If at or past a certain frame, disable collision
		if (GetBaseAnimator().GetRelativeCurrentFrame() >= HITBOX_END_FRAME)
		{
			SetCollisionFlag(false);
			m_State = StateID::INACTIVE;
		}
		//Else, update the collision data
		else
		{
			//Update collision for the entity
			SetCollisionData(CollisionArea({
				GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
				GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
				GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
				}));
		}

		break;

	case StateID::INACTIVE:
		//If animation complete, reset and despawn
		if (!GetBaseAnimator().GetPlayState())
		{
			Reset();
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		break;
	}
}

void Projectile_Whip::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Whip::Reset()
{
	GetBaseAnimator().RestartAnimation(true);
	SetCollisionFlag(true);
	m_State = StateID::ACTIVE;

	return true;
}

void Projectile_Whip::Init()
{
	SetCollisionFlag(true);
	m_State = StateID::ACTIVE;
}

void Projectile_Whip::Release()
{

}
