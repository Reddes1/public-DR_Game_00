#pragma once
#include "Loot_Interface.h"

/*
	Magnet Object for collecting all Experience objects in the scene.
*/
class Loot_Wipe : public Loot_Interface
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

	Loot_Wipe() { Init(); }
	~Loot_Wipe() { Release(); }

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

	float m_ScaleUp = 50.f;

	bool m_Active = false;

	short m_Damage = 999;
};