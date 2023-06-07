#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Cross".
*/
class Projectile_Bomb : public Projectile_Interface
{
public:

	//////////////////
	/// Defintions ///
	//////////////////

	//Describes the stages of the weapon from spawn, running, and despawning
	enum class StateID
	{
		STABLE,
		EXPLODING,
		EXPLODED,
		COUNT
	};


	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Bomb() { Init(); }
	~Projectile_Bomb() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this weapon is a normalised state (if required)
	bool Reset() override;

	//
	//Combat
	//

	bool ProcessCollisionEvent(EM_EntityInterface2D* const ent) override { return false; };
	bool ProcessCollisionEvent(Projectile_Interface* const proj) override { return false; };

	//////////////////
	/// Operations ///
	//////////////////

	//
	//See Parent for Standardised init functions
	//
	

	///////////
	/// Get ///
	///////////



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

	////////////
	/// Data ///
	////////////

	//Elapsed tracking for managing Bomb Explosion Interval
	float m_Elapsed = 0.0f;

	//Active State
	StateID m_State = StateID::STABLE;
};