#include "Projectile_HolyBeam.h"

#include "AnimationDefinitions.h"
#include "Game.h"	//Manager Access

void Projectile_HolyBeam::Update(GameTimer& gt)
{
	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());
	//Update hit list
	UpdateHitList(gt);

	//If at or past a certrain frame, disable collision
	if (GetBaseAnimator().GetRelativeCurrentFrame() >= HITBOX_END_FRAME_DESPAWN)
		SetCollisionFlag(false);
	//Else, update collision values
	else
	{
		//Update collision for the entity
		SetCollisionData(CollisionArea({
			GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
			GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
			GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
			}));
	}

	//If animation done, begin despawn
	if (!GetBaseAnimator().GetPlayState())
	{
		Reset();
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
			SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	}
}

void Projectile_HolyBeam::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_HolyBeam::Reset()
{
	GetBaseAnimator().SetLoop(false);
	GetBaseAnimator().RestartAnimation(true);
	SetCollisionFlag(true);

	return true;
}

bool Projectile_HolyBeam::ProcessCollisionEvent(EM_EntityInterface2D* const ent)
{
	return false;
}

bool Projectile_HolyBeam::ProcessCollisionEvent(Projectile_Interface* const proj)
{
	return false;
}

void Projectile_HolyBeam::Init()
{
}

void Projectile_HolyBeam::Release()
{
}
