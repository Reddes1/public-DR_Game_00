#pragma once

#include <list>

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"	

//Other types
#include "GameTypes.h"
#include "GameTimer.h"

//#include "SM_EntityManager.h"
#include "Entity_Player.h"
#include "EntityPool.h"

//Forward Dec
class EM_EntityInterface2D;

/*
	Base Loot Class for game. Hosts core data for loot drops and virtuals to define behaviour
	of the loot.
*/
class Loot_Interface
{
public:

	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	Loot_Interface() { Init(); }
	virtual ~Loot_Interface() { }

	////////////////
	/// Virtuals ///
	////////////////

	//Core Update Function
	virtual void Update(GameTimer& gt) = 0;
	//Core Render Function
	virtual void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);

	//Define any internal functionality that needs reseting between uses (like control flags, states etc).
	virtual bool Reset() = 0;
	


	//////////////////
	/// Operations ///
	//////////////////

	//
	//Initialisation Processes (Call one in each category ONCE for each time the projectile is setup)
	//

	//Initialises the visual details of the projectile (setting texture & fixed frame).
	void InitialiseVisuals(std::string& texName, unsigned frame);

	//Initialises position
	void InitialisePosition(DirectX::XMFLOAT2& pos);

	//Setup the entity with pool information (its pool and position in pool).
	//This is used later when the entity flags itself as free for use, and should only be called ONCE.
	void InitPoolData(unsigned short poolID, unsigned short poolIndex);

	//Process how the loot should manage collision with the player
	virtual void ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00);

	void SetFollowPlayer(bool follow) { m_FollowPlayer = follow; }
	bool GetFollowPlayer() { return m_FollowPlayer;; }

	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }

	//Returns copy, not actual (Should never be edited during run-time)
	PoolIDs& GetPoolData() { return m_PoolData; }
	ProjectileStats& GetStats() { return m_Stats; }

	const ObjectCol GetCollisionData() { return m_ColisionBox; }

	unsigned GetID() { return m_LootID; }

	int GetPassingValue() { return m_PassedValue; }

	bool GetCollisionCheck() { return m_CollisionCheck; }

	///////////
	/// Set ///
	///////////

	void SetID(unsigned newID) { m_LootID = newID; }

	void SetCollisionData(CollisionArea collider) { m_ColisionBox.m_CollisonBox = collider; }
	void SetCollisionLayer(CollisionLayers layer) { m_ColisionBox.m_CollisonLayer = layer; }

	void SetPassingValue(int value) { m_PassedValue = value; }

	void SetCollisionFlag(bool coll) { m_CollisionCheck = coll; }

private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	////////////
	/// Data ///
	////////////
	
	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;

	//Stats
	ProjectileStats m_Stats;

	//Pool Tracking
	PoolIDs m_PoolData;
	unsigned m_LootID = 0;

	//Directional Vector for movement (wrap this into a module later)
	DirectX::SimpleMath::Vector2 m_DirVel = { 0, 0 };

	//Collision Area
	//CollisionArea m_ColisionBox = { 0.f, 0.f, 0.f, 0.f };
	
	//Collision Box/Radius Container
	ObjectCol m_ColisionBox;

	// Stored Value
	int m_PassedValue = 0;

	// Follow Player Check
	bool m_FollowPlayer = false;

	//Collison Flag
	bool m_CollisionCheck = true;
};