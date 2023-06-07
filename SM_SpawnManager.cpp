#include "SM_SpawnManager.h"
#include "Entity_EnemyType_00.h"

#include "GameConstants.h"	//Global Info
#include "Game.h"			//Manager Access

void SM_SpawnManager::Init()
{

}

void SM_SpawnManager::Release()
{

}

void SM_SpawnManager::InitialiseWaveEnemyData()
{
	//Get data manager for brevity
	auto dataMgr = Game::GetGame()->GetGameplayManager().GetDataManager();

	//For each entry in this wave, get copy of associated enemy data + store flags/apply mods
	for (unsigned i(0); i < m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries.size(); ++i)
	{
		//Get entry for brevity
		auto& entry = m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i];

		//Get Data Copy
		auto data = dataMgr.GetEnemyDataAtID(entry.m_EnemyDataID);
		msg_assert(data, "InitialiseWaveEnemyData(): Enemy Data ID Invalid, No Data Found!");
		m_Data[i] = *data;

		//Store required flags in enemy data
		m_Data[i].m_Flags.m_DropsChest = entry.m_DropsChest;
		m_Data[i].m_Flags.m_ForcedItemDrop = entry.m_ForcedItemDrop;
		
		//If mods enabled, adjust the enemy stats accordingly
		if (entry.m_ModsEnabled)
		{				
			m_Data[i].m_Stats.m_MaxHealth = static_cast<short>(std::round((float)m_Data[i].m_Stats.m_MaxHealth * entry.m_HealthMod));
			m_Data[i].m_Stats.m_Movespeed = static_cast<short>(std::round((float)m_Data[i].m_Stats.m_Movespeed * entry.m_MovespeedMod));
			m_Data[i].m_Stats.m_DamageMod = static_cast<unsigned short>(std::round((float)m_Data[i].m_Stats.m_DamageMod * entry.m_DamageMod));
			m_Data[i].m_Stats.m_XPValue = static_cast<unsigned short>(std::round((float)m_Data[i].m_Stats.m_XPValue * entry.m_XPMod));
			m_Data[i].m_GeneralData.m_Scale.x *= entry.m_ScaleMod;
			m_Data[i].m_GeneralData.m_Scale.y *= entry.m_ScaleMod;
		}
	}
}

void SM_SpawnManager::InitialiseSpawnData(unsigned patternID)
{
	//Grab a copy of the data found at location
	auto spawnData = Game::GetGame()->GetGameplayManager().GetDataManager().GetSpawnDataAtID(patternID);
	msg_assert(spawnData, "InitialiseSpawnData(): No spawn data found");

	m_SpawnData = *spawnData;

	m_WaveIndex = 0;
	m_Elapsed = 0.f;

	InitialiseWaveEnemyData();
}
void SM_SpawnManager::CheckSpecialSpawn(GameTimer& gt, WaveEntityData& entry, EnemyCompositeData data)
{
	//Get pool for brevity
	auto pool = Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(
		(SM_EntityManager::PoolID)entry.m_PoolID);

	if (entry.m_SpawnLogicID == (unsigned)SpecialSpawn::UNIFORM_CIRCLE_PLAYER)
	{
		//Get player position
		DirectX::XMFLOAT2 pos = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();

		// Divide max angle into the number of enemies in the wave
		float angleIncrease = m_MaxAngle / static_cast<float>(entry.m_SpawnCount);
		float angle = 0;

		// Spawn an amount per each interval
		for (unsigned j(0); j < entry.m_SpawnCount; ++j)
		{
			//Find free entity using wave information
			EM_EntityInterface2D* ent = pool->FindFreeObjectByInternalTypeID(entry.m_BehaviourTypeID);

			//If entity found, spawn it
			if (ent)
			{
				// Set spawn for each entity around the circle
				angle += angleIncrease;
				ent->InitSpecialSpawn(DirectX::XMFLOAT2(
					std::sinf(DegreesToRadian(angle)) * (m_OutterRing)+pos.x,
					std::cosf(DegreesToRadian(angle)) * (m_OutterRing)+pos.y),
					data);
			}
			//No more free entities, so force a break (not great but functional)
			else
				break;
		}
	}
	else if (entry.m_SpawnLogicID == (unsigned)SpecialSpawn::UNIFORM_CIRCLE_POSITION)
	{

	}
}
void SM_SpawnManager::Update(GameTimer& gt)
{
	//Uptick current wave time
	m_Elapsed += gt.DeltaTime();

	// Checks for wave every minuite of gametime
	if (m_Elapsed >= WAVE_DURATION)
	{
		//Increment the Wave
		if (m_WaveIndex < m_SpawnData.m_WaveCount - 1)
		{
			//Increment wave index and get new data for the wave
			++m_WaveIndex;
			InitialiseWaveEnemyData();

			//Reset Time
			m_Elapsed = 0.f;
		}
		//Max waves hit, so end game
		else
		{
			Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::ActiveGameState::WIN_STATE_GAME_COMPLETE);
		}
	}	

	// Spawn Entitys
	for (unsigned i(0); i < m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries.size(); ++i)
	{				
		//Uptick elapsed 
		m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_Elapsed += gt.DeltaTime();

		//If interval passed, move to spawning enemies
		if (m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_Elapsed >= m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_SpawnInterval)
		{
			//Reset timer (if spawn once flag active, set timer to very low number so another spawn wont occur
			m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].SetElapsedWithSpawnCheck();
			//Get pool for brevity
			auto pool = Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(
				(SM_EntityManager::PoolID)m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_PoolID);
			msg_assert(pool, "Update(): Entity Pool Not Found!");


			if (m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_SpecialSpawnOverride == true)
				// Check Special Spawn Type
				CheckSpecialSpawn(gt, m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i], m_Data[i]);
			else {
				// Spawn an amount per each interval
				for (unsigned j(0); j < m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_SpawnCount; ++j)
				{
					//Find free entity using wave information
					EM_EntityInterface2D* ent = pool->FindFreeObjectByInternalTypeID(m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_BehaviourTypeID);

					//If entity found, spawn it
					if (ent)
					{
						ent->SpawnEntity(m_SpawnData.m_Waves[m_WaveIndex].m_WaveEntries[i].m_SpawnLogicID, m_Data[i]);
					}
					//No more free entities, so force a break (not great but functional)
					else
						break;
				}
			}


		}
	}
}

void SM_SpawnManager::ResetCurrentSpawnPattern()
{
	//Reset current wave index and timers to 0
	m_Elapsed = 0.f;
	m_WaveIndex = 0;

	//Cycle through all the current spawn pattern data and reset all the existing timers to zero
	for (unsigned i(0); i < m_SpawnData.m_Waves.size(); ++i)
	{
		for (unsigned j(0); j < m_SpawnData.m_Waves[i].m_WaveEntries.size(); ++j)
		{
			m_SpawnData.m_Waves[i].m_WaveEntries[j].CalculateInitialElapsed();
		}
	}

	InitialiseWaveEnemyData();
}

void SM_SpawnManager::AdvanceWaveByOne()
{
	//Increment the Wave
	if (m_WaveIndex < m_SpawnData.m_WaveCount - 1)
	{
		//Disable all current entities 
		Game::GetGame()->GetGameplayManager().GetEntityManager().TurnOffAllPools();
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->WaveAdvanceFix();

		//inc wave index and get new data for the wave
		++m_WaveIndex;
		InitialiseWaveEnemyData();
		m_Elapsed = 0.f;
	}
	//Max waves hit, so end game
	else
	{
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::ActiveGameState::WIN_STATE_GAME_COMPLETE);
	}
}
