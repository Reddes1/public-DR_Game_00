#include "Projectile_Book.h"

#include "Game.h"	//Manager Access
#include "MathHelper.h" //Lerp Methods

#define EXPAND_TIME 0.75f
#define CONTRACT_TIME 0.5f

void Projectile_Book::Update(GameTimer& gt)
{
	//Pre-Amble to update based on state/stage of weapon
	switch (m_State)
	{
	case StateID::EXPAND:

		//Interpolate position multiplier from 0-1
		m_ElapsedInter += gt.DeltaTime();
		m_ElapsedInter = MathHelper::Clamp(m_ElapsedInter, 0.f, 1.f);
		m_PosMul = MathHelper::Lerp(0.f, 1.f, m_ElapsedInter / EXPAND_TIME);

		//When position multiplier done, switch to stable
		if (m_PosMul >= 1.f)
		{
			m_PosMul = 1.f;
			m_State = StateID::STABLE;
			m_ElapsedInter = 0.f;
		}		
		break;

	case StateID::STABLE:

		//Change from this stage to final stage if lifetime done
		if (GetStats().m_Lifetime <= 0.f)
			m_State = StateID::CONTRACT;
		break;

	case StateID::CONTRACT:

		//Interpolate position multiplier from 1-0
		m_ElapsedInter += gt.DeltaTime();
		m_ElapsedInter = MathHelper::Clamp(m_ElapsedInter, 0.f, 1.f);
		m_PosMul = MathHelper::Lerp(1.f, 0.f, m_ElapsedInter / CONTRACT_TIME);

		break;
	}

	//Get player position
	DirectX::XMFLOAT2 pos = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();

	//Set position
	GetBaseSprite().SetPosition(
		std::sinf(m_Angle) * (m_OffsetX * m_PosMul) + pos.x,
		std::cosf(m_Angle) * (m_OffsetY * m_PosMul) + pos.y
	);

	//Update angle
	m_Angle += (DegreesToRadian(GetStats().m_Speed) * gt.DeltaTime());

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().y
		}));

	//Update hit list
	UpdateHitList(gt);

	//Downtick lifetime
	GetStats().m_Lifetime -= gt.DeltaTime();
	//When projectile is completely contracted, free the object
	if (GetStats().m_Lifetime <= 0.0f && m_PosMul <= 0.f)
	{
		Reset();
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
			(SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	}
}

void Projectile_Book::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Book::Reset()
{
	m_ElapsedInter = 0.f;
	m_PosMul = 0.f;
	m_State = StateID::EXPAND;
	return false;
}

bool Projectile_Book::ProcessCollisionEvent(EM_EntityInterface2D* const ent)
{
	return false;
}

bool Projectile_Book::ProcessCollisionEvent(Projectile_Interface* const proj)
{
	return false;
}

void Projectile_Book::SetFunctionalityData(float angle, float offsetX, float offsetY)
{
	m_Angle = angle;
	m_OffsetX = offsetX;
	m_OffsetY = offsetY;
}

void Projectile_Book::Init()
{

}

void Projectile_Book::Release()
{

}
