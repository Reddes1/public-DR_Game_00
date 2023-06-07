#include "Mode_MainMenu.h"	//Owner

void Mode_MainMenu::Enter()
{
	//set the game state to main menu open
	m_GPManager->GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::MAIN_MENU_OPEN);

	//Stop any main game audio and start playing menu audio
	Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::GRASSLAND)->Stop();
	Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::CAVE)->Stop();
	Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::MAIN_MENU)->Play(true);
}

void Mode_MainMenu::Update(GameTimer& gt)
{
	Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::MAIN_MENU)->SetVolume(
		Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicAdjustedVolume()
	);
}

void Mode_MainMenu::Render(PassData& data)
{
	//Get Game
	Game* game = Game::GetGame();

	//
	//Non-Contextual Renders (Ignores Game State)
	//

	////////////////////////
	///  Render Pass 00  ///
	///  Backgrounds	 ///
	////////////////////////


	///////////////////////////////
	///  Render Pass 01	        ///
	///	 Scene_00 (Contextual &	///
	///  Non-Contextual UIs)    ///
	///////////////////////////////

		//Start Render Pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack
	);

	//Render based on game state
	switch (m_GPManager->GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::MAIN_MENU_OPEN:
		m_MainMenuUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::VOLUME_CONTROLS:
		m_VolumeControlsUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::CONTROLS:
		m_MainMenuUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::CHARACTER_SELCTION:
		m_CharacterSelectUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::MAP_SELECTION:
		m_MapSelectionUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	default:
		msg_assert(false, "Render(): Invalid GameState Set!");
	};

	//End Render Pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();

}

void Mode_MainMenu::ProcessKey(char key)
{
	//Create upper'd copy of the key to align with virtual key codes (preserve original copy in case required)
	char nKey = std::toupper(key);

	switch (m_GPManager->GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::MAIN_MENU_OPEN:
	case SM_GameStateManager::AGS::CONTROLS:
		m_MainMenuUI->ProcessInput(nKey);
		break;
	case SM_GameStateManager::AGS::VOLUME_CONTROLS:
		m_VolumeControlsUI->ProcessInput(nKey);
		break;
	case SM_GameStateManager::AGS::CHARACTER_SELCTION:
		m_CharacterSelectUI->ProcessInput(nKey);
		break;

	case SM_GameStateManager::AGS::MAP_SELECTION:
		m_MapSelectionUI->ProcessInput(nKey);
		break;

	default:
		msg_assert(false, "ProcessKey(): Invalid GameState Set!");
	};
}

void Mode_MainMenu::Release()
{
	delete m_MainMenuUI;
	m_MainMenuUI = nullptr;

	delete m_MapSelectionUI;
	m_MapSelectionUI = nullptr;

	delete m_CharacterSelectUI;
	m_CharacterSelectUI = nullptr;

	delete m_VolumeControlsUI;
	m_VolumeControlsUI = nullptr;
}

void Mode_MainMenu::Init()
{
	//Get Game
	Game* game = Game::GetGame();
	m_GPManager = &game->GetGameplayManager();

	//Allocate UI

	//Main Menu
	m_MainMenuUI = new UI_MainMenu();
	m_MainMenuUI->InitialiseUI(std::string("data/files/UI_Config_00.json"), 3);
	m_MainMenuUI->SetMaxIndex(MAX_MAIN_MENU_INDEXES);
	//Map Selection
	m_MapSelectionUI = new UI_MapSelection();
	m_MapSelectionUI->InitialiseUI(std::string("data/files/UI_Config_00.json"), 4);
	//Character Selection
	m_CharacterSelectUI = new UI_CharacterSelect();
	m_CharacterSelectUI->InitialiseUI(std::string("data/files/UI_Config_00.json"), 
		std::string("data/files/Character_Data_00.json"), 
		std::string("data/files/Weapon_Data_00.json"), 7);
	//Volume Control
	m_VolumeControlsUI = new UI_VolumeControls();
	m_VolumeControlsUI->InitialiseUI(std::string("data/files/UI_Config_00.json"), 8);
}
