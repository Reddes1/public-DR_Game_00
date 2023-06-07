#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Whip".
*/
class Projectile_Super : public Projectile_Interface
{
public:

	////////////
	/// Data ///
	////////////



	//////////////////
	/// Defintions ///
	//////////////////



	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Super() { Init(); }
	~Projectile_Super() { Release(); }

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

