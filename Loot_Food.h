#pragma once
#include "Loot_Interface.h"

/*
	Health Object for giving the character health in dire situations.
*/
class Loot_Food : public Loot_Interface
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

	Loot_Food() { Init(); }
	~Loot_Food() { Release(); }

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

	unsigned m_Health = 25;

};