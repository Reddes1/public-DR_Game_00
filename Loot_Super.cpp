#include "Loot_Super.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils

#define GET_PROJ_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ)

void Loot_Super::Update(GameTimer& gt)
{
	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));
	
	if (m_Active == true) {
		if (m_Timer <= m_Time) {
			m_Timer += gt.DeltaTime();

			m_Interval += gt.DeltaTime();
			if (m_Interval >= m_InveralTime) {

				m_Interval = 0.f;
				//Call projectile inits

				Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::SUPER);

				if (proj != nullptr)
				{

					//
					//Set Projectile Stats (Varies by Weapon Type)
					// Uses: Damage, KBE, Scale, Proj Speed, Max Collisions, ReHitDelay
					// Doesn't Use: Lifetime

					//Get stat container for brevity
					auto& projStats = proj->GetStats();

					//Reset Projectile
					proj->Reset();
					proj->ResetHitlist();

					//Player Modded Stats
					projStats.m_Damage = m_SuperStats.m_BaseDamage;
					projStats.m_Speed = m_SuperStats.m_ProjSpeed;
					projStats.m_KnockbackEffect = m_SuperStats.m_Knockback;
					projStats.m_Lifetime = m_SuperStats.m_Lifetime;

					proj->GetBaseSprite().SetScale(
					DirectX::XMFLOAT2(proj->GetGeneralData().m_Scale.x * m_SuperStats.m_AoEScale,
					proj->GetGeneralData().m_Scale.y * m_SuperStats.m_AoEScale));

					// Find Direction for super projectile
					DirectX::SimpleMath::Vector2 dir = { 0.f,0.f };
					float angle = 0.f;
					switch (Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetDirection())
					{
					case Directional8Way::UP:
						angle = 270.f;
						break;
					case Directional8Way::DOWN:
						angle = 90.f;
						break;
					case Directional8Way::LEFT:
						angle = 180.f;
						break;
					case Directional8Way::RIGHT:
						angle = 0.f;
						break;
					case Directional8Way::UP_LEFT:
						angle = 225.f;
						break;
					case Directional8Way::UP_RIGHT:
						angle = 315.f;
						break;
					case Directional8Way::DOWN_LEFT:
						angle = 135.f;
						break;
					case Directional8Way::DOWN_RIGHT:
						angle = 45.f;
						break;
					}
					
					// Get Spray Range for specific direction
					float range = MathHelper::RandF(-m_SpreadRamge, m_SpreadRamge);
					angle = angle + range;
					angle = angle * (PI / 180);
					DirectX::SimpleMath::Vector2 vel = { std::cos(angle), std::sin(angle) };
					vel.Normalize();

					//Set a rotation using velocity
					proj->GetBaseSprite().SetRotation(MathHelper::AngleFromXY(vel.x, vel.y));
					//Apply position + offset and velocity to projectile
					proj->InitialisePositionAndVelocity(DirectX::XMFLOAT2(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition()), vel);

					if (m_SoundDelay == FIREBALL_DELAY)
					{
						//Play fireball  effect
						Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::FIREBALL_POWER, GameVolumes::SFX);
						m_SoundDelay = 0;
					}
					++m_SoundDelay;
				}
				//Unable to find projectile (Should never really happen for now, so just alert this)
				else
				{
					msg_assert(false, "WeaponUpdate_Super(): Unable to find free projectile.");
				}
			}

		}
		else
		{
			Reset();
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->SetObjectStatus(GetPoolData().m_PoolIndex, false);
			Game::GetGame()->GetGameplayManager().GetLootManager().SetSuperActive(false);
		}
	}

	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
	GetStats();

}

void Loot_Super::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Super::Reset()
{
	SetPassingValue(0);
	SetFollowPlayer(false);
	m_Timer = 0.f;
	m_Active = false;
	m_Interval = 0.f;
	return true;
}
void Loot_Super::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{

	if (Game::GetGame()->GetGameplayManager().GetLootManager().GetSuperActive() == true)
	{
		Reset();
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	}
	else
	{
		Game::GetGame()->GetGameplayManager().GetLootManager().SetSuperActive(true);
		m_Active = true;
		GetBaseSprite().SetPosition(-100, -100);
	}

}
void Loot_Super::Init()
{
	m_SuperStats = Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID((unsigned)WeaponTypeID::SUPER)->m_Stats;
	Reset();
}

void Loot_Super::Release()
{
	//Release code here (deletes, clears etc)
}

