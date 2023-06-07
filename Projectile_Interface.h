#pragma once

#include <list>

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"	

//Other types
#include "GameTypes.h"
#include "GameTimer.h"

//Forward Dec
class EM_EntityInterface2D;

/*
	Base Projectile Class for game. Hosts core data for projectiles and virtuals to define behaviour
	of the projectile.
*/
class Projectile_Interface
{
public:

	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Interface() { Init(); }
	virtual ~Projectile_Interface() { }

	////////////////
	/// Virtuals ///
	////////////////

	//Core Update Function
	virtual void Update(GameTimer& gt) = 0;
	//Core Render Function
	virtual void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);

	//Define any internal functionality that needs reseting between uses (like control flags, states etc).
	virtual bool Reset() = 0;

	//
	//Combat
	// 
	
	//Process how the projectile should manage collision with an entity (provides hitlist functionality)
	virtual bool ProcessCollisionEvent(EM_EntityInterface2D* const ent) { return false; }
	//Process how the projectile should manage collision with another projectile entity
	virtual bool ProcessCollisionEvent(Projectile_Interface* const proj) { return false; }

	//////////////////
	/// Operations ///
	//////////////////

	//
	//Initialisation Processes (Call one in each category ONCE for each time the projectile is setup)
	//

	//Initialises the visual details of the projectile (setting texture & fixed frame).
	void InitialiseVisuals(std::string& texName, unsigned frame);
	//Initialises the visual details of the projectile (setting texture, anim and anim details).
	void InitialiseVisuals(std::string& texName, unsigned animID, bool play, bool loop, bool reverse);

	//Initialises position
	void InitialisePosition(DirectX::XMFLOAT2& pos);
	//Initialises position & velocity
	void InitialisePositionAndVelocity(DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2& vel);

	//Setup the entity with pool information (its pool and position in pool).
	//This is used later when the entity flags itself as free for use, and should only be called ONCE.
	void InitPoolData(unsigned short poolID, unsigned short poolIndex);

	//Checks entity against list returns flag based on resolution. 
	//Additionally resets time/flag on ReHit period if entity would push size of list over maximum
	bool IsEntityInHitList(EM_EntityInterface2D* const ent);
	//Updates hitlist behaviour
	void UpdateHitList(GameTimer& gt);
	//Resets all hit list variables/container
	void ResetHitlist();

	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }


	PoolIDs& GetPoolData() { return m_PoolData; }
	ProjectileStats& GetStats() { return m_Stats; }
	GeneralEntityData& GetGeneralData() { return m_GenData; }
	DirectX::SimpleMath::Vector2& GetVelocity() { return m_DirVel; }

	//For Pool Usage
	unsigned GetID() { return m_WeaponID; }
	unsigned GetWeaponID() { return GetID(); }
	const CollisionArea GetCollisionData() { return m_CollisionBox; }
	bool GetCollisionCheck() { return m_CollisionCheck; }
	
	///////////
	/// Set ///
	///////////

	//Sets weapon ID (Use WeaponTypeID enum, and ideally only set ONCE, unless resetting to baseline)
	void SetWeaponID(unsigned newID) { m_WeaponID = newID; }

	void SetCollisionActive (DirectX::SimpleMath::Vector2 scale, bool collision) { m_CollisionCheck = collision; m_BaseSprite.SetScale(scale); }
	void SetCollisionFlag(bool coll) { m_CollisionCheck = coll; }
	void SetCollisionData(CollisionArea collider) { m_CollisionBox = collider; }

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

	//List for tracking the enemies hit by this projectile
	std::list<EM_EntityInterface2D*> m_HitList;
	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;
	//Stats
	ProjectileStats m_Stats;
	GeneralEntityData m_GenData;
	//Collision Data(Position + Rect/Circle Values)
	CollisionArea m_CollisionBox = { 0.f, 0.f, 0.f, 0.f };
	//Pool Tracking
	PoolIDs m_PoolData;
	//Directional Vector for movement (wrap this into a module later)
	DirectX::SimpleMath::Vector2 m_DirVel = { 0, 0 };
	//ID for finding in shared pool (This is relative to the WeaponIDType enum)
	unsigned m_WeaponID = 0;
	//Elapsed timer for hit (ticks up and checked against m_ReHit in projectile data)
	float m_HLElapsed = 0.f;
	//Control flag for if hit timer is active
	bool m_HLTimerActive = false;
	//Collison Flag
	bool m_CollisionCheck = true;
};