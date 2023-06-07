#pragma once

//Sub-Managers
#include "SM_TilemapManager.h"
#include "SM_GameStateManager.h"
#include "SM_CombatManager.h"
#include "SM_EntityManager.h"
#include "SM_CollisionManager.h"
#include "SM_SpawnManager.h"
#include "SM_LootManager.h"
#include "SM_DataManager.h"
#include "SM_FloatingCombatText.h"

//Other
#include "SM_ProgressionManager.h"
#include "CameraMovement.h"
#include "CS_Main_00.h"				//Main Game Control Scheme

/*
	Centralises all gameplay orientated behaviours and modules together.
	If possible, any behaviour that can be wrapped up into tidy classes should be added here,
	and getters added to access them. This will effectively act as a hub type class, but
	specifically for gameplay related tasks and actions, without cluttering this hub.
*/
class GameplayManager
{
public:

	////////////
	/// Data ///
	////////////


	////////////////////
	/// Constructors ///
	////////////////////

	GameplayManager() {}
	~GameplayManager() {}

	//////////////////
	/// Operations ///
	//////////////////

	//
	// Composite Functions
	//

	///////////
	/// Get ///
	///////////

	SM_EntityManager& GetEntityManager() { return m_EntityManager; }
	SM_CollisionManager& GetCollisionManager() { return m_CollisionManager; }
	SM_LootManager& GetLootManager() { return m_LootManager; }
	SM_SpawnManager& GetSpawnManager() { return m_SpawnManager; }
	SM_TilemapManager& GetTileManager() { return m_TileManager; }
	SM_GameStateManager& GetStateManager() { return m_StateManager; }
	SM_CombatManager& GetCombatManager() { return m_CombatManager; }
	SM_DataManager& GetDataManager() { return m_DataManager; }
	SM_FloatingCombatText& GetFCTManager() { return m_FCTManager; }

	CameraMovement& GetCamera() { return m_CameraManager; }
	CS_Interface& GetControlScheme() { return m_ControlScheme; }
	SM_ProgressionManager& GetProgressManager() { return m_ProgressionManager; }

	///////////
	/// Set ///
	///////////


private:

	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//Main game entity hub
	SM_EntityManager m_EntityManager;

	//Entity Collision Interface
	SM_CollisionManager m_CollisionManager;

	//Loot Manager
	SM_LootManager m_LootManager;

	//Entity Spawn Manager
	SM_SpawnManager m_SpawnManager;

	//Responsible for managing games tilemap
	SM_TilemapManager m_TileManager;

	//Responsible for tracking and managing program/game state for use elsewhere
	SM_GameStateManager m_StateManager;

	//Responsible for manage damage calculations, and setting unit states post combat
	SM_CombatManager m_CombatManager;

	//Camera class that simulates a camera in the 2D spaces by manipulating the position of the scene
	//during draw calls.
	CameraMovement m_CameraManager;

	//Main control scheme
	CS_Main_00 m_ControlScheme;

	//Stores original copies of character, weapon and enemy entity information
	SM_DataManager m_DataManager;

	//Player level/ progression manager
	SM_ProgressionManager m_ProgressionManager;

	//Housing manager for floating combat text
	SM_FloatingCombatText m_FCTManager;
};


