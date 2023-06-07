#pragma once

/*
	Simple sub-manager for defining and tracking any number of states for program control in game modes
	or other managers.
*/
class SM_GameStateManager
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	/*
		Defines the states for the active game state.
	*/
	enum class ActiveGameState : unsigned short
	{
		////////////////////////
		/// Main Menu States ///
		////////////////////////

		START_SCREEN,
		CONTROLS,
		MAIN_MENU_OPEN,
		OPTIONS_OPEN,
		CHARACTER_SELCTION,
		MAP_SELECTION,
		VOLUME_CONTROLS,
		///////////////////////
		/// Gameplay States ///
		///////////////////////

		GAME_ACTIVE,
		LEVEL_SELECTION_OPEN,
		PAUSE_SCREEN_OPEN,
		PAUSE_VOLUME_CONTROL,
		LOOT_SCREEN_OPEN,

		////////////////////////////
		/// Win Condition States ///
		////////////////////////////

		WIN_STATE_GAME_COMPLETE,
		LOST_STATE_PLAYER_KILLED,
		STATE_COUNT
	};
	enum class GamePrepState : unsigned short
	{
		STATE_COUNT
	};

	typedef ActiveGameState AGS;
	typedef GamePrepState GPS;

	////////////////////
	/// Constructors ///
	////////////////////

	SM_GameStateManager() { Init(); }
	~SM_GameStateManager() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////


	///////////
	/// Get ///
	///////////

	ActiveGameState GetActiveGameState()			  { return m_ActiveGameState; }
	ActiveGameState GetPreviousActiveGameState()	  { return m_PreviousGameState; }

	GamePrepState GetGamePrepState()				  { return m_GamePrepState; }
	GamePrepState GetPreviousGamePrepState()		  { return m_PreviousPrepState; }

	//Checks to see if a state change occured. This should be called in the active modes update cycle.
	//If update flag returns true, the game mode should do with that information what they need then flag it as false when done.
	bool CheckForActiveStateChange()				  { return m_ActiveStateChanged; }
	bool CheckForPrepStateChange()					  { return m_PrepStateChanged; }


	///////////
	/// Set ///
	///////////

	void SetActiveGameState(ActiveGameState newState); 
	void SetGamePrepState(GamePrepState newState);

	//Should be called and set true following confirmation on state change by modes
	void DisabledActiveStateChangeFlag()			 { m_ActiveStateChanged = false; }
	void DisablePrepStateChangeFlag()				 { m_PrepStateChanged = false; }


private:

	//////////////////
	/// Operations ///
	//////////////////

	void Init();
	void Release();
	
	////////////
	/// Data ///
	////////////

	ActiveGameState m_ActiveGameState = ActiveGameState::START_SCREEN;
	ActiveGameState m_PreviousGameState = ActiveGameState::STATE_COUNT;

	GamePrepState m_GamePrepState = GamePrepState::STATE_COUNT;
	GamePrepState m_PreviousPrepState = GamePrepState::STATE_COUNT;

	//Update flags to let modes know that a state change happened, and they may do stuff with that information
	bool m_ActiveStateChanged = false;
	bool m_PrepStateChanged = false;
};