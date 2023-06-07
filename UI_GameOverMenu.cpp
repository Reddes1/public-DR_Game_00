#include "UI_GameOverMenu.h"
#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils
void UI_GameOverMenu::Reset()
{
	m_CurrentIndex = 0;
	m_MenuSelection.SetPosition(m_MenuFrames->GetPosition());
}

void UI_GameOverMenu::InitialiseUI(std::string& fp, unsigned index)
{
	//Create and parse doc
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
	for (unsigned i(0); i < MAX_GAMEOVER_MENU_INDEXES; ++i)
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

	//Setup window text
	m_WindowText.m_DrawString = arr[index]["Window Text"]["Text String"].GetString();
	m_WindowText.m_Position = {
		arr[index]["Window Text"]["Pos X"].GetFloat(),
		arr[index]["Window Text"]["Pos Y"].GetFloat()
	};
	m_WindowText.m_StringJustification = (short)arr[index]["Window Text"]["Origin ID"].GetInt();
	m_WindowText.m_Colour = DirectX::Colors::White;
	m_WindowText.m_FontSF = passData.m_Spritefonts[arr[index]["Window Text"]["Font ID"].GetUint()].get();

	//Get array for brevity
	rapidjson::Value& tArr = doc["Custom UI Elements"][index]["Texts"].GetArray();

	//Setup menu text
	for (unsigned i(0); i < MAX_GAMEOVER_MENU_INDEXES; ++i)
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

	SetMaxIndex(MAX_GAMEOVER_MENU_INDEXES);
}

void UI_GameOverMenu::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	//Render highlight selection frame
	m_MenuSelection.Draw(sb, heap);

	//Draw frame text
	DirectX::SimpleMath::Vector2 origin = { 0, 0 };
	CalculateStringJustifyPosition(
		m_WindowText.m_FontSF,
		m_WindowText.m_StringJustification,
		m_WindowText.m_DrawString,
		origin
	);
	m_WindowText.m_FontSF->DrawString(
		&sb,
		m_WindowText.m_DrawString.c_str(),
		m_WindowText.m_Position,
		m_WindowText.m_Colour,
		m_WindowText.m_Rotation,
		origin,
		m_WindowText.m_Scale,
		m_WindowText.m_Effect,
		m_WindowText.m_LayerDepth
	);

	for (unsigned i(0); i < MAX_GAMEOVER_MENU_INDEXES; ++i)
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

UI_GameOverMenu::Actions UI_GameOverMenu::ProcessInput(char key)
{
	switch(key)
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
		if (m_CurrentIndex == 0)
			Game::GetGame()->GetModeManager().SwitchMode(ModeNames::MODE_MAIN_MENU);
		else
			PostQuitMessage(0);
		Reset();
		return Actions::SELECTION_MADE;
		break;
	}
	return Actions::NO_ACTION;
}
