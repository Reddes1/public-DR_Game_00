#include "UI_MapSelection.h"
#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils

void UI_MapSelection::Reset()
{
	m_CurrentIndex = 0;
	m_MapSelection.SetPosition(m_MapOptionFrame->GetPosition());
}

void UI_MapSelection::InitialiseUI(std::string& fp, unsigned index)
{
	//Setup background
	m_Background.SetTexture("Backdrop_00", true);
	m_Background.SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	//Create and parse doc
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);
	auto& passData = Game::GetGame()->GetPassData();

	//Get array index
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	//Pre-Store offset values
	m_Offests.x = arr[index]["Menu Offset X"].GetFloat();
	m_Offests.y = arr[index]["Menu Offset Y"].GetFloat();

	//////////////
	/// Frames ///
	//////////////

	//Get array
	rapidjson::Value& fArr = doc["Custom UI Elements"][index]["Frame Setup"].GetArray();

	//setup map menu frames
	for (unsigned i(0); i < MAX_MAP_INDEXES; ++i)
	{
		m_MapOptionFrame[i].SetTexture(fArr[0]["Texture Name"].GetString());
		m_MapOptionFrame[i].SetFrame(fArr[0]["Frame ID"].GetUint());
		m_MapOptionFrame[i].SetOriginToPoint((JustificationPosition)fArr[0]["Origin ID"].GetUint());
		m_MapOptionFrame[i].SetScale(
			fArr[0]["Scale X"].GetFloat(),
			fArr[0]["Scale Y"].GetFloat()
		);
		m_MapOptionFrame[i].SetPosition(
			fArr[0]["Pos X"].GetFloat() + (i * m_Offests.x),
			fArr[0]["Pos Y"].GetFloat() + (i * m_Offests.y)
		);
		m_MapOptionFrame[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//////////////
	/// Icons ///
	/////////////

	//Get array
	rapidjson::Value& iArr = doc["Custom UI Elements"][index]["Map Icons"].GetArray();

	//setup map icons
	for (unsigned i(0); i < MAX_MAP_INDEXES; ++i)
	{
		m_MapIcons[i].SetTexture(iArr[i]["Texture Name"].GetString());
		m_MapIcons[i].SetFrame(iArr[i]["Frame ID"].GetUint());
		m_MapIcons[i].SetOriginToPoint((JustificationPosition)iArr[i]["Origin ID"].GetUint());
		m_MapIcons[i].SetScale(
			iArr[i]["Scale X"].GetFloat(),
			iArr[i]["Scale Y"].GetFloat()
		);
		m_MapIcons[i].SetPosition(
			iArr[i]["Pos X"].GetFloat() + (i * m_Offests.x),
			iArr[i]["Pos Y"].GetFloat() + (i * m_Offests.y)
		);
		m_MapIcons[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//Setup selection frame
	m_MapSelection.SetTexture(fArr[1]["Texture Name"].GetString());
	m_MapSelection.SetFrame(fArr[1]["Frame ID"].GetUint());
	m_MapSelection.SetOriginToPoint((JustificationPosition)fArr[1]["Origin ID"].GetUint());
	m_MapSelection.SetPosition(
		fArr[1]["Pos X"].GetFloat(),
		fArr[1]["Pos Y"].GetFloat()
	);
	m_MapSelection.SetScale(
		fArr[1]["Scale X"].GetFloat(),
		fArr[1]["Scale Y"].GetFloat()
	);
	m_MapSelection.SetLayerDepth(DRAW_DEPTH_UI_1);

	//Setup Title frame
	m_TitleFrame.SetTexture(fArr[2]["Texture Name"].GetString());
	m_TitleFrame.SetFrame(fArr[2]["Frame ID"].GetUint());
	m_TitleFrame.SetOriginToPoint((JustificationPosition)fArr[2]["Origin ID"].GetUint());
	m_TitleFrame.SetPosition(
		fArr[2]["Pos X"].GetFloat(),
		fArr[2]["Pos Y"].GetFloat()
	);
	m_TitleFrame.SetScale(
		fArr[2]["Scale X"].GetFloat(),
		fArr[2]["Scale Y"].GetFloat()
	);
	m_TitleFrame.SetLayerDepth(DRAW_DEPTH_UI_0);

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
	
	SetMaxIndex(MAX_MAP_INDEXES);

}

void UI_MapSelection::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	m_Background.Draw(sb, heap);
	//Render Selection frame
	m_MapSelection.Draw(sb, heap);
	//Render Title Frame
	m_TitleFrame.Draw(sb, heap);

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

	//Draw map frames and icons
	for (unsigned i(0); i < MAX_MAP_INDEXES; ++i)
	{
		m_MapOptionFrame[i].Draw(sb, heap);
		m_MapIcons[i].Draw(sb, heap);
	}
}

UI_MapSelection::Actions UI_MapSelection::ProcessInput(char key)
{
	switch (key)
	{
	case (char)CS_Survivor_00::DMP::BACK:
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::CHARACTER_SELCTION);
		break;

	case (char)CS_Survivor_00::DMP::MOVE_LEFT:
	case (char)CS_Survivor_00::DMS::MOVE_LEFT:
		if (m_CurrentIndex > 0)
		{
			--m_CurrentIndex;
			m_MapSelection.AddToPositionX(-m_Offests.x);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::MOVE_RIGHT:
	case (char)CS_Survivor_00::DMS::MOVE_RIGHT:
		if (m_CurrentIndex < m_MaxIndex)
		{
			++m_CurrentIndex;
			m_MapSelection.AddToPositionX(m_Offests.x);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::ACTION_00:
	case (char)CS_Survivor_00::DMS::ACTION_00:
		//play menu selection sound
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::MENU_SELECT, GameVolumes::SFX);
		SelectMap(m_CurrentIndex);
		Reset();
		return Actions::SELECTION_MADE;
		break;

	}
	return Actions::NO_ACTION;
}

void UI_MapSelection::SelectMap(unsigned index)
{
	switch (index)
	{
	case (unsigned)MapIDs::GRASSLAND:
		Game::GetGame()->GetGameplayManager().GetCamera().SetSceneTranslation(DirectX::XMFLOAT2{ -3072,-3072 });
		Game::GetGame()->GetGameplayManager().GetTileManager().SetActiveMap((unsigned)MapIDs::GRASSLAND);
		Game::GetGame()->GetModeManager().SwitchMode(ModeNames::MODE_GAME_SCENE);
		break;
	case (unsigned)MapIDs::CAVE:
		Game::GetGame()->GetGameplayManager().GetCamera().SetSceneTranslation(DirectX::XMFLOAT2{ -3072,-3072 });
		Game::GetGame()->GetGameplayManager().GetTileManager().SetActiveMap((unsigned)MapIDs::CAVE);
		Game::GetGame()->GetModeManager().SwitchMode(ModeNames::MODE_GAME_SCENE);
		break;
	}
}
