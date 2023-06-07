#pragma once
#include "ModeInterface.h"	//Parent Class

#include "Game.h"			//Manager Access + Utils
#include "GameplayManager.h" 
//UI 
#include "UI_MainMenu.h"
#include "UI_MapSelection.h"
#include "UI_CharacterSelect.h"
#include "UI_VolumeControls.h"

class Mode_MainMenu : public ModeInterface
{
public:
	Mode_MainMenu(size_t id) :ModeInterface(id) { Init(); }
	~Mode_MainMenu() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	void Enter() override;
	bool Exit() override { return true; }
	void Update(GameTimer& gt) override;
	void Render(PassData& data) override;
	void ProcessKey(char key) override;
	void Release() override;

private:

	//////////////////
	/// Operations ///
	//////////////////

	void Init();

	////////////
	/// Data ///
	////////////

	//Splash screens
	//std::vector<EM_Sprite*> m_Backgrounds;


	//Main Menu UI 
	UI_MainMenu* m_MainMenuUI = nullptr;
	UI_MapSelection* m_MapSelectionUI = nullptr;
	UI_CharacterSelect* m_CharacterSelectUI = nullptr;
	UI_VolumeControls* m_VolumeControlsUI = nullptr;
	//Gamepaly manager
	GameplayManager* m_GPManager = nullptr;
};