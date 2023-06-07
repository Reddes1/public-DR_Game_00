#include "Projectile_Super.h"
#include "Game.h"	//Manager Access

void Projectile_Super::Update(GameTimer& gt)
{
	//Downtick lifetime
	GetStats().m_Lifetime -= gt.DeltaTime();

	//Add to position
	GetBaseSprite().AddToPosition(
		(GetStats().m_Speed * GetVelocity().x) * gt.DeltaTime(),
		(GetStats().m_Speed * GetVelocity().y) * gt.DeltaTime()
	);

	//Update certain areas if lifetime good
	if (GetStats().m_Lifetime > 0.f)
	{
		//Update collision for the entity
		SetCollisionData(CollisionArea({
			GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
			GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
			GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
			}));

		//Update hit list
		UpdateHitList(gt);
		//Update animation
		GetBaseAnimator().Update(gt.DeltaTime());
	}
	//Else, release projectile
	else
	{
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID((SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		Reset();
	}
}

void Projectile_Super::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Super::Reset()
{		
	return false;
}

bool Projectile_Super::ProcessCollisionEvent(EM_EntityInterface2D* const ent)
{
	return false;
}

bool Projectile_Super::ProcessCollisionEvent(Projectile_Interface* const proj)
{
	return false;
}

void Projectile_Super::Init()
{
	Reset();
	SetCollisionFlag(true);
}

void Projectile_Super::Release()
{
}
