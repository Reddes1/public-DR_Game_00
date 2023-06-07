#pragma once
#include "ModeInterface.h"	//Parent

//Entities
#include "Entity_EnemyType_00.h"
#include "AnimationDefinitions.h"

//Managers
#include "GameplayManager.h"

//UIs
#include "UI_LevelUpSelector.h"
#include "UI_ExperienceBar.h"
#include "UI_PlayerInfo.h"
#include "UI_PauseMenu.h"
#include "UI_GameOverMenu.h"
#include "UI_WinGame.h"
#include "UI_PauseVolumeControl.h"
#include "FNP_Player.h"

/*
	Main game state that the game will be primarily played in.
*/
class Mode_MainGameScene : public ModeInterface
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	Mode_MainGameScene(size_t id);
	~Mode_MainGameScene() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	void Enter() override;
	bool Exit() override;
	void Update(GameTimer& gt) override;
	void Render(PassData& data) override;
	void ProcessKey(char key) override;
	void Release() override;

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

	void Init();

	//
	//Process Key
	//

	void PK_GameActive(char key);
	void PK_PauseScreen(char key);
	void PK_LevelSelection(char key);
	void PK_LootScreen(char key);

	////////////
	/// Data ///
	////////////

	//Hold large, scene/background sprites here
	std::vector<EM_Sprite*> m_Backgrounds;

	//Hold reference to gameplay manager for quicker access
	GameplayManager* m_GPManager = nullptr;
	CS_Main_00* m_CS = nullptr;

	//Debugging flag
	bool m_DebugToolsEnabled = true;

	//////////////////
	/// Custom UIs ///
	//////////////////

	UI_LevelUpSelector* m_LevelSelectorUI = nullptr;
	UI_PlayerInfo* m_PlayerInfo = nullptr;
	UI_ExperienceBar* m_ExperienceBar = nullptr;
	UI_PauseMenu* m_PauseMenu = nullptr;
	UI_PauseVolumeControl* m_PauseVolumeControl = nullptr;
	UI_GameOverMenu* m_GameOver = nullptr;
	UI_WinGame* m_WinGame = nullptr;
	UI_FNP_Player* m_PlayerFNP = nullptr;
};