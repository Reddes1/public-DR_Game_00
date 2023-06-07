#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Behaviour class for one of several player projectiles.
	Spawns pillars of lights that deal damage around the origin point (near the bottom of the sprite).
	Used with Heaven-on-High Ability.
*/
class Projectile_HolyBeam : public Projectile_Interface
{
public:

	////////////
	/// Data ///
	////////////

	static constexpr unsigned HITBOX_END_FRAME_DESPAWN = 6;

	///////////////////
	/// Definitions ///
	///////////////////

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_HolyBeam() { Init(); }
	~Projectile_HolyBeam() { Release(); }

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

};



