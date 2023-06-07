#include "Player_Ability.h"

#include "Game.h"	//Manager Access

#include "Projectile_WaterDragon.h"		//Wizard Special
#include "Projectile_BulletHell.h"		//Gunner Special
#include "Projectile_HoH.h"				//Knight Special

#define OFFSET_X 120.f
#define OFFSET_Y 120.f
#define GATE_COUNT 4

void Player_Ability::InitialiseAbility(unsigned id)
{
	//Use ID to find "weapon" data
	auto data = Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID(id);

	m_GenData = data->m_GeneralData;
	m_Stats = data->m_Stats;
	m_ID = (WeaponTypeID)id;
}

void Player_Ability::Update(GameTimer& gt)
{
	//Uptick CD
	if (m_ElapsedCD < m_Stats.m_Cooldown)
	{
		m_ElapsedCD += gt.DeltaTime();
		if (m_ElapsedCD > m_Stats.m_Cooldown)
			m_ElapsedCD = m_Stats.m_Cooldown;
	}		
}

void Player_Ability::ActivateAbility()
{
	//Run the setup differently based on ability ID
	switch (m_ID)
	{
	case WeaponTypeID::HEAVEN_ON_HIGH_ANGEL:
		SetupHoH();
		break;

	case WeaponTypeID::BULLET_HELL_GATE:
		SetupBulletHell();
		break;

	case WeaponTypeID::WATER_DRAGON:
		SetupDragon();
		break;
	
	default:
		msg_assert(false, "ActivateAbility(): No Valid ID Set.");
	}

	//Reset cooldown timer
	m_ElapsedCD = 0.f;
}

void Player_Ability::Reset()
{
	m_ElapsedCD = 0.f;
}

void Player_Ability::SetPlayerPointer(Entity_Player* playerPtr)
{
	assert(playerPtr);
	m_Player = playerPtr;
}

void Player_Ability::Init()
{

}

void Player_Ability::Release()
{

}

void Player_Ability::SetupDragon()
{
	//Find a free projectile
	Projectile_Interface* proj = Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
		SM_EntityManager::PoolID::PLAYER_PROJ)->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::WATER_DRAGON);
	if (proj != nullptr)
	{
		//Get stat container for brevity
		auto& projStats = proj->GetStats();

		//Reset Projectile
		proj->Reset();
		proj->ResetHitlist();

		//Set stats
		projStats.m_Damage = m_Stats.m_BaseDamage;
		projStats.m_Lifetime = m_Stats.m_Lifetime;
		projStats.m_KnockbackEffect = m_Stats.m_Knockback;
		projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;
		projStats.m_Speed = m_Stats.m_ProjSpeed;
		projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;

		//Set position (with minor offset on Y to account for origin shifting on sprite)
		proj->InitialisePosition(m_Player->GetRelativePosition());

		//Play Water Dragon Ability effect
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::WATER_DRAGON, GameVolumes::SFX);

	}
	else
		msg_assert(false, "SetupDragon(): No projectile found!");

}

void Player_Ability::SetupBulletHell()
{
	std::vector<Projectile_BulletHell*> projs;
	projs.reserve(4);

	for (unsigned i(0); i < GATE_COUNT; ++i)
	{
		Projectile_Interface* proj = Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
			SM_EntityManager::PoolID::PLAYER_PROJ)->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::BULLET_HELL_GATE);
		if (proj != nullptr)
			projs.push_back((Projectile_BulletHell*)proj);
		//Play Bullet Hell Activate effect
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::BULLET_HELL_ACTIVE, GameVolumes::SFX);
	}
	//If any free projectiles found
	if (!projs.empty() && projs.size() == GATE_COUNT)
	{
		//Get player position and set gate positions with offsets
		auto pos = m_Player->GetRelativePosition();

		for (unsigned i(0); i < projs.size(); ++i)
		{
			//Get stat container for brevity
			auto& projStats = projs[i]->GetStats();

			//Reset Projectile
			projs[i]->Reset();
			projs[i]->ResetHitlist();

			//Pass values that will then be passed onto each sub-projectile
			projStats.m_Damage = m_Stats.m_BaseDamage;
			projStats.m_Lifetime = m_Stats.m_Lifetime;
			projStats.m_KnockbackEffect = m_Stats.m_Knockback;
			projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;
			projStats.m_Speed = m_Stats.m_ProjSpeed;
			projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;

			//Set position equal to player
			projs[i]->GetBaseSprite().SetPosition(pos);
		}

		//Set functionality data
		projs[0]->SetFunctionalityData(GetStats().m_ProjInterval, GetStats().m_InnateProjCount, -OFFSET_X, -OFFSET_Y, PI/2.f, false);
		projs[1]->SetFunctionalityData(GetStats().m_ProjInterval, GetStats().m_InnateProjCount, OFFSET_X, -OFFSET_Y, 0.f, false);
		projs[2]->SetFunctionalityData(GetStats().m_ProjInterval, GetStats().m_InnateProjCount, OFFSET_X, OFFSET_Y, PI*1.5f, false);
		projs[3]->SetFunctionalityData(GetStats().m_ProjInterval, GetStats().m_InnateProjCount, -OFFSET_X, OFFSET_Y, PI, false);
	}
	//Unable to find projectile (Should never really happen for now, so just alert this)
	else
	{
		msg_assert(false, "SetupBulletHell(): Not enough gates found!");
	}
}

void Player_Ability::SetupHoH()
{
	//Find a free projectile
	Projectile_Interface* proj = Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
		SM_EntityManager::PoolID::PLAYER_PROJ)->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::HEAVEN_ON_HIGH_ANGEL);
	//Play Angel Spawn Ability effect
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::ANGEL_SPAWN, GameVolumes::SFX);
	if (proj != nullptr)
	{
		//Get stat container for brevity
		auto& projStats = proj->GetStats();

		//Reset Projectile
		proj->Reset();
		proj->ResetHitlist();

		//Set stats
		projStats.m_Damage = m_Stats.m_BaseDamage;
		projStats.m_Lifetime = m_Stats.m_Lifetime;
		projStats.m_KnockbackEffect = m_Stats.m_Knockback;
		projStats.m_ReHitDelay = m_Stats.m_ReHitDelay;
		projStats.m_Speed = m_Stats.m_ProjSpeed;
		projStats.m_CollisionsRemaining = m_Stats.m_MaxCollisions;

		static_cast<Projectile_HoH*>(proj)->SetFunctionalityData(GetStats().m_ProjInterval, GetStats().m_InnateProjCount, -350.f);

	}
	else
		msg_assert(false, "SetupHoH(): No projectile found!");
}
