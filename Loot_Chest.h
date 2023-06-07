#pragma once
#include "Loot_Interface.h"

/*
	Magnet Object for collecting all Experience objects in the scene.
*/
class Loot_Chest : public Loot_Interface
{
public:

	////////////
	/// Data ///
	////////////



	//////////////////
	/// Defintions ///
	//////////////////

	enum class StateID : unsigned
	{
		STABLE,
		DESPAWNING,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Loot_Chest() { Init(); }
	~Loot_Chest() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this loot object is a normalised state (if required)
	bool Reset() override;

	void ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00) override;

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
	
	StateID m_State = StateID::STABLE;
	float m_Elapsed = 1.f;
};