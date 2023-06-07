#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Behaviour class for one of several player projectiles. 
	Primary behaviour is an arc-based movement pattern.
*/
class Projectile_Axe : public Projectile_Interface
{
public:

	////////////
	/// Data ///
	////////////



	//////////////////
	/// Defintions ///
	//////////////////

	//Describes the stages of the weapon from spawn, running, and despawning
	enum class StateID
	{
		ACCELERATING,
		FULL_SPEED,
		COUNT
	};


	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Axe() { Init(); }
	~Projectile_Axe() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this weapon to a normalised state (if required)
	bool Reset() override;

	//
	//Combat
	//

	bool ProcessCollisionEvent(EM_EntityInterface2D* const ent) override;
	bool ProcessCollisionEvent(Projectile_Interface* const proj) override;

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

	//Interpolation elapsed time
	float m_Elapsed = 0.f;
	//Y Start point
	float m_VelYStart = -1.f;
	//Flag to capture vel once
	bool m_VelCaptured = false;

	//Track current internal state
	StateID m_State = StateID::ACCELERATING;
};

