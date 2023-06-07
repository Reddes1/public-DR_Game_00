#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Hex Steppers".
*/
class Projectile_HexStep: public Projectile_Interface
{
public:


	//////////////////
	/// Defintions ///
	//////////////////

	//Describes current state
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

	Projectile_HexStep() { Init(); }
	~Projectile_HexStep() { Release(); }

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

	//Current internal state
	StateID m_State = StateID::SPAWNING;
};