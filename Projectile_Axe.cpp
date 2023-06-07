#include "Projectile_Axe.h"
#include "Game.h"

void Projectile_Axe::Update(GameTimer& gt)
{
	//Run once flag check
	if (!m_VelCaptured)
	{
		m_VelYStart = GetVelocity().y;
		m_VelCaptured = true;
	}		

	//Update Rotation (using fraction of current speed)
	if(GetVelocity().x > 0.f)
		GetBaseSprite().AddToRotation((GetStats().m_Speed * 0.008f) * gt.DeltaTime());
	else
		GetBaseSprite().AddToRotation(-(GetStats().m_Speed * 0.008f) * gt.DeltaTime());

	switch (m_State)
	{
	case StateID::ACCELERATING:
	{
		//Update Y velocity
		m_Elapsed += gt.DeltaTime();
		GetVelocity().y = MathHelper::Lerp(m_VelYStart, 1.f, m_Elapsed / 1.75f);
		GetVelocity().y = MathHelper::Clamp(GetVelocity().y, m_VelYStart, 1.f);

		if (GetVelocity().y >= 1.f)
			m_State = StateID::FULL_SPEED;

		//Calculate new movement and apply
		GetBaseSprite().AddToPosition(
			(GetStats().m_Speed * GetVelocity().x) * gt.DeltaTime(),
			(GetStats().m_Speed * GetVelocity().y) * gt.DeltaTime()
		);
	}
	break;

	case StateID::FULL_SPEED:
	{
		//Calculate new movement and apply
		GetBaseSprite().AddToPosition(
			(GetStats().m_Speed * GetVelocity().x) * gt.DeltaTime(),
			(GetStats().m_Speed * GetVelocity().y) * gt.DeltaTime()
		);

		//Get some values for brevity
		Game* game = Game::GetGame();
		float winX = game->GetWindowWidthF();
		float winY = game->GetWindowHeightF();
		auto pos = GetBaseSprite().GetPosition();

		//Check if offscreen, resetting and disabling if true
		if ((pos.x + ENTITY_SCREEN_OFFSET_MAX < 0.f - game->GetGameplayManager().GetCamera().GetSceneTranslation().x)  ||	//Left
			(pos.x - ENTITY_SCREEN_OFFSET_MAX > winX - game->GetGameplayManager().GetCamera().GetSceneTranslation().x) ||	//Right
			(pos.y - ENTITY_SCREEN_OFFSET_MAX > winY - game->GetGameplayManager().GetCamera().GetSceneTranslation().y))		//Bottom
		{
			//Reset on the way out
			Reset();
			//Disable the projectile by using pool data to flag it free
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID((SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
	}		
	break;
	}

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().y
		}));

}

void Projectile_Axe::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Axe::Reset()
{
	m_State = StateID::ACCELERATING;
	m_VelCaptured = false;
	m_Elapsed = 0.f;
	SetCollisionFlag(true);

	return true;
}

bool Projectile_Axe::ProcessCollisionEvent(EM_EntityInterface2D* const ent)
{
	//Subtract current collision count and disable if out of collisions
	if (--GetStats().m_CollisionsRemaining <= 0)
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID((SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	return false;
}

bool Projectile_Axe::ProcessCollisionEvent(Projectile_Interface* const proj)
{
	return false;
}

void Projectile_Axe::Init()
{
	Reset();
}

void Projectile_Axe::Release()
{
}
