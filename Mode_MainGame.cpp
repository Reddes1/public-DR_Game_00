#include "Mode_MainGame.h"		//Owner

#include "Game.h"			//Manager Access

Mode_MainGameScene::Mode_MainGameScene(size_t id)
	:ModeInterface(id)
{
	Init();
}

void Mode_MainGameScene::Init()
{
	//Get Game
	Game* game = Game::GetGame();
	//Grab gameplay manager pointer
	m_GPManager = &game->GetGameplayManager();

	//Setup background
	m_Backgrounds.push_back(new EM_Sprite());
	m_Backgrounds[0]->SetTexture(DEFAULT_TEST_BG_TEXTURE_NAME, true);
	m_Backgrounds[0]->SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	std::string ui_fp = "data/files/UI_Config_00.json";

	//Allocate UIs
	m_LevelSelectorUI = new UI_LevelUpSelector();
	m_LevelSelectorUI->InitialiseUI(ui_fp, 0);
	m_PlayerInfo = new UI_PlayerInfo();
	m_PlayerInfo->InitialiseUI(ui_fp, 1);
	m_ExperienceBar = new UI_ExperienceBar();
	m_ExperienceBar->InitialiseUI(ui_fp, 2);
	m_PauseMenu = new UI_PauseMenu();
	m_PauseMenu->InitialiseUI(ui_fp, 5);
	m_PauseVolumeControl = new UI_PauseVolumeControl();
	m_PauseVolumeControl->InitialiseUI(ui_fp, 11);
	m_GameOver = new UI_GameOverMenu();
	m_GameOver->InitialiseUI(ui_fp, 6);
	m_WinGame = new UI_WinGame();
	m_WinGame->InitialiseUI(ui_fp, 12);
	m_PlayerFNP = new UI_FNP_Player();
	m_PlayerFNP->InitialiseUI(ui_fp, 10);

	//Bind this to the Progression System
	Game::GetGame()->GetGameplayManager().GetProgressManager().BindLevelSelectorUI(m_LevelSelectorUI);
}

void Mode_MainGameScene::Release()
{
	//Release backgrounds
	for (auto& a : m_Backgrounds)
	{
		delete a;
		a = nullptr;
	}

	//Release UIs
	delete m_LevelSelectorUI;
	m_LevelSelectorUI = nullptr;
	delete m_PlayerInfo;
	m_PlayerInfo = nullptr;
	delete m_ExperienceBar;
	m_ExperienceBar = nullptr;
	delete m_PauseMenu;
	m_PauseMenu = nullptr;
	delete m_GameOver;
	m_GameOver = nullptr;
	delete m_WinGame;
	m_WinGame = nullptr;
	delete m_PlayerFNP;
	m_PlayerFNP = nullptr;
	delete m_PauseVolumeControl;
	m_PauseVolumeControl = nullptr;
}

void Mode_MainGameScene::Enter()
{
	//Set gamestate
	m_GPManager->GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::GAME_ACTIVE);

	//Reset appropriate UIs/Managers etc
	m_GPManager->GetEntityManager().TurnOffAllPools();
	m_GPManager->GetProgressManager().ResetProgress();
	m_GPManager->GetSpawnManager().ResetCurrentSpawnPattern();

	//Bind UIs (must do here as managers aren't completely setup by init time)
	m_PlayerInfo->SetPlayerPointer(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer());
	m_PlayerFNP->SetPlayerPointer(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer());
	//Syncs player info frame with player inventory
	m_PlayerInfo->SyncUIWithPlayer();

	//Stop menu music and play game music based on the active map
	Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::MAIN_MENU)->Stop();

	if (m_GPManager->GetTileManager().GetActiveMapIndex() == (unsigned)MapIDs::GRASSLAND)
	{
		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::GRASSLAND)->Play(true);

		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::GRASSLAND)->SetVolume(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicAdjustedVolume()
		);
	}
	else if (m_GPManager->GetTileManager().GetActiveMapIndex() == (unsigned)MapIDs::CAVE)
	{
		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::CAVE)->Play(true);

		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::CAVE)->SetVolume(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicAdjustedVolume()
		);
	}
}

bool Mode_MainGameScene::Exit()
{
	return true;
}

void Mode_MainGameScene::Update(GameTimer& gt)
{
	
	//Update based on game state
	switch (m_GPManager->GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::GAME_ACTIVE:
		
		//Player Update
		m_GPManager->GetEntityManager().UpdatePlayer(gt);
		//Update Enemy Pool 00
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::ENTITY_00, gt);
		//Update Enemy Pool 01
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::ENTITIES_01_02_03, gt);
		//Update Player Proj Pool 00
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::PLAYER_PROJ, gt);
		//Update Enemy Proj Pool 00
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::ENEMY_PROJ, gt);
		//Update Loot Pool
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::LOOT, gt);
		//Update Effects Pool
		m_GPManager->GetEntityManager().UpdatePool(SM_EntityManager::PoolID::EFFECTS_00, gt);
		//Collision Update
		m_GPManager->GetCollisionManager().Update(gt);
		//Spawner Update
		m_GPManager->GetSpawnManager().Update(gt);
		//Update Progression Manager
		m_GPManager->GetProgressManager().Update();
		//Update FloatingCombatText
		m_GPManager->GetFCTManager().Update(gt);		
		//Update UI
		m_PlayerInfo->Update(gt);
		m_PlayerFNP->Update(gt);
		m_ExperienceBar->Update(gt);
		
		break;

	case SM_GameStateManager::AGS::PAUSE_SCREEN_OPEN:
		break;

	case SM_GameStateManager::AGS::LEVEL_SELECTION_OPEN:

		break;

	case SM_GameStateManager::AGS::LOOT_SCREEN_OPEN:

		break;
	case SM_GameStateManager::AGS::LOST_STATE_PLAYER_KILLED:

		break;
	case SM_GameStateManager::AGS::PAUSE_VOLUME_CONTROL:
		m_PauseVolumeControl->Update();
		break;
	case SM_GameStateManager::AGS::WIN_STATE_GAME_COMPLETE:
		break;
	default:
		msg_assert(false, "Update(): Invalid GameState Set!");
	}

}

void Mode_MainGameScene::Render(PassData& data)
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

	//Start pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack
	);

	//Render backgrounds
	for (auto& a : m_Backgrounds)
		a->Draw(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//End pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();

	////////////////////////////////////
	///	 Render Pass 01 			 ///
	///	 Scene_00 (Terrain)			 ///
	///  Camera Adjusted + PW Shader ///
	////////////////////////////////////

	//Start pass
	data.m_Spritebatches[SpritebatchDefinitions::POINT_WRAP_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack,
		game->GetGameplayManager().GetCamera().GetSceneTransform()
	);

	//Render Tiles
	m_GPManager->GetTileManager().Render(*data.m_Spritebatches[SpritebatchDefinitions::POINT_WRAP_SB], data.m_Heap);

	//End pass
	data.m_Spritebatches[SpritebatchDefinitions::POINT_WRAP_SB]->End();

	/////////////////////////////
	///	 Render Pass 02 	  ///
	///	 Scene_00 (Enemies)	  ///
	///  Camera Adjusted	  ///
	/////////////////////////////

	//Start pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack,
		game->GetGameplayManager().GetCamera().GetSceneTransform()
	);

	//Render Enemy Pool 00
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::ENTITY_00,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render Enemy Pool 01
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::ENTITIES_01_02_03,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render Enemy Pool 01
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::EFFECTS_00,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render Loot Pool
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::LOOT,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render Player Projectiles 00
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::PLAYER_PROJ,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render Enemy Projectiles 00
	m_GPManager->GetEntityManager().RenderPool(SM_EntityManager::PoolID::ENEMY_PROJ,
		*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);


	//End pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();

	////////////////////////////////
	///  Render Pass 03		     ///
	///	 Scene_02 (Projectiles)	 ///
	///  Camera Adjusted         ///
	////////////////////////////////

	//Start pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack,
		game->GetGameplayManager().GetCamera().GetSceneTransform()
	);


	//Render Floating Combat Text
	m_GPManager->GetFCTManager().Render();

	//End pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();

	///////////////////////////////
	///  Render Pass 04	        ///
	///	 Scene_03 (Contextual &	///
	///  Non-Contextual UIs)    ///
	///////////////////////////////

	//Start pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack
	);

	//Render player
	m_GPManager->GetEntityManager().RenderPlayer(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);

	//Render UI Elements
	switch (m_GPManager->GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::GAME_ACTIVE:
		break;

	case SM_GameStateManager::AGS::PAUSE_SCREEN_OPEN:
		m_PauseMenu->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[FontDescriptions::EXPRESSION_PRO_20], data.m_Heap);
		break;

	case SM_GameStateManager::AGS::PAUSE_VOLUME_CONTROL:
		m_PauseVolumeControl->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[FontDescriptions::EXPRESSION_PRO_20], data.m_Heap);
		break;

	case SM_GameStateManager::AGS::LEVEL_SELECTION_OPEN:
		m_LevelSelectorUI->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[FontDescriptions::EXPRESSION_PRO_20], data.m_Heap);
		break;

	case SM_GameStateManager::AGS::LOOT_SCREEN_OPEN:

		break;
	case SM_GameStateManager::AGS::LOST_STATE_PLAYER_KILLED:
		m_GameOver->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[FontDescriptions::EXPRESSION_PRO_20], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::WIN_STATE_GAME_COMPLETE:
		m_WinGame->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[FontDescriptions::EXPRESSION_PRO_20], data.m_Heap);
		break;
	default:
		msg_assert(false, "Render(): Invalid GameState Set!");
	};

	//Main Gameplay UIs
	m_PlayerInfo->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);
	m_PlayerFNP->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);
	m_ExperienceBar->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);


	//End pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();
}

void Mode_MainGameScene::ProcessKey(char key)
{
	//Create upper'd copy of the key to align with virtual key codes (preserve original copy in case required)
	char nKey = std::toupper(key);

	switch (m_GPManager->GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::GAME_ACTIVE:
		PK_GameActive(nKey);
		break;

	case SM_GameStateManager::AGS::PAUSE_SCREEN_OPEN:
		PK_PauseScreen(nKey);
		break;

	case SM_GameStateManager::AGS::LEVEL_SELECTION_OPEN:
		PK_LevelSelection(nKey);
		break;

	case SM_GameStateManager::AGS::LOOT_SCREEN_OPEN:
		PK_LootScreen(nKey);
		break;
	case SM_GameStateManager::AGS::LOST_STATE_PLAYER_KILLED:
		m_GameOver->ProcessInput(nKey);
		break;
	case SM_GameStateManager::AGS::PAUSE_VOLUME_CONTROL:
		m_PauseVolumeControl->ProcessInput(nKey);
		break;
	case SM_GameStateManager::AGS::WIN_STATE_GAME_COMPLETE:
		m_WinGame->ProcessInput(nKey);
		break;
	default:
		msg_assert(false, "ProcessKey(): Invalid GameState Set!");
	};
}

void Mode_MainGameScene::PK_GameActive(char key)
{
	switch (key)
	{
	case (char)CS_Survivor_00::DMP::BACK:

		m_PauseMenu->SetStatNumbers();
		m_GPManager->GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::PAUSE_SCREEN_OPEN);
		break;
	}

	if (m_DebugToolsEnabled)
	{
		switch (key)
		{
		case 'L':
		case 'l':
			m_GPManager->GetProgressManager().ForceLevelUp();
			break;

		case 'O':
		case 'o':
			m_GPManager->GetSpawnManager().AdvanceWaveByOne();
			break;
		}
	}	
}

void Mode_MainGameScene::PK_PauseScreen(char key)
{
	m_PauseMenu->ProcessInput(key);
}

void Mode_MainGameScene::PK_LevelSelection(char key)
{
	//If selection is made, switch back to active game
	if (m_LevelSelectorUI->ProcessInput(key) == UI_LevelUpSelector::Actions::SELECTION_MADE)
	{
		m_GPManager->GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::GAME_ACTIVE);
		m_PlayerInfo->SyncUIWithPlayer();
	}
}

void Mode_MainGameScene::PK_LootScreen(char key)
{
}


