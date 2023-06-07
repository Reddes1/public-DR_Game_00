#pragma once
#include "Projectile_Interface.h"		//Parent

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"		

/*
	Projectile weapon that rotates around the player, expanding and contracting on timers
*/
class Projectile_Book : public Projectile_Interface
{
public:

	//////////////////
	/// Defintions ///
	//////////////////

	//Describes the stages of the weapon from spawn, running, and despawning
	enum class StateID
	{
		EXPAND,
		STABLE,
		CONTRACT,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Book() { Init(); }
	~Projectile_Book() { Release(); }

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

	///////////
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

	//Set core functionality stats in one go
	void SetFunctionalityData(float angle, float offsetX, float offsetY);

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

	float m_Angle = 0.f;
	float m_OffsetX = 150.f;
	float m_OffsetY = -150.f;

	//Interpolation holding value, to move the weapons in and out when spawning/despawning
	float m_ElapsedInter = 0.f;
	float m_PosMul = 0.f;

	StateID m_State = StateID::EXPAND;
};