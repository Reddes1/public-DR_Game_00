#include "UI_MainMenu.h"

#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils
void UI_MainMenu::Reset()
{
	m_CurrentIndex = 0;
	m_MenuSelection.SetPosition(m_MenuFrames->GetPosition());
}

void UI_MainMenu::InitialiseUI(std::string& fp, unsigned index)
{
	//Setup backgrounds
	//Main menu
	m_Background[0].SetTexture("MainMenu_00", true);
	m_Background[0].SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);
	//Controls
	m_Background[1].SetTexture("Controls_00", true);
	m_Background[1].SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	//Create and parse Doc
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);
	auto& passData = Game::GetGame()->GetPassData();

	//Get array index
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	//Pre-store offset values
	m_Offests.x = arr[index]["Menu Offset X"].GetFloat();
	m_Offests.y = arr[index]["Menu Offset Y"].GetFloat();

	//////////////
	/// Frames ///
	//////////////

	//Get array for brevity
	rapidjson::Value& fArr = doc["Custom UI Elements"][index]["Frame Setup"].GetArray();

	//setup menu frames
	for (unsigned i(0); i < MAX_MAIN_MENU_INDEXES; ++i)
	{
		m_MenuFrames[i].SetTexture(fArr[0]["Texture Name"].GetString());
		m_MenuFrames[i].SetFrame(fArr[0]["Frame ID"].GetUint());
		m_MenuFrames[i].SetOriginToPoint((JustificationPosition)fArr[0]["Origin ID"].GetUint());
		m_MenuFrames[i].SetScale(
			fArr[0]["Scale X"].GetFloat(),
			fArr[0]["Scale Y"].GetFloat()
		);
		m_MenuFrames[i].SetPosition(
			fArr[0]["Pos X"].GetFloat() + (i * m_Offests.x),
			fArr[0]["Pos Y"].GetFloat() + (i * m_Offests.y)
		);
		m_MenuFrames[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//Setup selection frame
	m_MenuSelection.SetTexture(fArr[1]["Texture Name"].GetString());
	m_MenuSelection.SetFrame(fArr[1]["Frame ID"].GetUint());
	m_MenuSelection.SetOriginToPoint((JustificationPosition)fArr[1]["Origin ID"].GetUint());
	m_MenuSelection.SetPosition(
		fArr[1]["Pos X"].GetFloat(),
		fArr[1]["Pos Y"].GetFloat()
	);
	m_MenuSelection.SetScale(
		fArr[1]["Scale X"].GetFloat(),
		fArr[1]["Scale Y"].GetFloat()
	);
	m_MenuSelection.SetLayerDepth(DRAW_DEPTH_UI_1);

	/////////////
	/// Texts ///
	/////////////

	//Get array for brevity
	rapidjson::Value& tArr = doc["Custom UI Elements"][index]["Texts"].GetArray();

	//Setup menu text
	for (unsigned i(0); i < MAX_MAIN_MENU_INDEXES; ++i)
	{
		m_MenuText[i].m_DrawString = tArr[i]["Text String"].GetString();

		m_MenuText[i].m_Position = {
			tArr[i]["Offset X"].GetFloat() + m_MenuFrames[i].GetPosition().x,
			tArr[i]["Offset Y"].GetFloat() + m_MenuFrames[i].GetPosition().y
		};

		m_MenuText[i].m_StringJustification = (short)tArr[i]["Origin ID"].GetInt();
		m_MenuText[i].m_Colour = DirectX::Colors::White;
		m_MenuText[i].m_FontSF = passData.m_Spritefonts[tArr[i]["Font ID"].GetUint()].get();

	}
}

void UI_MainMenu::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	//Get Game
	Game* game = Game::GetGame();
	//Render based on game state
	switch (game->GetGameplayManager().GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::MAIN_MENU_OPEN:
	{
		m_Background[0].Draw(sb, heap);
		//Render highlight selection frame
		m_MenuSelection.Draw(sb, heap);
		//Draw frame text
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };

		for (unsigned i(0); i < MAX_MAIN_MENU_INDEXES; ++i)
		{
			m_MenuFrames[i].Draw(sb, heap);

			//Render Text (Name)
			CalculateStringJustifyPosition(
				m_MenuText[i].m_FontSF,
				m_MenuText[i].m_StringJustification,
				m_MenuText[i].m_DrawString,
				origin
			);
			m_MenuText[i].m_FontSF->DrawString(
				&sb,
				m_MenuText[i].m_DrawString.c_str(),
				m_MenuText[i].m_Position,
				m_MenuText[i].m_Colour,
				m_MenuText[i].m_Rotation,
				origin,
				m_MenuText[i].m_Scale,
				m_MenuText[i].m_Effect,
				m_MenuText[i].m_LayerDepth
			);
		}
	}
		break;
	case SM_GameStateManager::AGS::CONTROLS:
		m_Background[1].Draw(sb, heap);
		break;
	}


}

UI_MainMenu::Actions UI_MainMenu::ProcessInput(char key)
{
	//Get Game
	Game* game = Game::GetGame();
	//Render based on game state
	switch (game->GetGameplayManager().GetStateManager().GetActiveGameState())
	{
	case SM_GameStateManager::AGS::MAIN_MENU_OPEN:
		switch (key)
		{
		case (char)CS_Survivor_00::DMP::MOVE_UP:
		case (char)CS_Survivor_00::DMS::MOVE_UP:
			if (m_CurrentIndex > 0)
			{
				--m_CurrentIndex;
				m_MenuSelection.AddToPositionY(-m_Offests.y);
			}
			return Actions::NO_ACTION;
			break;

		case (char)CS_Survivor_00::DMP::MOVE_DOWN:
		case (char)CS_Survivor_00::DMS::MOVE_DOWN:
			if (m_CurrentIndex < m_MaxIndex)
			{
				++m_CurrentIndex;
				m_MenuSelection.AddToPositionY(m_Offests.y);
			}
			return Actions::NO_ACTION;
			break;

		case (char)CS_Survivor_00::DMP::ACTION_00:
		case (char)CS_Survivor_00::DMS::ACTION_00:

			//play menu selection sound
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::MENU_SELECT, GameVolumes::SFX);

			//Based on index, execute target function
			if (m_CurrentIndex == 0)
				Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::CHARACTER_SELCTION);
			else if (m_CurrentIndex == 1)
				Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::VOLUME_CONTROLS);
			else if (m_CurrentIndex == 2)
				Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::CONTROLS);
			else
				PostQuitMessage(0);
			Reset();

			return Actions::SELECTION_MADE;
			break;
		}
		break;
	case SM_GameStateManager::AGS::CONTROLS:
		switch (key)
		{
		case (char)CS_Survivor_00::DMP::BACK:
			Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::MAIN_MENU_OPEN);
			return Actions::SELECTION_MADE;
			break;
		}
		break;
	}

	//No valid key passed
	return Actions::NO_ACTION;
}
