#pragma once
#include "D3DUtils.h"
#include "GameTimer.h"		//Delta & Gametime
#include "EM_Coordinates.h" //World space tracking
#include "GameTypes.h"		//PoolID Data

/*
	Parent class for use in creating game entities. Allows for high level access and calls for updating, rendering and basic flags.
	Child classes can be composed of modular functionality classes, each providing key behaviour functions (movement, sprites, animation, actions etc)
*/

class EM_EntityInterface2D
{
public:

	////////////
	/// Data ///
	////////////

	struct EntityFlags
	{
		EntityFlags()
			:m_IsActive(false), m_Renderable(false), m_IsNudge(false), m_IsSleep(false)
		{}

		//Is the entity updatable/interactable
		bool m_IsActive : 1;
		//Is the entity draw enabled
		bool m_Renderable : 1;
		//Is the entity to be nudged
		bool m_IsNudge : 1;
		//Is the entity to be slept
		bool m_IsSleep : 1;


	};

	////////////////////
	/// Constructors ///
	////////////////////

	EM_EntityInterface2D() {}
	virtual ~EM_EntityInterface2D() {}


	////////////////
	/// Virtuals ///
	////////////////

	//
	//Operations
	//
	
	//Use this call to update any behaviours for the object
	virtual void Update(GameTimer& gt) = 0;
	//Use this call to draw any sprites (EM_Sprite etc) owned by the object
	virtual void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) = 0;

	//
	//Spawning
	//

	//Generalist window for spawning logic.
	//posInit: Spawn position/velocity logic (defined by child), data: data package from DataManager
	//Designed for specifically spawning enemy entities
	virtual bool SpawnEntity(unsigned initPos, const EnemyCompositeData& data) { return false; }
	virtual void InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data) {}

	//
	//Combat
	//

	//Defines a general method of how the entity should process a damage amount being applied to it
	virtual bool ProcessDamageEvent(short damage, short knockback) { return false; }
	//Defines how the entity should resolve collision effects generically
	virtual bool ProcessCollisionEvent() { return false; }
	//Conditional check for if the entity can attack (subjective to the entity)
	virtual bool CanCollisionDamageOccur() { return false; }
	//Resets the entities collision timer
	virtual void ResetCollisionTimer() { }
	//Returns a damage value related to the entity (use m_Stats appropriately to entity type)
	virtual short GetDamageValue() { return 0; }

	//
	//Utilities
	//
	
	//Optional defaulter function so that any entity can be reset to a definable state
	virtual void ResetEntityToDefaults() {}

	///////////
	/// Get ///
	///////////

	//
	//Basics
	//

	EM_Coordinates& GetCoords()			            { return m_WorldCoordinates; }
	bool GetRenderState()						    { return m_Flags.m_Renderable; }
	bool GetActiveState()				            { return m_Flags.m_IsActive; }	
	unsigned GetID()								{ return m_ID; }
	
	//
	//Additions
	//

	PoolIDs& GetPoolData()							{ return m_PoolData; }
	bool GetNudge()						            { return m_Flags.m_IsNudge; }

	DirectX::XMFLOAT2 GetNudgeDir()				    { return m_NudgeDir; }				    
	const ObjectCol& GetCollisionData()			{ return m_CollisionBox; }

	//Union-based get: only call if player entity
	PlayerStats& GetPlayerStats()					{ return m_Stats.m_PlayerStats; }
	//Union-based get: only call if enemy entity
	EnemyStats& GetEnemyStats()						{ return m_Stats.m_EnemyStats; }
	GeneralEntityData& GetGeneralData()				{ return m_GenData; }
	EnemyFlags& GetFlags()							{ return m_EnemyFlags; }

	bool GetCollisionCheck() { return m_CollisionCheck; }

	///////////
	/// Set ///
	///////////

	//
	//Basics
	//

	void SetActiveState(bool isActive)              { m_Flags.m_IsActive = isActive; }
	void SetRenderState(bool isRenderable)          { m_Flags.m_Renderable = isRenderable; }
	void SetID(unsigned id)							{ m_ID = id; }
	
	//
	//Additions
	//

	void SetCollisionData(CollisionArea& collider)  { m_CollisionBox.m_CollisonBox = collider; }
	void SetCollisionLayer(CollisionLayers collLayer) {m_CollisionBox.m_CollisonLayer = collLayer; }




	void SetNudge(bool isNudged)                    { m_Flags.m_IsNudge = isNudged; }
	void SetNudgeDirection(DirectX::XMFLOAT2& dir)  { m_NudgeDir = dir; }

	void SetCollisionFlag(bool coll) { m_CollisionCheck = coll; }
private:

	///////////////////
	/// Definitions ///
	///////////////////

	//Re-interpretable container holding stats for either a player or enemy entity
	union EntityStats
	{
		//Unconstrained union fix
		EntityStats() {}
		EntityStats(const PlayerStats& pt) : m_PlayerStats(pt) {}
		EntityStats& operator=(const PlayerStats& pt) { new(&m_PlayerStats) PlayerStats(pt); return *this; }
		~EntityStats() {}

		PlayerStats m_PlayerStats;
		EnemyStats m_EnemyStats;
	};

	////////////
	/// Data ///
	////////////

	//Entity Stats
	EntityStats m_Stats;
	GeneralEntityData m_GenData;
	EnemyFlags m_EnemyFlags;

	//Almost everything needs world space coordinates, so add this to the baseline
	EM_Coordinates m_WorldCoordinates;
	//Entities Pool Data (if applicable)
	PoolIDs m_PoolData;
	//Some basic entity flags the child classes can use for control purposes
	EntityFlags m_Flags;

	//Collision Box/Radius Container
	ObjectCol m_CollisionBox;
	//Holding container for new velocity when collision occurs between entities
	DirectX::XMFLOAT2 m_NudgeDir = {0.f, 0.f};

	//ID relative to the purpose/uniqueness of the entity (i.e, 0 = enemy_type_00, 1 = enemy_type_01 etc)
	//Used in pool searching to avoid typeid checks
	unsigned m_ID = 0xFFFF;

	//Collision Control Flag
	bool m_CollisionCheck = true;
};

