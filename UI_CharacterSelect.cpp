#include "UI_CharacterSelect.h"
#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils

void UI_CharacterSelect::Reset()
{
	m_CurrentIndex = 0;
	m_CharacterSelection.SetPosition(m_CharacterOptionFrame->GetPosition());
}

void UI_CharacterSelect::InitialiseUI(std::string& uiFP, std::string& cdFP, std::string& wdFP, unsigned index)
{
	//Setup background
	m_Background.SetTexture("Backdrop_00", true);
	m_Background.SetLayerDepth(DRAW_DEPTH_BACKGROUND_0);

	//Create and parse ui doc
	rapidjson::Document uiDoc;
	ParseNewJSONDocument(uiDoc, uiFP);

	auto& passData = Game::GetGame()->GetPassData();

	//Get array index
	rapidjson::Value& arr = uiDoc["Custom UI Elements"].GetArray();

	//Pre-Store offset values
	m_Offests.x = arr[index]["Menu Offset X"].GetFloat();
	m_Offests.y = arr[index]["Menu Offset Y"].GetFloat();
	m_TextOffset.x = arr[index]["Text Offset X"].GetFloat();
	m_TextOffset.y = arr[index]["Text Offset Y"].GetFloat();

	//////////////
	/// Frames ///
	//////////////

	//Get array
	rapidjson::Value& fArr = uiDoc["Custom UI Elements"][index]["Frame Setup"].GetArray();

	//setup character option frames
	for (unsigned i(0); i < MAX_CHARACTER_INDEXES; ++i)
	{
		m_CharacterOptionFrame[i].SetTexture(fArr[0]["Texture Name"].GetString());
		m_CharacterOptionFrame[i].SetFrame(fArr[0]["Frame ID"].GetUint());
		m_CharacterOptionFrame[i].SetOriginToPoint((JustificationPosition)fArr[0]["Origin ID"].GetUint());
		m_CharacterOptionFrame[i].SetScale(
			fArr[0]["Scale X"].GetFloat(),
			fArr[0]["Scale Y"].GetFloat()
		);
		m_CharacterOptionFrame[i].SetPosition(
			fArr[0]["Pos X"].GetFloat() + (i * m_Offests.x),
			fArr[0]["Pos Y"].GetFloat() + (i * m_Offests.y)
		);
		m_CharacterOptionFrame[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//Setup selection frame
	m_CharacterSelection.SetTexture(fArr[1]["Texture Name"].GetString());
	m_CharacterSelection.SetFrame(fArr[1]["Frame ID"].GetUint());
	m_CharacterSelection.SetOriginToPoint((JustificationPosition)fArr[1]["Origin ID"].GetUint());
	m_CharacterSelection.SetPosition(
		fArr[1]["Pos X"].GetFloat(),
		fArr[1]["Pos Y"].GetFloat()
	);
	m_CharacterSelection.SetScale(
		fArr[1]["Scale X"].GetFloat(),
		fArr[1]["Scale Y"].GetFloat()
	);
	m_CharacterSelection.SetLayerDepth(DRAW_DEPTH_UI_1);

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

	//Setup Info frame
	m_InfoFrame.SetTexture(fArr[3]["Texture Name"].GetString());
	m_InfoFrame.SetFrame(fArr[3]["Frame ID"].GetUint());
	m_InfoFrame.SetOriginToPoint((JustificationPosition)fArr[3]["Origin ID"].GetUint());
	m_InfoFrame.SetPosition(
		fArr[3]["Pos X"].GetFloat(),
		fArr[3]["Pos Y"].GetFloat()
	);
	m_InfoFrame.SetScale(
		fArr[3]["Scale X"].GetFloat(),
		fArr[3]["Scale Y"].GetFloat()
	);
	m_InfoFrame.SetLayerDepth(DRAW_DEPTH_UI_0);

	//////////////
	/// Icons ///
	/////////////

	//Get positional data from the UI doc
	rapidjson::Value& uiArr = uiDoc["Custom UI Elements"][index]["Character Icons"].GetArray();

	for (unsigned i(0); i < MAX_CHARACTER_INDEXES; ++i)
	{
		m_CharacterImages[i].SetOriginToPoint((JustificationPosition)uiArr[0]["Origin ID"].GetUint());
		m_CharacterImages[i].SetScale(
			uiArr[0]["Scale X"].GetFloat(),
			uiArr[0]["Scale Y"].GetFloat()
		);
		m_CharacterImages[i].SetPosition(
			uiArr[0]["Pos X"].GetFloat() + (i * m_Offests.x),
			uiArr[0]["Pos Y"].GetFloat() + (i * m_Offests.y));

		m_CharacterImages[i].SetLayerDepth(DRAW_DEPTH_UI_1);

		m_CharacterWepaons[i].SetOriginToPoint((JustificationPosition)uiArr[1]["Origin ID"].GetUint());
		m_CharacterWepaons[i].SetScale(
			uiArr[1]["Scale X"].GetFloat(),
			uiArr[1]["Scale Y"].GetFloat()
		);
		m_CharacterWepaons[i].SetPosition(
			uiArr[1]["Pos X"].GetFloat(),
			uiArr[1]["Pos Y"].GetFloat());

		m_CharacterWepaons[i].SetLayerDepth(DRAW_DEPTH_UI_1);
	}

	/////////////
	/// Texts ///
	/////////////

	//Get Array
	rapidjson::Value& tArr = uiDoc["Custom UI Elements"][index]["Texts"].GetArray();

	//Set the font and justification for each of the texts
	for (unsigned i(0); i < MAX_CHARACTER_INDEXES; ++i)
	{
		m_InfoText[i].m_StringJustification = (short)tArr[0]["Origin ID"].GetInt();
		m_InfoText[i].m_FontSF = passData.m_Spritefonts[tArr[0]["Font ID"].GetUint()].get();

		m_WeaponTexts[i].m_StringJustification = (short)tArr[1]["Origin ID"].GetInt();
		m_WeaponTexts[i].m_FontSF = passData.m_Spritefonts[tArr[1]["Font ID"].GetUint()].get();

		m_AbilityTexts[i].m_StringJustification = (short)tArr[1]["Origin ID"].GetInt();
		m_AbilityTexts[i].m_FontSF = passData.m_Spritefonts[tArr[1]["Font ID"].GetUint()].get();
	}

	//Setup window text
	m_WindowText.m_DrawString = arr[index]["Window Text"]["Text String"].GetString();
	m_WindowText.m_Position = {
		arr[index]["Window Text"]["Pos X"].GetFloat(),
		arr[index]["Window Text"]["Pos Y"].GetFloat()
	};
	m_WindowText.m_StringJustification = (short)arr[index]["Window Text"]["Origin ID"].GetInt();
	m_WindowText.m_Colour = DirectX::Colors::White;
	m_WindowText.m_FontSF = passData.m_Spritefonts[arr[index]["Window Text"]["Font ID"].GetUint()].get();

	//////////////////////
	/// Character Info ///
	/////////////////////

	//Setup first frame as the knight
	SetupCharacterFrame(cdFP, wdFP, 0, (unsigned)CharacterIDs::KNIGHT_00);
	//Setup second frame as the Gunner
	SetupCharacterFrame(cdFP, wdFP, 1, (unsigned)CharacterIDs::GUNNER_00);
	//Setup third frame as the Mage
	SetupCharacterFrame(cdFP, wdFP, 2, (unsigned)CharacterIDs::MAGE_00);

	SetMaxIndex(MAX_CHARACTER_INDEXES);
}

void UI_CharacterSelect::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	m_Background.Draw(sb, heap);
	//Render Selection frame
	m_CharacterSelection.Draw(sb, heap);
	//Render Title Frame
	m_TitleFrame.Draw(sb, heap);
	//Render info frame
	m_InfoFrame.Draw(sb, heap);
	//Draw character images
	for (unsigned i(0); i < MAX_CHARACTER_INDEXES; ++i)
	{
		m_CharacterImages[i].Draw(sb, heap);
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

	//Draw option frames
	for (unsigned i(0); i < MAX_CHARACTER_INDEXES; ++i)
	{
		m_CharacterOptionFrame[i].Draw(sb, heap);
	}

	//Contextual Rendering based on the current character index
	switch (m_CurrentIndex)
	{
	case (unsigned)CharacterIDs::KNIGHT_00:
		m_CharacterWepaons[m_CurrentIndex].Draw(sb, heap);

		TextRenderCall(m_CurrentIndex, sb, sf, heap, origin);
		break;
	case (unsigned)CharacterIDs::GUNNER_00:
		m_CharacterWepaons[m_CurrentIndex].Draw(sb, heap);

		TextRenderCall(m_CurrentIndex, sb, sf, heap, origin);
		break;
	case (unsigned)CharacterIDs::MAGE_00:
		m_CharacterWepaons[m_CurrentIndex].Draw(sb, heap);

		TextRenderCall(m_CurrentIndex, sb, sf, heap, origin);
		break;
	}
}

UI_CharacterSelect::Actions UI_CharacterSelect::ProcessInput(char key)
{
	switch (key)
	{
	case (char)CS_Survivor_00::DMP::BACK:
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::MAIN_MENU_OPEN);
		break;
	case (char)CS_Survivor_00::DMP::MOVE_LEFT:
	case (char)CS_Survivor_00::DMS::MOVE_LEFT:
		if (m_CurrentIndex > 0)
		{
			--m_CurrentIndex;
			m_CharacterSelection.AddToPositionX(-m_Offests.x);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::MOVE_RIGHT:
	case (char)CS_Survivor_00::DMS::MOVE_RIGHT:
		if (m_CurrentIndex < m_MaxIndex)
		{
			++m_CurrentIndex;
			m_CharacterSelection.AddToPositionX(m_Offests.x);
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::ACTION_00:
	case (char)CS_Survivor_00::DMS::ACTION_00:
		//play menu selection sound
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::MENU_SELECT, GameVolumes::SFX);
		SelectCharacter(m_CurrentIndex);
		
		//Change game state
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::MAP_SELECTION);
		
		Reset();
		return Actions::SELECTION_MADE;
		break;

	}
	return Actions::NO_ACTION;
}

void UI_CharacterSelect::SetupCharacterFrame(std::string& cdFP, std::string& wdFP, unsigned frameIndex, unsigned CharacterID)
{
	//Create and parse character info doc
	rapidjson::Document cdDoc;
	ParseNewJSONDocument(cdDoc, cdFP);

	//Get Character Info array
	rapidjson::Value& Arr = cdDoc["Character Data"].GetArray();

	//Set the character image based on character info
	m_CharacterImages[frameIndex].SetTexture(Arr[CharacterID]["Character Info"]["Character Image Texture Name"].GetString());
	m_CharacterImages[frameIndex].SetFrame(Arr[CharacterID]["Character Info"]["Character Image ID"].GetUint());
	//Set the weapon icon based on character info
	m_CharacterWepaons[frameIndex].SetTexture(Arr[CharacterID]["Character Info"]["Weapon Icon Texture Name"].GetString());
	m_CharacterWepaons[frameIndex].SetFrame(Arr[CharacterID]["Character Info"]["Weapon Frame ID"].GetUint());
	//Set the character description based on character info
	m_InfoText[frameIndex].m_DrawString = Arr[CharacterID]["Character Info"]["Character Description"].GetString();
	m_InfoText[frameIndex].m_Position = { m_InfoFrame.GetPosition().x - m_TextOffset.x
		, m_InfoFrame.GetPosition().y - m_TextOffset.y};
	m_InfoText[frameIndex].m_Colour = DirectX::Colors::White;

	SetupWeaponAndAbilityTexts(wdFP, frameIndex,
		Arr[CharacterID]["Character Info"]["Relative Weapon ID"].GetUint(),
		Arr[CharacterID]["Character Info"]["Relative Ability ID"].GetUint());
}

void UI_CharacterSelect::SetupWeaponAndAbilityTexts(std::string& wdFP, unsigned frameIndex, unsigned weaponID, unsigned abilityID)
{
	//Create and parse character info doc
	rapidjson::Document wdDoc;
	ParseNewJSONDocument(wdDoc, wdFP);

	//Get Character Info array
	rapidjson::Value& Arr = wdDoc["Weapon Data"].GetArray();

	//Set the weapon description based on weapon info
	m_WeaponTexts[frameIndex].m_DrawString = std::string("Weapon: ") + Arr[weaponID]["Misc"]["Description"].GetString();
	m_WeaponTexts[frameIndex].m_Position = { m_InfoFrame.GetPosition().x - m_TextOffset.x
		, m_InfoFrame.GetPosition().y };
	m_WeaponTexts[frameIndex].m_Colour = DirectX::Colors::White;

	//Set the ability description based on ability info
	m_AbilityTexts[frameIndex].m_DrawString = std::string("Ability: ") + Arr[abilityID]["Misc"]["Description"].GetString();
	m_AbilityTexts[frameIndex].m_Position = { m_InfoFrame.GetPosition().x - m_TextOffset.x
		, m_InfoFrame.GetPosition().y + m_TextOffset.y};
	m_AbilityTexts[frameIndex].m_Colour = DirectX::Colors::White;
}

void UI_CharacterSelect::TextRenderCall(unsigned currentIndex, DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap, DirectX::SimpleMath::Vector2 origin)
{
	//Character info text
	CalculateStringJustifyPosition(
		m_InfoText[currentIndex].m_FontSF,
		m_InfoText[currentIndex].m_StringJustification,
		m_InfoText[currentIndex].m_DrawString,
		origin
	);

	m_InfoText[currentIndex].m_FontSF->DrawString(
		&sb,
		m_InfoText[currentIndex].m_DrawString.c_str(),
		m_InfoText[currentIndex].m_Position,
		m_InfoText[currentIndex].m_Colour,
		m_InfoText[currentIndex].m_Rotation,
		origin,
		m_InfoText[currentIndex].m_Scale,
		m_InfoText[currentIndex].m_Effect,
		m_InfoText[currentIndex].m_LayerDepth
	);

	//Weapon info text
	CalculateStringJustifyPosition(
		m_WeaponTexts[currentIndex].m_FontSF,
		m_WeaponTexts[currentIndex].m_StringJustification,
		m_WeaponTexts[currentIndex].m_DrawString,
		origin
	);

	m_WeaponTexts[currentIndex].m_FontSF->DrawString(
		&sb,
		m_WeaponTexts[currentIndex].m_DrawString.c_str(),
		m_WeaponTexts[currentIndex].m_Position,
		m_WeaponTexts[currentIndex].m_Colour,
		m_WeaponTexts[currentIndex].m_Rotation,
		origin,
		m_WeaponTexts[currentIndex].m_Scale,
		m_WeaponTexts[currentIndex].m_Effect,
		m_WeaponTexts[currentIndex].m_LayerDepth
	);

	//Ability info text
	CalculateStringJustifyPosition(
		m_AbilityTexts[currentIndex].m_FontSF,
		m_AbilityTexts[currentIndex].m_StringJustification,
		m_AbilityTexts[currentIndex].m_DrawString,
		origin
	);

	m_AbilityTexts[currentIndex].m_FontSF->DrawString(
		&sb,
		m_AbilityTexts[currentIndex].m_DrawString.c_str(),
		m_AbilityTexts[currentIndex].m_Position,
		m_AbilityTexts[currentIndex].m_Colour,
		m_AbilityTexts[currentIndex].m_Rotation,
		origin,
		m_AbilityTexts[currentIndex].m_Scale,
		m_AbilityTexts[currentIndex].m_Effect,
		m_AbilityTexts[currentIndex].m_LayerDepth
	);
}

void UI_CharacterSelect::SelectCharacter(unsigned index)
{
	switch (index)
	{
	case (unsigned)CharacterIDs::KNIGHT_00:
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->InitAsCharacter(CharacterIDs::KNIGHT_00);
		break;
	case (unsigned)CharacterIDs::GUNNER_00:
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->InitAsCharacter(CharacterIDs::GUNNER_00);
		break;
	case (unsigned)CharacterIDs::MAGE_00:
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->InitAsCharacter(CharacterIDs::MAGE_00);
		break;
	default:
		msg_assert(false, "SelectCharacter(): Invalid Character ID!");
	}
}
