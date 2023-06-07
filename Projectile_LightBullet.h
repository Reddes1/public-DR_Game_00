#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Behaviour class for one of several player projectiles.
	Primary behaviour is straight line movement, with direction dictated by player weapon.
	Spawned by Bullet Hell Ability.
*/
class Projectile_LightBullet : public Projectile_Interface
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_LightBullet() { Init(); }
	~Projectile_LightBullet() { Release(); }

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
};

