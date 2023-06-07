#include "SM_GameStateManager.h"

void SM_GameStateManager::SetActiveGameState(ActiveGameState newState)
{
	//Store the current state as the old state
	m_PreviousGameState = m_ActiveGameState;
	//Update the state
	m_ActiveGameState = newState;
	//Flag that state change occured
	m_ActiveStateChanged = true;
}

void SM_GameStateManager::SetGamePrepState(GamePrepState newState)
{
	//Store the current state as the old state
	m_PreviousPrepState = m_GamePrepState;
	//Update the state
	m_GamePrepState = newState;
	//Flag that state changed occured
	m_PrepStateChanged = true;
}

void SM_GameStateManager::Init()
{

}

void SM_GameStateManager::Release()
{

}
