#include "Mode_SplashScreen.h"	//Owner

#include "Game.h"		//Manager Acccess

void Mode_SplashScreen::Enter()
{
	//Set the game state to start/splash screen
	m_GPManager->GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::START_SCREEN);
}

void Mode_SplashScreen::Update(GameTimer& gt)
{
}

void Mode_SplashScreen::Render(PassData& data)
{
	//Get Game
	Game* game = Game::GetGame();
	//Start Render Pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->Begin(
		game->GetCommandList().Get(),
		DirectX::SpriteSortMode_FrontToBack
	);
	switch (Game::GetGame()->GetGameplayManager().GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::START_SCREEN:
		m_Backgrounds[0]->Draw(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);
		m_TitleScreen->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	case SM_GameStateManager::AGS::CONTROLS:
		m_Backgrounds[1]->Draw(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], data.m_Heap);
		m_Controls->Render(*data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB], *data.m_Spritefonts[SpritefontDefinitions::CALIBRI_22_SF], data.m_Heap);
		break;
	}

	//End Render Pass
	data.m_Spritebatches[SpritebatchDefinitions::MAIN_SB]->End();
}

void Mode_SplashScreen::ProcessKey(char key)
{
	//Create upper'd copy of the key to align with virtual key codes (preserve original copy in case required)
	char nKey = std::toupper(key);

	switch (Game::GetGame()->GetGameplayManager().GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::START_SCREEN:
		m_TitleScreen->ProcessInput(nKey);
		break;
	case SM_GameStateManager::AGS::CONTROLS:
		m_Controls->ProcessInput(nKey);
		break;
	}

}

void Mode_SplashScreen::Release()
{
	delete m_Backgrounds[0];
	m_Backgrounds[0] = nullptr;

	delete m_Backgrounds[1];
	m_Backgrounds[1] = nullptr;

	delete m_TitleScreen;
	m_TitleScreen = nullptr;

	delete m_Controls;
	m_Controls = nullptr;
}

void Mode_SplashScreen::Init()
{
	//Get Game
	Game* game = Game::GetGame();
	m_GPManager = &game->GetGameplayManager();
	//Setup background
	m_Backgrounds[0] = new EM_Sprite();
	m_Backgrounds[0]->SetTexture("TitleScreen_00",true);
	m_Backgrounds[0]->SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	m_Backgrounds[1] = new EM_Sprite();
	m_Backgrounds[1]->SetTexture("Controls_00", true);
	m_Backgrounds[1]->SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	m_TitleScreen = new UI_TitleScreen();
	m_TitleScreen->InitialiseUI(std::string("data/files/UI_Config_00.json"), 9);

	m_Controls = new UI_Controls();
	m_Controls->InitialiseUI(std::string("data/files/UI_Config_00.json"), 13);
}
