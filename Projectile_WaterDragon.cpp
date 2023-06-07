#include "Projectile_WaterDragon.h"

#include "AnimationDefinitions.h"
#include "Game.h"	//Manager Access

void Projectile_WaterDragon::Update(GameTimer& gt)
{
	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());
	//Update hit list
	UpdateHitList(gt);

	//Follow the players position
	GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionBox.x * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionBox.y * GetBaseSprite().GetScale().y
		}));
	
	//Update differently based on state
	switch (m_State)
	{
	case StateID::SPAWNING:

		if (!GetBaseAnimator().GetPlayState())
		{
			m_State = StateID::ACTIVE;
			SetCollisionFlag(true);
			GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::WATER_DRAGON_ACTIVE_00, true, true, false, true);
		}
		break;

	case StateID::ACTIVE:

		//Normal update
		if (!(GetStats().m_Lifetime <= 0.0 && !GetBaseAnimator().GetPlayState()))
		{
			//Downtick lifetime
			GetStats().m_Lifetime -= gt.DeltaTime();
		}
		//Trigger despawn
		else
		{
			m_State = StateID::DESPAWNING;
			GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::WATER_DRAGON_DESPAWN_00, true, false, false, true);
		}

		//If lifetime over, flag loop over (so the animation doesn't break, but will bridge to despawn later).
		if (GetStats().m_Lifetime < 0.f)
			GetBaseAnimator().SetLoop(false);
		break;

	case StateID::DESPAWNING:
		
		if (!GetBaseAnimator().GetPlayState())
		{
			Reset();
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		break;
	}
}

void Projectile_WaterDragon::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{	
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_WaterDragon::Reset()
{
	SetCollisionFlag(false);
	m_State = StateID::SPAWNING;
	GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::WATER_DRAGON_SPAWN_00, true, false, false, true);
	return false;
}

void Projectile_WaterDragon::Init()
{

}

void Projectile_WaterDragon::Release()
{

}
