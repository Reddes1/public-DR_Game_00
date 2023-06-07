#include "UI_VolumeControls.h"
#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils

void UI_VolumeControls::Reset()
{
	m_CurrentIndex = 0;
	m_VolumeBarOutline.SetPosition(m_VolumeBars->GetPosition());
}

void UI_VolumeControls::InitialiseUI(std::string& fp, unsigned index)
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

	//Set up Volume bars
	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		m_VolumeBars[i].SetTexture(fArr[0]["Texture Name"].GetString());
		m_VolumeBars[i].SetFrame(fArr[0]["Frame ID"].GetUint());
		m_VolumeBars[i].SetOriginToPoint((JustificationPosition)fArr[0]["Origin ID"].GetUint());
		m_VolumeBars[i].SetPosition(
			fArr[0]["Pos X"].GetFloat() + (i * m_Offests.x),
			fArr[0]["Pos Y"].GetFloat() + (i * m_Offests.y)
		);
		m_VolumeBars[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//Set the scales for each of the volume bars relative to the current volume they represent
	//Master
	m_VolumeBars[0].SetScale(
		Game::GetGame()->GetAudioManager().GetVolumeControls().GetMasterVolume() / 1,
		fArr[0]["Scale Y"].GetFloat()
	);
	//Music
	m_VolumeBars[1].SetScale(
		Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicVolume() / 1,
		fArr[0]["Scale Y"].GetFloat()
	);
	//SFX
	m_VolumeBars[2].SetScale(
		Game::GetGame()->GetAudioManager().GetVolumeControls().GetSFXVolume() / 1,
		fArr[0]["Scale Y"].GetFloat()
	);

	//Setup volume bar outline frame
	m_VolumeBarOutline.SetTexture(fArr[1]["Texture Name"].GetString());
	m_VolumeBarOutline.SetFrame(fArr[1]["Frame ID"].GetUint());
	m_VolumeBarOutline.SetOriginToPoint((JustificationPosition)fArr[1]["Origin ID"].GetUint());
	m_VolumeBarOutline.SetPosition(
		fArr[1]["Pos X"].GetFloat(),
		fArr[1]["Pos Y"].GetFloat()
	);
	m_VolumeBarOutline.SetScale(
		fArr[1]["Scale X"].GetFloat(),
		fArr[1]["Scale Y"].GetFloat()
	);
	m_VolumeBarOutline.SetLayerDepth(DRAW_DEPTH_UI_1);

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

	//////////////////////////
	/// Volume Name Frames ///
	/////////////////////////

	rapidjson::Value& vArr = doc["Custom UI Elements"][index]["Volume Name Frames"].GetArray();

	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		m_VolumeNameFrames[i].SetTexture(vArr[i]["Texture Name"].GetString());
		m_VolumeNameFrames[i].SetFrame(vArr[i]["Frame ID"].GetUint());
		m_VolumeNameFrames[i].SetOriginToPoint((JustificationPosition)vArr[i]["Origin ID"].GetUint());
		m_VolumeNameFrames[i].SetScale(
			vArr[i]["Scale X"].GetFloat(),
			vArr[i]["Scale Y"].GetFloat()
		);
		m_VolumeNameFrames[i].SetPosition(
			vArr[i]["Pos X"].GetFloat(),
			vArr[i]["Pos Y"].GetFloat());
		m_VolumeNameFrames[i].SetLayerDepth(DRAW_DEPTH_UI_1);
	}

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

	//Get Array
	rapidjson::Value& tArr = doc["Custom UI Elements"][index]["Texts"].GetArray();

	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		m_VolumeNameText[i].m_DrawString = tArr[i]["Text String"].GetString();

		m_VolumeNameText[i].m_Position = { m_VolumeNameFrames[i].GetPosition().x, m_VolumeNameFrames[i].GetPosition().y };

		m_VolumeNameText[i].m_StringJustification = (short)tArr[i]["Origin ID"].GetInt();
		m_VolumeNameText[i].m_Colour = DirectX::Colors::White;
		m_VolumeNameText[i].m_FontSF = passData.m_Spritefonts[tArr[i]["Font ID"].GetUint()].get();
	}

	SetMaxIndex(MAX_OPTION_MENU_ENTRIES);
}

void UI_VolumeControls::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	m_Background.Draw(sb, heap);
	//Render volume outline
	m_VolumeBarOutline.Draw(sb, heap);
	//Render Title Frame
	m_TitleFrame.Draw(sb, heap);
	//Render Volume name frames
	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		m_VolumeNameFrames[i].Draw(sb, heap);
	}
	//Render Volume bars 
	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		m_VolumeBars[i].Draw(sb, heap);
	}

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

	//Render Volume name frame text 
	for (unsigned i(0); i < MAX_OPTION_MENU_ENTRIES; ++i)
	{
		CalculateStringJustifyPosition(
			m_VolumeNameText[i].m_FontSF,
			m_VolumeNameText[i].m_StringJustification,
			m_VolumeNameText[i].m_DrawString,
			origin
		);

		m_VolumeNameText[m_CurrentIndex].m_FontSF->DrawString(
			&sb,
			m_VolumeNameText[i].m_DrawString.c_str(),
			m_VolumeNameText[i].m_Position,
			m_VolumeNameText[i].m_Colour,
			m_VolumeNameText[i].m_Rotation,
			origin,
			m_VolumeNameText[i].m_Scale,
			m_VolumeNameText[i].m_Effect,
			m_VolumeNameText[i].m_LayerDepth
		);
	}
}

UI_VolumeControls::Actions UI_VolumeControls::ProcessInput(char key)
{
	switch (key)
	{
	case (char)CS_Survivor_00::DMP::BACK:
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::MAIN_MENU_OPEN);
		break;

	case (char)CS_Survivor_00::DMP::MOVE_UP:
	case (char)CS_Survivor_00::DMS::MOVE_UP:
		if (m_CurrentIndex > 0)
		{
			--m_CurrentIndex;
			m_VolumeBarOutline.AddToPositionY(-m_Offests.y);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::MOVE_DOWN:
	case (char)CS_Survivor_00::DMS::MOVE_DOWN:
		if (m_CurrentIndex < m_MaxIndex)
		{
			++m_CurrentIndex;
			m_VolumeBarOutline.AddToPositionY(m_Offests.y);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::MOVE_LEFT:
	case (char)CS_Survivor_00::DMS::MOVE_LEFT:
		DecrementVolumes();
		break;

	case (char)CS_Survivor_00::DMP::MOVE_RIGHT:
	case (char)CS_Survivor_00::DMS::MOVE_RIGHT:
		IncrementVolumes();
		break;

	}

	return Actions::NO_ACTION;
}

void UI_VolumeControls::DecrementVolumes()
{
	if (m_CurrentIndex == 0)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncMasterVolume(-0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMasterVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
	else if (m_CurrentIndex == 1)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncMusicVolume(-0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
	else if (m_CurrentIndex == 2)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncSFXVolume(-0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetSFXVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
}

void UI_VolumeControls::IncrementVolumes()
{
	if (m_CurrentIndex == 0)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncMasterVolume(0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMasterVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
	else if (m_CurrentIndex == 1)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncMusicVolume(0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetMusicVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
	else if (m_CurrentIndex == 2)
	{
		Game::GetGame()->GetAudioManager().GetVolumeControls().IncSFXVolume(0.1f);
		m_VolumeBars[m_CurrentIndex].SetScale(
			Game::GetGame()->GetAudioManager().GetVolumeControls().GetSFXVolume() / 1,
			m_VolumeBars[m_CurrentIndex].GetScale().y
		);
	}
}
