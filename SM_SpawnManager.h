#pragma once
#include "GameTimer.h"
#include "GameTypes.h"

/*
	Specialised spawner class. Manages spawning patterns 
*/
class SM_SpawnManager
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class SpecialSpawn
	{
		UNIFORM_CIRCLE_PLAYER,
		UNIFORM_CIRCLE_POSITION
	};

	////////////
	/// Data ///
	////////////

	static constexpr float WAVE_DURATION = 60.f;
	static constexpr unsigned MAX_ENTITY_TYPES_PER_WAVE = 6;

	////////////////////
	/// Constructors ///
	////////////////////

	SM_SpawnManager() { Init(); }
	~SM_SpawnManager() { Release(); }


	//////////////////
	/// Operations ///
	//////////////////

	//Initialises the spawn manager by ID (sources data from DataManager)
	void InitialiseSpawnData(unsigned patternID);

	//Main update window
	void Update(GameTimer& gt);

	//Resets current elapsed and wave count to 0 (does not reset wave data)
	void ResetCurrentSpawnPattern();

	//Debug function advance the wave by one
	void AdvanceWaveByOne();

	///////////
	/// Get ///
	///////////

	float GetElapsed() { return m_Elapsed; }
	unsigned GetCurrentWaveIndex() { return m_WaveIndex; }

	///////////
	/// Set ///
	///////////



private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//Initialises the enemy data for the current wave using current index
	void InitialiseWaveEnemyData();

	void CheckSpecialSpawn(GameTimer& gt, WaveEntityData& entry, EnemyCompositeData data);

	////////////
	/// Data ///
	////////////

	//Current Spawn Pattern
	SpawnerData m_SpawnData;
	//Local copy of entity data (modifiable)
	EnemyCompositeData m_Data[MAX_ENTITY_TYPES_PER_WAVE];
	//Current Wave Time
	float m_Elapsed = 0.f;
	//Current wave (acts as index to m_WaveData entries)
	unsigned m_WaveIndex;

	// Special Spawn Logic

	float m_MaxAngle = 360.f;
	float m_SmallRing = 150.f;
	float m_BigRing = 500.f;
	float m_OutterRing = 1000.f;

};
