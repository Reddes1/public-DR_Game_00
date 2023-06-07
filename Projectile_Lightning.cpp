#include "Projectile_Lightning.h"
#include "Game.h"			//Manager Access
#include "CollisionTypes.h"

#define GET_ENEMY_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)

void Projectile_Lightning::Update(GameTimer& gt)
{

	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());
	//Update hit list
	UpdateHitList(gt);

	//If animation done, begin despawn
	if (!GetBaseAnimator().GetPlayState())
	{
		Reset();
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
			SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	}

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
		}));
}

void Projectile_Lightning::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Lightning::Reset()
{
	GetBaseAnimator().SetLoop(false);
	GetBaseAnimator().RestartAnimation(true);
	SetCollisionFlag(true);
	return true;
}

void Projectile_Lightning::Init()
{
}

void Projectile_Lightning::Release()
{
	//Release code here (deletes, clears etc)
}

