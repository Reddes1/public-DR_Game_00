#include "Projectile_LightBullet.h"
#include "Game.h"	//Manager Access

void Projectile_LightBullet::Update(GameTimer& gt)
{
	//Update hit list
	UpdateHitList(gt);

	//Update Animator
	GetBaseAnimator().Update(gt.DeltaTime());

	//Calculate and add to position
	GetBaseSprite().AddToPosition(
		(GetStats().m_Speed * GetVelocity().x) * gt.DeltaTime(),
		(GetStats().m_Speed * GetVelocity().y) * gt.DeltaTime()
	);

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
		}));

	//Check against screen edges (with adjustments for camera)
	auto& pos = GetBaseSprite().GetPosition();
	//Get Game for brevity
	Game* game = Game::GetGame();
	//Get window dims
	float winX = game->GetWindowWidthF();
	float winY = game->GetWindowHeightF();
	if ((pos.x < 0.f - game->GetGameplayManager().GetCamera().GetSceneTranslation().x) ||	//Left
		(pos.y < 0.f - game->GetGameplayManager().GetCamera().GetSceneTranslation().y) ||	//Top
		(pos.x > winX - game->GetGameplayManager().GetCamera().GetSceneTranslation().x) ||	//Right
		(pos.y > winY - game->GetGameplayManager().GetCamera().GetSceneTranslation().y))	//Bottom
	{
		//Disable the projectile by using pool data to flag it free
		game->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
			(SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		//Reset on the way out
		Reset();
	}
}

void Projectile_LightBullet::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_LightBullet::Reset()
{
	GetBaseAnimator().RestartAnimation(true);
	GetBaseAnimator().SetLoop(false);
	return false;
}

bool Projectile_LightBullet::ProcessCollisionEvent(EM_EntityInterface2D* const ent)
{
	return false;
}

bool Projectile_LightBullet::ProcessCollisionEvent(Projectile_Interface* const proj)
{
	//Subtract current collision count and disable if out of collisions
	if (--GetStats().m_CollisionsRemaining <= 0)
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID((SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	return false;
}

void Projectile_LightBullet::Init()
{
	SetCollisionFlag(true);
}

void Projectile_LightBullet::Release()
{
}
