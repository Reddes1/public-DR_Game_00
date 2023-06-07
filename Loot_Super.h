#pragma once
#include "Loot_Interface.h"

/*
	Magnet Object for collecting all Experience objects in the scene.
*/
class Loot_Super : public Loot_Interface
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

	Loot_Super() { Init(); }
	~Loot_Super() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this loot object is a normalised state (if required)
	bool Reset() override;


	//////////////////
	/// Operations ///
	//////////////////

	void ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00) override;

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

	// Stats for the Super Projectile Weapon
	WeaponStats m_SuperStats;
	
	// Timer for projectile firing
	float m_Timer = 0.f;
	float m_Time = 4.f;
	float m_Interval = 0.f;
	float m_InveralTime = 0.05f;

	// Range of the projectile spread
	float m_SpreadRamge = 30.f;

	// Active check for if being used
	bool m_Active = false;
	int m_SoundDelay = 0;
};