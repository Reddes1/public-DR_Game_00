#include "Projectile_BulletHell.h"

#include "AnimationDefinitions.h"
#include "Game.h"	//Manager Access

void Projectile_BulletHell::Update(GameTimer& gt)
{
	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());

	//Get player position and set equal to + offsets
	DirectX::XMFLOAT2 pos = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();
	GetBaseSprite().SetPosition(
		pos.x + m_OffsetX,
		pos.y + m_OffsetY
	);

	//Update differently based on state
	switch (m_State)
	{
	case StateID::SPAWNING:

		//Wait for spawn animation to finish before firing projectiles
		if (!GetBaseAnimator().GetPlayState())
		{
			m_State = StateID::ACTIVE;
			GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::GATE_IDLE_00, true, true, false, true);
		}
		break;

	case StateID::ACTIVE:

		m_Elapsed += gt.DeltaTime();
		if (m_Elapsed >= m_Interval)
		{
			//Reset and run state checking
			m_Elapsed = 0.f;
			if (--m_RemainingProj <= 0)
			{
				m_State = StateID::DESPAWNING;
				GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::GATE_SPAWN_00, true, false, true, true);
			}
			//Find projectile
			Projectile_Interface* proj = Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID::PLAYER_PROJ)->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::BULLET_HELL_PROJECTILE);


			if (proj != nullptr)
			{
				//Get stat container for brevity
				auto& projStats = proj->GetStats();

				//Reset Projectile
				proj->Reset();
				proj->ResetHitlist();
				
				//Pass copy of stats over to projectile
				projStats = GetStats();

				//Add to the current angle and generate a velocity
				m_CurrentAngle += m_AngleInc;
				DirectX::SimpleMath::Vector2 vel = { std::cos(m_CurrentAngle), std::sin(m_CurrentAngle) };
				vel.Normalize();

				//Set position and velocity
				proj->InitialisePositionAndVelocity(DirectX::XMFLOAT2(GetBaseSprite().GetPosition()), vel);
				//Calculate rotation with velocity
				proj->GetBaseSprite().SetRotation(MathHelper::AngleFromXY(vel.x, vel.y) - PI);


				if (m_SoundDelay == BULLET_HELL_DELAY)
				{
					//Play Bullet Hell Shot effect
					Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::BULLET_HELL_SHOT, GameVolumes::SFX);
					m_SoundDelay = 0;
				}
				++m_SoundDelay;
			}

		}
		break;

	case StateID::DESPAWNING:
		//Wait for spawn animation to finish before despawning
		if (!GetBaseAnimator().GetPlayState())
		{
			Reset();
			//Play Bullet Hell despawn effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::BULLET_HELL_DOWN, GameVolumes::SFX);

			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		break;
	
	}

}

void Projectile_BulletHell::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_BulletHell::Reset()
{
	m_State = StateID::SPAWNING;
	GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::GATE_SPAWN_00, true, false, false, true);
	m_CurrentAngle = 0.f;
	m_AngleInc = 0.f;
	return false;
}

void Projectile_BulletHell::SetFunctionalityData(float interval, short projCount, float offsetX, float offsetY, float initAngleOffset, bool reverse)
{
	m_Interval = interval;
	m_RemainingProj = projCount;
	m_OffsetX = offsetX;
	m_OffsetY = offsetY;
	m_CurrentAngle = initAngleOffset;

	m_AngleInc = (PI*2) / m_RemainingProj;
	if (reverse)
		m_AngleInc = -m_AngleInc;

}

void Projectile_BulletHell::Init()
{
	SetCollisionFlag(false);
}

void Projectile_BulletHell::Release()
{

}
