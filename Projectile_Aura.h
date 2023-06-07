#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Barrier Aura".
*/
class Projectile_Aura : public Projectile_Interface
{
public:


	//////////////////
	/// Defintions ///
	//////////////////

	//Describes current state
	enum class StateID : unsigned
	{
		INACTIVE,
		ACTIVE,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_Aura() { Init(); }
	~Projectile_Aura() { Release(); }

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

	//Track duration of time that aura is inactive, checking against lifetime (surrogate for CD)
	float m_Elapsed = 0.f;

	//Current internal state
	StateID m_State = StateID::ACTIVE;
};