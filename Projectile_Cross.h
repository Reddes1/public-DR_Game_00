#pragma once
#include "Projectile_Interface.h"		//Parent

/*
	Player projectile for player weapon "Cross".
*/
class Projectile_Cross : public Projectile_Interface
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

	Projectile_Cross() { Init(); }
	~Projectile_Cross() { Release(); }

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

	//
	//Update Stages
	//

	//Update focused on the behaviour of the weapon (Decel-to-Accel phases)
	void UpdateBehaviour(GameTimer& gt);
	//Update focused on updating the position of the projectile
	void UpdatePosition(GameTimer& gt);
	//Update focusing on the termination conditions for this projectile
	void TerminationUpdate();


	////////////
	/// Data ///
	////////////

	//Elapsed tracking for managing EaseIn/EaseOut duration
	float m_Elapsed = 0.0f;
	//The current speed of projectile
	float m_CurrentSpeed = 0.f;
	//Acceleration/Deceleraton time
	float m_Duration = 1.5f;
	//Damage dealt from impact
	float m_Damage = 10.f;
	//Speed post decel phase
	float m_OldSpeed = 0.f;
	//Flag checks what state the weapon should act under
	struct
	{
		bool m_StageOneDone : 1;
		bool m_BehaviourDone : 1;
	} Flags;

};
