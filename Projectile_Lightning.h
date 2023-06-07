#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Cross".
*/
class Projectile_Lightning : public Projectile_Interface
{
public:


	//////////////////
	/// Defintions ///
	//////////////////

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Lightning() { Init(); }
	~Projectile_Lightning() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Main Update Window
	void Update(GameTimer& gt) override;
	//Main Render Window
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this weapon is a normalised state (if required)
	bool Reset() override;


	//////////////////
	/// Operations ///
	//////////////////

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

	float localRange = 10.f;

};