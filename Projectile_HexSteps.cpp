#include "Projectile_HexSteps.h"
#include "Game.h"


void Projectile_HexStep::Update(GameTimer& gt)
{
	//Update hit list
	UpdateHitList(gt);
	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());

	//Update differently based on state
	switch (m_State)
	{
	case StateID::SPAWNING:

		//Wait till spawning animation complete
		if (!GetBaseAnimator().GetPlayState())
		{
			//Play Hex Weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::HEX, GameVolumes::SFX);
			m_State = StateID::ACTIVE;
			GetBaseAnimator().SetAnimation((int)Weapon00Desc::HEX_ACTIVE, true, true, false, true);
			SetCollisionFlag(true);
		}
		break;

	case StateID::ACTIVE:

		//Downtick lifetime
		GetStats().m_Lifetime -= gt.DeltaTime();

		//While still active
		if (GetStats().m_Lifetime > 0.f)
		{
			//Update collision for the entity
			SetCollisionData(CollisionArea({
				GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().x,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().y
				}));
		}
		//Lifetime over, start despawning
		else
		{
			m_State = StateID::DESPAWNING;
			SetCollisionFlag(false);
			GetBaseAnimator().SetAnimation((int)Weapon00Desc::HEX_DESPAWN, true, false, false, true);
		}

		break;

	case StateID::DESPAWNING:

		//Wait till despawning animation complete before releasing
		if (!GetBaseAnimator().GetPlayState())
		{
			Reset();

			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				(SM_EntityManager::PoolID)GetPoolData().m_PoolID)->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		break;
	}

}

void Projectile_HexStep::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_HexStep::Reset()
{
	m_State = StateID::SPAWNING;
	GetBaseAnimator().SetAnimation((int)Weapon00Desc::HEX_SPAWN, true, false, false, true);
	SetCollisionFlag(false);

	return false;
}

void Projectile_HexStep::Init()
{
}

void Projectile_HexStep::Release()
{
}
