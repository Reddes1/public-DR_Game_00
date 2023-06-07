#pragma once

#include "Projectile_Interface.h"	//Parent

/*
	Special projectile class being used with ability "Water Dragon".
*/
class Projectile_WaterDragon : public Projectile_Interface
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class StateID : unsigned
	{
		SPAWNING,
		ACTIVE,
		DESPAWNING,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_WaterDragon() { Init(); }
	~Projectile_WaterDragon() { Release(); }
 
	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
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

	StateID m_State = StateID::SPAWNING;

};