#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Whip".
*/
class Projectile_Whip : public Projectile_Interface
{
public:

	////////////
	/// Data ///
	////////////

	static constexpr unsigned HITBOX_END_FRAME = 7;


	//////////////////
	/// Defintions ///
	//////////////////

	enum class StateID : unsigned
	{
		INACTIVE,
		ACTIVE,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Whip() { Init(); }
	~Projectile_Whip() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

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

	//Current internal state
	StateID m_State = StateID::ACTIVE;
};

