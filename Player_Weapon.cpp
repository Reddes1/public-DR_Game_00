#include "Player_Weapon.h"
#include "Game.h"	//Manager Access

//Projectile Types
#include "Projectile_Cross.h"
#include "Projectile_Whip.h"
#include "Projectile_Dagger.h"
#include "Projectile_Book.h"
#include "Projectile_Axe.h"
#include "Projectile_HexSteps.h"

//Items
#include "Projectile_Super.h"

//Brevity Macros
#define GET_PROJ_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ)
#define GET_ENEMY_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)
#define GET_ENEMY_POOL_OTHER Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITIES_01_02_03)
//Weapon specific definitions
#define BOOK_OFFSET_X 165.f
#define BOOK_OFFSET_Y 165.f
#define BOOK_OFFSET_FACTOR 0.90f

void Player_Weapon::SetPlayerPointer(Entity_Player* playerPtr)
{
	assert(playerPtr);
	m_Player = playerPtr;
}

bool Player_Weapon::LevelUpWeapon()
{
	//Check that weapon is not already equal to max
	if (!IsLevelMaxed())
	{
		//Debug checking
		msg_assert(!m_LevelSys.m_Levels.empty(), "LevelUpWeapon(): No leveling data found!");
		msg_assert(&m_LevelSys.m_Levels[m_LevelSys.m_WeaponLevel], "LevelUpWeapon(): No level data found at index!");

		//Apply each effect related to the level at the index to the weapons stats
		for (auto& a : m_LevelSys.m_Levels[m_LevelSys.m_WeaponLevel].m_LevelData)
		{
			ApplyLevelEffect(a);
		}

		//As index 0 points to Lv 2, increment after the fact
		++m_LevelSys.m_WeaponLevel;

		return true;
	}
	//Weapon Maxed, just exit out
	return false;
}

void Player_Weapon::ApplyLevelEffect(LevelUpData& data)
{
	//Access union data and apply to weapon differently based on ID
	//The effects of the new stats will apply to the next projectiles produced
	switch (static_cast<StatIDs>(data.m_StatIndex))
	{
	case StatIDs::WEAPON_BASE_DAMAGE:
		m_Stats.m_BaseDamage += data.StatModValues.m_I16;
		break;
	case StatIDs::WEAPON_PROJ_SPEED:
		m_Stats.m_ProjSpeed += data.StatModValues.m_I16;
		break;
	case StatIDs::WEAPON_KNOCKBACK:
		m_Stats.m_Knockback += data.StatModValues.m_I16;
		break;
	case StatIDs::WEAPON_BASE_PROJ_COUNT:
		m_Stats.m_InnateProjCount += data.StatModValues.m_I16;
		break;
	case StatIDs::WEAPON_MAX_COLLISIONS:
		m_Stats.m_MaxCollisions += data.StatModValues.m_I16;
		break;
	case StatIDs::WEAPON_AOE_SCALE:
		m_Stats.m_AoEScale += data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_LIFETIME:
		m_Stats.m_Lifetime += data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_COOLDOWN_MOD:
		m_Stats.m_CooldownMod -= data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_PROJ_CD_INTERVAL:
		m_Stats.m_ProjInterval += data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_PROJ_SPEED_MOD:
		m_Stats.m_ProjSpeedMod += data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_REHIT_DELAY:
		m_Stats.m_ReHitDelay += data.StatModValues.m_F32;
		break;
	case StatIDs::WEAPON_FLAT_COOLDOWN:
		m_Stats.m_Cooldown -= data.StatModValues.m_F32;
		break;

	default:
		msg_assert(false, "ApplyLevelEffect(): Invalid Stat Index in level data! No stat increase applied!");
	}
}

void Player_Weapon::Init()
{

}

void Player_Weapon::Release()
{

}

void Player_Weapon::WeaponExtraInit()
{
	switch (m_WeaponID)
	{
	case WeaponTypeID::CROSS:
		break;

	case WeaponTypeID::WHIP:
		break;

	case WeaponTypeID::DAGGER:
		break;

	case WeaponTypeID::BOMB:
		break;

	case WeaponTypeID::AURA:
		break;

	case WeaponTypeID::BOOK:
		//repurpose anchor container as offsetting baseline
		m_AnchorPos = { BOOK_OFFSET_X, BOOK_OFFSET_Y };
		//Use flag so immediately fires (for very long CD weapons)
		GenFlags.m_Flag01 = true;
		break;

	case WeaponTypeID::AXE:
		break;

	case WeaponTypeID::LIGHTNING:
		break;
	}
}

void Player_Weapon::DowntickProjectileCount()
{
	//Tickdown projectile count
	--m_RemainingProjCount;

	//If no projectiles remaining, then cycle complete
	if (m_RemainingProjCount <= 0)
	{
		//Set regular CD
		CalcRemainingCD();
		//Reset projectile remaining projectile count
		CalcProjCount();

		GenFlags.m_FreshWeaponCycle = true;
	}
	//There are still projectiles to fire, set the CD time to the interval time so that the following projectiles are produced faster
	else
	{
		m_RemainingCD = m_Stats.m_ProjInterval;
		GenFlags.m_FreshWeaponCycle = false;
	}
}

short Player_Weapon::CalculateDamage()
{
	//Weapon Damage * Player Damage Mod
	return static_cast<short>(std::round(m_Stats.m_BaseDamage * m_Player->GetStats().m_WeaponDMGMod));
}

short Player_Weapon::CalculateProjectileSpeed()
{
	//(Weapon Proj Speed * Weapon Proj Speed Mod) * Player Proj Speed Mod
	return static_cast<short>((m_Stats.m_ProjSpeed * m_Stats.m_ProjSpeedMod) * m_Player->GetStats().m_WeaponProjSpeedMod);
}

short Player_Weapon::CalculateKnockbackEffect()
{
	//Weapon Knockback Effect * Player Knockback Mod
	return static_cast<short>(m_Stats.m_Knockback * m_Player->GetStats().m_WeaponKnockbackMod);
}

float Player_Weapon::CalculateLifetime()
{
	//Weapon Lifetime * Player Duration Mod
	return m_Stats.m_Lifetime * m_Player->GetStats().m_WeaponDurationMod;
}

DirectX::XMFLOAT2 Player_Weapon::CalculateAoEScale(Projectile_Interface* const proj)
{
	//Pending proper testing as individual sprites may need different scaling in program to get them size appropriate
	//Weapon AoE * Player AoE Mod
	return DirectX::XMFLOAT2((proj->GetGeneralData().m_Scale.x * m_Stats.m_AoEScale) * m_Player->GetStats().m_WeaponAoEMod, (proj->GetGeneralData().m_Scale.y * m_Stats.m_AoEScale) * m_Player->GetStats().m_WeaponAoEMod);
}

float Player_Weapon::CalculateInverseLifetime()
{
	//Get current duration/lifetime mod
	float temp = m_Player->GetStats().m_WeaponDurationMod;
	//Isolate any bonuses/negatives applied to stat
	temp -= 1.f;
	temp = 1.f - temp;
	return m_Stats.m_Lifetime * temp;
}

void Player_Weapon::CalcRemainingCD()
{
	m_RemainingCD = (m_Stats.m_Cooldown * (m_Stats.m_CooldownMod * m_Player->GetStats().m_WeaponCDMod));
}

void Player_Weapon::CalcProjCount()
{
	m_RemainingProjCount = m_Stats.m_InnateProjCount + m_Player->GetStats().m_ExtraProjCount;
}

float Player_Weapon::CalcLifetime()
{
	return m_Stats.m_Lifetime * m_Player->GetStats().m_WeaponDurationMod;
}

void Player_Weapon::WeaponUpdate_Cross(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::CROSS);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, Proj Speed, KBE, Scale, ReHitDelay
			// Doesn't Use: Lifetime, Max Collisions

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();
			
			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_Speed = CalculateProjectileSpeed();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));
			//Non-Player Modded Stats
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;		

			//
			//Position & Direction Behaviour
			//

			//Calculate a random direction vector & normalise it
			DirectX::SimpleMath::Vector2 dir = { MathHelper::RandF(-100.f, 100.f), MathHelper::RandF(-100.f, 100.f) };
			dir.Normalize();

			//Set position and velocity
			proj->InitialisePositionAndVelocity(DirectX::XMFLOAT2(m_Player->GetRelativePosition()), dir);
			
			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();

			//Play Cross weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::CROSS, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Cross(): Unable to find free projectile.");
		}
	}
}

void Player_Weapon::WeaponUpdate_Whip(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Get projectile here
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::WHIP);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, ReHitDelay
			// Doesn't Use: Proj Speed, Max Collisions, Lifetime(Tied to Anim Time)

			//Get stat container for brevity
			auto& projStats = proj->GetStats();
			
			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();		
			//Non-Player Modded Stats
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));
			
			//
			//Behaviour
			//

			//Check to see if this a fresh cycle or not, capturing a new anchor if so
			if (GenFlags.m_FreshWeaponCycle)
				m_AnchorPos = m_Player->GetRelativePosition();
			//Copy current anchor position and add to it through following processes
			DirectX::SimpleMath::Vector2 pos = m_AnchorPos;
			//Calc difference between max possible projectiles and current
			int projDiff = (GetStats().m_InnateProjCount + m_Player->GetStats().m_ExtraProjCount) - m_RemainingProjCount;

			//Position based on if the remaining proj count is odd/even + an offset based on the difference between
			//Right of player (Odd)
			if (projDiff % 2)
			{
				//Subtract half-rect size of the whip from player position (setting it on the left of the player)
				pos.x += (proj->GetBaseSprite().GetScaleAdjustedRectX() * 0.5f) + (m_Player->GetGeneralData().m_CollisionBox.x * 0.40f);
				//Position the Y with an offset based on remaining projectiles (is directly next to player on the first
				pos.y -= proj->GetBaseSprite().GetScaleAdjustedRectY() * 0.5f * (projDiff - 1) * 0.65f;

				//Set Sprite Effect
				proj->GetBaseSprite().SetEffect(DirectX::SpriteEffects_None);
			}
			//Left of Player (Even)
			else
			{
				//Subtract half-rect size of the whip from player position (setting it on the left of the player)
				pos.x -= (proj->GetBaseSprite().GetScaleAdjustedRectX() * 0.5f) + (m_Player->GetGeneralData().m_CollisionBox.x * 0.40f);
				//Position the Y with an offset based on remaining projectiles (is directly next to player on the first
				pos.y -= proj->GetBaseSprite().GetScaleAdjustedRectY() * 0.5f * (projDiff * 0.65f);

				//Set Sprite Effect
				proj->GetBaseSprite().SetEffect(DirectX::SpriteEffects_FlipHorizontally);
			}

			//Set position
			proj->GetBaseSprite().SetPosition(pos);
			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();

			//Play Whip weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::WHIP, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Whip(): Unable to find free projectile.");
		}
	}
}

void Player_Weapon::WeaponUpdate_Dagger(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::DAGGER);
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
			projStats.m_Damage = CalculateDamage();
			projStats.m_Speed = CalculateProjectileSpeed();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			//Calculate scale (with diminished effect on the Y to maintain shape
			auto scale = CalculateAoEScale(proj);
			proj->GetBaseSprite().SetScale(scale.x, scale.y * 0.60f);
			
			//Non-Player Modded Stats
			projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;


			//Based on player direction, shoot daggers in certain direction
			//Hold velocity container here and offset value for this dagger
			DirectX::SimpleMath::Vector2 vel = { 0.f, 0.f };
			DirectX::SimpleMath::Vector2 offset = { 0.f, 0.f };
			DirectX::SimpleMath::Vector2 playerSize = { m_Player->GetBaseSprite().GetScaleAdjustedRectX() , m_Player->GetBaseSprite().GetScaleAdjustedRectY() };
			switch (m_Player->GetDirection())
			{
			case Directional8Way::UP:
				vel.y = -1.f;
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				break;																				
			case Directional8Way::DOWN:																
				vel.y = 1.f;																		
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				break;																				
			case Directional8Way::LEFT:																
				vel.x = -1.f;																		
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				break;																				
			case Directional8Way::RIGHT:															
				vel.x = 1.f;																		
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				break;																				
			case Directional8Way::UP_LEFT:															
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				vel = { -1.f, -1.f };																
				break;																				
			case Directional8Way::UP_RIGHT:															
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				vel = { 1.f, -1.f };																
				break;																				
			case Directional8Way::DOWN_LEFT:														
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				vel = { -1.f, 1.f };																
				break;																				
			case Directional8Way::DOWN_RIGHT:														
				offset.x = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				offset.y = MathHelper::RandF(-playerSize.x * 0.35f, playerSize.y * 0.35f);
				vel = { 1.f, 1.f };
				break;
			}
			vel.Normalize();

			//Set a rotation using velocity
			proj->GetBaseSprite().SetRotation(MathHelper::AngleFromXY(vel.x, vel.y));
			//Apply position + offset and velocity to projectile
			proj->InitialisePositionAndVelocity(DirectX::XMFLOAT2(m_Player->GetRelativePosition() + offset), vel);

			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();

			//Play Dagger weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::DAGGER, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Dagger(): Unable to find free projectile.");
		}
	}
}

void Player_Weapon::WeaponUpdate_Bomb(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::BOMB);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, Lifetime, ReHitDelay
			// Doesn't Use: Proj Speed, Max Collisions

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));
			//Scales positively with reduced lifetime so use inverse calculation
			projStats.m_Lifetime = CalculateInverseLifetime();			
			//Non-Player Modded Stats
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;;

			//
			//Position & Direction Behaviour
			//

			//Set position (with minor offset on Y to account for origin shifting on sprite)
			proj->InitialisePosition(DirectX::XMFLOAT2(m_Player->GetRelativePosition().x, m_Player->GetRelativePosition().y + 20.f));

			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Bomb(): Unable to find free projectile.");
		}
	}
}

void Player_Weapon::WeaponUpdate_Aura(GameTimer& gt)
{
	//If aura projectile already tracked
	if (m_TrackedProj != nullptr)
	{
		//Update projectile stats using remaining cd timer as an interval
		m_RemainingCD -= gt.DeltaTime();
		if (m_RemainingCD <= 0.f)
		{
			//Reset timer to fixed time
			m_RemainingCD = 1.0f;
			
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, ReHitDelay, Lifetime(CDR)
			// Doesn't Use: Proj Speed, Max Collisions 

			//Get stat container for brevity
			auto& projStats = m_TrackedProj->GetStats();

			//Calculate stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			m_TrackedProj->GetBaseSprite().SetScale(CalculateAoEScale(m_TrackedProj));
			//Lifetime is used as a standin for CDR on the weapon, so run calculation for it here
			projStats.m_Lifetime = (m_Stats.m_Cooldown * (m_Stats.m_CooldownMod * m_Player->GetStats().m_WeaponCDMod));
		}
	}
	else
	{
		//Find projectile
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::AURA);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, ReHitDelay, Lifetime(CDR)
			// Doesn't Use: Proj Speed, Max Collisions

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));
			//Lifetime is used as a standin for CDR on the weapon, so run calculation for it here
			projStats.m_Lifetime = (m_Stats.m_Cooldown * (m_Stats.m_CooldownMod * m_Player->GetStats().m_WeaponCDMod));
			//Non-Player Modded Stats
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;

			//Set position initial position
			proj->InitialisePosition(DirectX::XMFLOAT2(m_Player->GetRelativePosition()));

			//Store projectile as tracked
			m_TrackedProj = proj;

		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Aura(): Unable to find free projectile.");
		}
	}
}
void Player_Weapon::WeaponUpdate_Book(GameTimer& gt)
{
	//If flag active, put CD to zero once
	if (GenFlags.m_Flag01)
	{
		m_RemainingCD = 0.f;
		GenFlags.m_Flag01 = false;
	}		

	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Recalculate the max projectile count
		CalcProjCount();
		//Find an amount of projectiles equal to projectile count (with casting as need to access specific calls)
		std::vector<Projectile_Book*> projs;
		projs.reserve(m_RemainingProjCount);
		for (unsigned i(0); i < (unsigned)m_RemainingProjCount; ++i)
		{
			Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::BOOK);
			if (proj != nullptr)
				projs.push_back((Projectile_Book*)proj);
		}
		//If any free projectiles found
		if (!projs.empty())
		{
			//Get player position
			auto pos = m_Player->GetRelativePosition();
			
			//Calculate a number of angles based on number of found projectiles
			float angle = (PI*2) / projs.size();
			for (unsigned i(0); i < projs.size(); ++i)
			{
				//
				//Set Projectile Stats (Varies by Weapon Type)
				// Uses: Damage, KBE, Scale, Proj Speed, ReHitDelay
				// Doesn't Use: Lifetime, Max Collisions

				//Get stat container for brevity
				auto& projStats = projs[i]->GetStats();

				//Reset Projectile
				projs[i]->Reset();
				projs[i]->ResetHitlist();

				//Player Modded Stats
				projStats.m_Damage = CalculateDamage();
				projStats.m_Speed = CalculateProjectileSpeed();
				projStats.m_KnockbackEffect = CalculateKnockbackEffect();
				projStats.m_Lifetime = CalculateLifetime();

				//Setup projectile specific data
				projs[i]->SetFunctionalityData(
					angle * i,
					m_AnchorPos.x * (m_Stats.m_AoEScale * m_Player->GetStats().m_WeaponAoEMod),
					m_AnchorPos.y * (m_Stats.m_AoEScale * m_Player->GetStats().m_WeaponAoEMod)
				);
				projs[i]->InitialisePosition(pos);
			}
			CalcRemainingCD();

			//Play Book weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::BOOK, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Book(): Unable to find free projectile.");
		}
	}
}
void Player_Weapon::WeaponUpdate_Axe(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::AXE);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, ReHitDelay, Max Collisions, Proj Speed
			// Doesn't Use: Lifetime

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			projStats.m_Speed = CalculateProjectileSpeed();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));

			//Non-Player Modded Stats
			projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;

			//
			//Position & Direction Behaviour
			//

			//Generate a random X velocity, and minor randomness on Y velocity around -1.f
			DirectX::SimpleMath::Vector2 vel = {
				MathHelper::RandF(-0.32f, 0.32f),
				MathHelper::RandF(0.10f, 0.20f) - 1.f
			};

			//Set position and velocity
			proj->InitialisePositionAndVelocity(m_Player->GetRelativePosition(), vel);
			//Add an additional offset
			proj->GetBaseSprite().AddToPositionY(-35.f);

			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();

			//Play Whip weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::AXE, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Axe(): Unable to find free projectile.");
		}
	}
}
void Player_Weapon::WeaponUpdate_Hex(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::HEX);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, Scale, ReHitDelay
			// Doesn't Use: Lifetime, KBE, Max Collisions, Proj Speed, Proj Count

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));
			projStats.m_Lifetime = CalculateLifetime();

			//Non-Player Modded Stats
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;

			//Set position
			proj->InitialisePosition(m_Player->GetRelativePosition());

			//Set regular CD
			CalcRemainingCD();
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Hex(): Unable to find free projectile.");
		}
	}
}
void Player_Weapon::WeaponUpdate_Lightning(GameTimer& gt)
{
	//Downtick the remaining CD time
	m_RemainingCD -= gt.DeltaTime();

	//If remaining time elapsed, start attack process
	if (m_RemainingCD <= 0.f)
	{
		//Call projectile inits
		Projectile_Interface* proj = GET_PROJ_POOL_00->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::LIGHTNING);
		if (proj != nullptr)
		{
			//
			//Set Projectile Stats (Varies by Weapon Type)
			// Uses: Damage, KBE, Scale, ReHitDelay, Max Collisions, Proj Speed
			// Doesn't Use: Lifetime

			//Get stat container for brevity
			auto& projStats = proj->GetStats();

			//Reset Projectile
			proj->Reset();
			proj->ResetHitlist();

			//Player Modded Stats
			projStats.m_Damage = CalculateDamage();
			projStats.m_KnockbackEffect = CalculateKnockbackEffect();
			projStats.m_Speed = CalculateProjectileSpeed();
			projStats.m_Lifetime = CalculateLifetime();
			proj->GetBaseSprite().SetScale(CalculateAoEScale(proj));

			//Non-Player Modded Stats
			projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;

			//
			//Entity Position
			//

			// Get a random target to target on screen
			if (GET_ENEMY_POOL_00->GetInUseCount() != 0 && GET_ENEMY_POOL_OTHER->GetInUseCount() != 0) {
				unsigned randTarget = rand() % GET_ENEMY_POOL_00->GetInUseCount()+ GET_ENEMY_POOL_OTHER->GetInUseCount();

				for (unsigned i = 0; i < GET_ENEMY_POOL_00->GetPoolSize(); i++)
				{
					if (GET_ENEMY_POOL_00->GetUseFlagAtIndex(i) == true)
					{
						if (i == randTarget)
							proj->InitialisePosition(DirectX::XMFLOAT2(GET_ENEMY_POOL_00->GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox.posX, GET_ENEMY_POOL_00->GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox.posY));
					}
					else
						randTarget++;
				}
				for (unsigned i = 0; i < GET_ENEMY_POOL_OTHER->GetPoolSize(); i++)
				{
					if (GET_ENEMY_POOL_OTHER->GetUseFlagAtIndex(i) == true)
					{
						if (i == randTarget)
							proj->InitialisePosition(DirectX::XMFLOAT2(GET_ENEMY_POOL_OTHER->GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox.posX, GET_ENEMY_POOL_OTHER->GetObjectAtIndex(i)->GetCollisionData().m_CollisonBox.posY));
					}
					else
						randTarget++;
				}
			}



			//Dec proj count and determine how to set weapon CD
			DowntickProjectileCount();

			//Play Whip weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::AXE, GameVolumes::SFX);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "WeaponUpdate_Trail(): Unable to find free projectile.");
		}
	}
}
void Player_Weapon::Update(GameTimer& gt)
{
	//Redirect the update based on weapon ID
	switch (m_WeaponID)
	{
	case WeaponTypeID::CROSS:
		WeaponUpdate_Cross(gt);
		break;

	case WeaponTypeID::WHIP:
		WeaponUpdate_Whip(gt);
		break;

	case WeaponTypeID::DAGGER:
		WeaponUpdate_Dagger(gt);
		break;

	case WeaponTypeID::BOMB:
		WeaponUpdate_Bomb(gt);
		break;

	case WeaponTypeID::AURA:
		WeaponUpdate_Aura(gt);
		break;

	case WeaponTypeID::BOOK:
		WeaponUpdate_Book(gt);
		break;

	case WeaponTypeID::AXE:
		WeaponUpdate_Axe(gt);
		break;

	case WeaponTypeID::HEX:
		WeaponUpdate_Hex(gt);
		break;

	case WeaponTypeID::LIGHTNING:
		WeaponUpdate_Lightning(gt);
		break;	
	case WeaponTypeID::UNDEFINED:
		//The weapon has not been setup beyond its initial state, so skip updating for now
		break;
	default:
		//Undefined weapon being updated, likely not intended
		msg_assert(false, "PlayerWeaponUpdate(): Default Switch hit, intended?");
	}
}

void Player_Weapon::InitialiseWeapon(WeaponTypeID weaponID, WeaponCompositeData& statPack)
{
	//Default state is undefined, so shouldn't be re-defined as that
	msg_assert((unsigned)weaponID < (unsigned)WeaponTypeID::UNDEFINED,
		"InitialiseWeapon(): Weapon being init as invalid type");
	
	//Set weapon ID
	m_WeaponID = weaponID;
	//Get baseline stats and data
	m_Stats = statPack.m_Stats;
	m_GenData = statPack.m_GeneralData;
	m_LevelSys = statPack.m_LevelData;
	m_IconData = statPack.m_IconData;

	//Sync certain internal data with player data
	ResyncWeapon();

	//Any extra setup required for weapons
	WeaponExtraInit();

	//Setup flags
	GenFlags.m_FreshWeaponCycle = true;
}

void Player_Weapon::ResyncWeapon()
{
	//Safety net as player ptr may not be set yet
	if (m_Player)
	{
		CalcProjCount();
		CalcRemainingCD();
	}
}

void Player_Weapon::DisableWeapon()
{
	m_WeaponID = WeaponTypeID::UNDEFINED;
	m_TrackedProj = nullptr;
}
