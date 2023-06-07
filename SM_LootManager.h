#pragma once

//Other types
#include "GameTypes.h"

//Forward Decs
class EM_EntityInterface2D;

/*
	Manager for all those loot goodies.
	Manages the collection of loot entities dropped during gametime.
*/
class SM_LootManager
{
public:

	///////////////////
	/// Definitions ///
	///////////////////


	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	SM_LootManager() { Init(); }
	~SM_LootManager() { Release(); }


	//////////////////
	/// Operations ///
	//////////////////

	//In the event of a unit death, process XP gem spawning and potential loot dropping
	void ProcessNewLootEvent(EM_EntityInterface2D* const ent);

	///////////
	/// Get ///
	///////////

	bool GetSuperActive() { return m_SuperActive;  }

	///////////
	/// Set ///
	///////////

	void SetSuperActive(bool active) { m_SuperActive = active; }

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

	bool m_SuperActive = false;
};