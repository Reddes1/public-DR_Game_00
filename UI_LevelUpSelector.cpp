#include "UI_LevelUpSelector.h"

#include "Game.h"	//Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils

void UI_LevelUpSelector::Reset()
{
	//Reset cursor position and index
	m_CurrentIndex = 0;
	m_SelectionFrame.SetPosition(m_OptionFrames->GetPosition());

	//Clear pointers and flags
	for (unsigned i(0); i < MAX_LEVEL_OPTIONS; ++i)
	{
		m_LevelFunctions[i] = nullptr;
		m_NewEquipFunctions[i] = nullptr;
		m_OptionEnabled[i] = false;
	}
}

void UI_LevelUpSelector::InitialiseUI(std::string& fp, unsigned index)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);
	auto& passData = Game::GetGame()->GetPassData();

	//Get array index for brevity
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	//Pre-store offset values
	m_Offsets.x = arr[index]["Menu Offset X"].GetFloat();
	m_Offsets.y = arr[index]["Menu Offset Y"].GetFloat();

	//////////////
	/// Frames ///
	//////////////

	//Get array for brevity
	rapidjson::Value& fArr = doc["Custom UI Elements"][index]["Frame Setup"].GetArray();
	//Setup option frames
	for (unsigned i(0); i < MAX_LEVEL_OPTIONS; ++i)
	{
		//
		//Main Frames 
		//

		m_OptionFrames[i].SetTexture(fArr[0]["Texture Name"].GetString());
		m_OptionFrames[i].SetFrame(fArr[0]["Frame ID"].GetUint());
		m_OptionFrames[i].SetOriginToPoint((JustificationPosition)fArr[0]["Origin ID"].GetUint());
		m_OptionFrames[i].SetScale(
			fArr[0]["Scale X"].GetFloat(),
			fArr[0]["Scale Y"].GetFloat()
		);
		switch (i)
		{
		case 0:
			m_OptionFrames[i].SetPosition(
				fArr[0]["Pos X"].GetFloat(),
				fArr[0]["Pos Y"].GetFloat()
			);
			break;
		case 1:
			m_OptionFrames[i].SetPosition(
				fArr[0]["Pos X"].GetFloat() - m_Offsets.x,
				fArr[0]["Pos Y"].GetFloat() + (i * m_Offsets.y)
			);
			break;
		case 2:
			m_OptionFrames[i].SetPosition(
				fArr[0]["Pos X"].GetFloat() + m_Offsets.x,
				fArr[0]["Pos Y"].GetFloat() + (i * m_Offsets.y)
			);
			break;
		}

		m_OptionFrames[i].SetLayerDepth(DRAW_DEPTH_UI_4);

		//
		//Icons
		//

		m_OptionIcons[i].SetTexture("UI_00");
		m_OptionIcons[i].SetFrame((int)GameUI00Desc::COIN_04);
		m_OptionIcons[i].SetOriginToCenter();
		m_OptionIcons[i].SetScale(2.f, 2.f);
		m_OptionIcons[i].SetPosition(m_OptionFrames[i].GetPosition().x, m_OptionFrames[i].GetPosition().y - 32.f);
		m_OptionIcons[i].SetLayerDepth(DRAW_DEPTH_UI_6);
	}

	//Setup highlight/selection frame
	m_SelectionFrame.SetTexture(fArr[1]["Texture Name"].GetString());
	m_SelectionFrame.SetFrame(fArr[1]["Frame ID"].GetUint());
	m_SelectionFrame.SetOriginToPoint((JustificationPosition)fArr[1]["Origin ID"].GetUint());
	m_SelectionFrame.SetPosition(
		fArr[1]["Pos X"].GetFloat(),
		fArr[1]["Pos Y"].GetFloat()
	);
	m_SelectionFrame.SetScale(
		fArr[1]["Scale X"].GetFloat(),
		fArr[1]["Scale Y"].GetFloat()
	);
	m_SelectionFrame.SetLayerDepth(DRAW_DEPTH_UI_5);

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
	//Setup window strings
	for (unsigned i(0); i < MAX_LEVEL_OPTIONS; ++i)
	{
		//
		//Texts
		//

		m_OptionName[i].m_Position = {
			tArr[0]["Offset X"].GetFloat() + m_OptionFrames[i].GetPosition().x,
			tArr[0]["Offset Y"].GetFloat() + m_OptionFrames[i].GetPosition().y
		};
		m_OptionLevel[i].m_Position = {
			tArr[1]["Offset X"].GetFloat() + m_OptionFrames[i].GetPosition().x,
			tArr[1]["Offset Y"].GetFloat() + m_OptionFrames[i].GetPosition().y
		};
		m_OptionDescription[i].m_Position = {
			tArr[2]["Offset X"].GetFloat() + m_OptionFrames[i].GetPosition().x,
			tArr[2]["Offset Y"].GetFloat() + m_OptionFrames[i].GetPosition().y
		};

		m_OptionName[i].m_StringJustification = (short)tArr[0]["Origin ID"].GetInt();
		m_OptionLevel[i].m_StringJustification = (short)tArr[1]["Origin ID"].GetInt();
		m_OptionDescription[i].m_StringJustification = (short)tArr[2]["Origin ID"].GetInt();

		m_OptionName[i].m_Colour = DirectX::Colors::White;
		m_OptionLevel[i].m_Colour = DirectX::Colors::White;
		m_OptionDescription[i].m_Colour = DirectX::Colors::White;

		m_OptionName[i].m_FontSF = passData.m_Spritefonts[tArr[0]["Font ID"].GetUint()].get();
		m_OptionLevel[i].m_FontSF = passData.m_Spritefonts[tArr[1]["Font ID"].GetUint()].get();
		m_OptionDescription[i].m_FontSF = passData.m_Spritefonts[tArr[2]["Font ID"].GetUint()].get();

		m_OptionName[i].m_LayerDepth = DRAW_DEPTH_UI_8;
		m_OptionLevel[i].m_LayerDepth = DRAW_DEPTH_UI_8;
		m_OptionDescription[i].m_LayerDepth = DRAW_DEPTH_UI_8;
	}
}

void UI_LevelUpSelector::Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap)
{
	//Render Highlight frame
	m_SelectionFrame.Draw(sb, heap);

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

	//For each option that is enabled, render frame and text
	for (unsigned i(0); i < MAX_LEVEL_OPTIONS; ++i)
	{
		if (m_OptionEnabled[i])
		{
			//Render frame
			m_OptionFrames[i].Draw(sb, heap);
			//Render Icon
			m_OptionIcons[i].Draw(sb, heap);
			
			//Render Text (Name)
			CalculateStringJustifyPosition(
				m_OptionName[i].m_FontSF,
				m_OptionName[i].m_StringJustification,
				m_OptionName[i].m_DrawString,
				origin
			);

			//Original Text
			m_OptionName[i].m_FontSF->DrawString(
				&sb,
				m_OptionName[i].m_DrawString.c_str(),
				m_OptionName[i].m_Position,
				m_OptionName[i].m_Colour,
				m_OptionName[i].m_Rotation,
				origin,
				m_OptionName[i].m_Scale,
				m_OptionName[i].m_Effect,
				m_OptionName[i].m_LayerDepth
			);

			//Render Text (Level)
			CalculateStringJustifyPosition(
				m_OptionLevel[i].m_FontSF,
				m_OptionLevel[i].m_StringJustification,
				m_OptionLevel[i].m_DrawString,
				origin
			);
			m_OptionLevel[i].m_FontSF->DrawString(
				&sb,
				m_OptionLevel[i].m_DrawString.c_str(),
				m_OptionLevel[i].m_Position,
				m_OptionLevel[i].m_Colour,
				m_OptionLevel[i].m_Rotation,
				origin,
				m_OptionLevel[i].m_Scale,
				m_OptionLevel[i].m_Effect,
				m_OptionLevel[i].m_LayerDepth
			);
			//Render Text (Description)
			CalculateStringJustifyPosition(
				m_OptionDescription[i].m_FontSF,
				m_OptionDescription[i].m_StringJustification,
				m_OptionDescription[i].m_DrawString,
				origin
			);
			m_OptionDescription[i].m_FontSF->DrawString(
				&sb,
				m_OptionDescription[i].m_DrawString.c_str(),
				m_OptionDescription[i].m_Position,
				m_OptionDescription[i].m_Colour,
				m_OptionDescription[i].m_Rotation,
				origin,
				m_OptionDescription[i].m_Scale,
				m_OptionDescription[i].m_Effect,
				m_OptionDescription[i].m_LayerDepth
			);
		}
	}
}

UI_LevelUpSelector::Actions UI_LevelUpSelector::ProcessInput(char key)
{
	switch (key) 
	{
	case (char)CS_Survivor_00::DMP::MOVE_LEFT:
	case (char)CS_Survivor_00::DMS::MOVE_LEFT:
		if (m_CurrentIndex == 0)
		{
			if (m_OptionEnabled[m_CurrentIndex + 1])
			{
				++m_CurrentIndex;
				m_SelectionFrame.AddToPositionX(-m_Offsets.x);
			}
		}
		else if (m_CurrentIndex == 2)
		{
			--m_CurrentIndex;
			--m_CurrentIndex;
			m_SelectionFrame.AddToPositionX(-m_Offsets.x);

		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::MOVE_RIGHT:
	case (char)CS_Survivor_00::DMS::MOVE_RIGHT:
		if (m_CurrentIndex == 1)
		{
			--m_CurrentIndex;
			m_SelectionFrame.AddToPositionX(m_Offsets.x);
		}
		else if (m_CurrentIndex == 0)
		{
			if (m_OptionEnabled[m_CurrentIndex + 2])
			{
				++m_CurrentIndex;
				++m_CurrentIndex;
				m_SelectionFrame.AddToPositionX(m_Offsets.x);
			}
		}
		return Actions::NO_ACTION;
		break;

	case (char)CS_Survivor_00::DMP::ACTION_00:
	case (char)CS_Survivor_00::DMS::ACTION_00:
		//play menu selection sound
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::MENU_SELECT, GameVolumes::SFX);
		//Based on type of action, call associated function
		if (m_LevelFunctions[m_CurrentIndex])
			m_LevelFunctions[m_CurrentIndex]();
		else if(m_NewEquipFunctions[m_CurrentIndex])
			m_NewEquipFunctions[m_CurrentIndex](1234); //<< Dummy number, already bound ID at creation
		else
		{
			//Error prevention till rest of mechanics added
		}
		//Reset UI elements/functions ptrs
		Reset();
		return Actions::SELECTION_MADE;
		break;

	}

	return Actions::NO_ACTION;
}

void UI_LevelUpSelector::InitialiseOptionAsOwnedWeapon(unsigned index, Player_Weapon* weapon)
{
	//Set weapon name
	m_OptionName[index].m_DrawString = weapon->GetGeneralData().m_Name;
	//Set weapon level
	unsigned weaponLevel = weapon->GetLevelingSystem().m_WeaponLevel;

	//Set level string based on level status
	if(weaponLevel+1 >= weapon->GetLevelingSystem().m_MaxLevel)
		m_OptionLevel[index].m_DrawString = std::string("Max!");
	else
		m_OptionLevel[index].m_DrawString = std::string("Lv: " + std::to_string(weaponLevel + 2));
	
	//Construct and set weapon description
	std::string str;
	for (auto& a : weapon->GetLevelingSystem().m_Levels[weaponLevel].m_LevelData)
	{
		str.append(a.m_Description);
		str.append("\n");
	}
	m_OptionDescription[index].m_DrawString = str;

	//Bind function
	m_LevelFunctions[index] = std::bind(&Player_Weapon::LevelUpWeapon, weapon);

	//Setup Icon
	m_OptionIcons[index].SetTexture(weapon->GetIconData().m_TextureName);
	m_OptionIcons[index].SetFrame(weapon->GetIconData().m_Frame);
	m_OptionIcons[index].SetScale(
		weapon->GetIconData().m_ScaleX * 2.f,
		weapon->GetIconData().m_ScaleY * 2.f
	);

	//Enable option
	m_OptionEnabled[index] = true;
}

void UI_LevelUpSelector::InitialiseOptionAsUnownedWeapon(unsigned index, unsigned weaponID)
{
	//Find data about weaponID
	WeaponCompositeData* const data = Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID(weaponID);

	//Set weapon name
	m_OptionName[index].m_DrawString = data->m_GeneralData.m_Name;

	//Set weapon level
	m_OptionLevel[index].m_DrawString = "New!";

	//Set weapon description
	m_OptionDescription[index].m_DrawString = data->m_LevelData.m_Description;

	//Bind Function
	m_NewEquipFunctions[index] = std::bind(
		&Entity_Player::AddWeaponToFreeSlot,
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer(),
		weaponID
		);

	//Setup Icon
	m_OptionIcons[index].SetTexture(data->m_IconData.m_TextureName);
	m_OptionIcons[index].SetFrame(data->m_IconData.m_Frame);
	m_OptionIcons[index].SetScale(
		data->m_IconData.m_ScaleX * 2.f,
		data->m_IconData.m_ScaleY * 2.f
	);

	//Enable option
	m_OptionEnabled[index] = true;
}

void UI_LevelUpSelector::InitialiseOptionAsOwnedAccessory(unsigned index, Player_Accessory* accessory)
{
	//Set weapon name
	m_OptionName[index].m_DrawString = accessory->GetGeneralData().m_Name;
	//Set weapon level
	unsigned accessoryLevel = accessory->GetLevelingSystem().m_WeaponLevel;
	//Set level string based on level status
	if (accessoryLevel + 1 >= accessory->GetLevelingSystem().m_MaxLevel)
		m_OptionLevel[index].m_DrawString = std::string("Max!");
	else
		m_OptionLevel[index].m_DrawString = std::string("Lv: " + std::to_string(accessoryLevel + 1));

	//Construct and set weapon description
	std::string str;
	for (auto& a : accessory->GetLevelingSystem().m_Levels[accessoryLevel].m_LevelData)
	{
		str.append(a.m_Description);
		str.append("\n");
	}
	m_OptionDescription[index].m_DrawString = str;

	//Bind function
	m_LevelFunctions[index] = std::bind(&Player_Accessory::LevelUpAccessory, accessory);

	//Setup Icon
	m_OptionIcons[index].SetTexture(accessory->GetIconData().m_TextureName);
	m_OptionIcons[index].SetFrame(accessory->GetIconData().m_Frame);
	m_OptionIcons[index].SetScale(
		accessory->GetIconData().m_ScaleX * 2.f,
		accessory->GetIconData().m_ScaleY * 2.f
	);

	//Enable option
	m_OptionEnabled[index] = true;
}

void UI_LevelUpSelector::InitialiseOptionAsUnownedAccessory(unsigned index, unsigned accessoryID)
{
	//Find data about weaponID
	AccessoryCompositeData* const data = Game::GetGame()->GetGameplayManager().GetDataManager().GetAccessoryDataAtID(accessoryID);

	//Set weapon name
	m_OptionName[index].m_DrawString = data->m_GeneralData.m_Name;

	//Set weapon level
	m_OptionLevel[index].m_DrawString = "New!";

	//Set weapon description
	m_OptionDescription[index].m_DrawString = data->m_LevelData.m_Description;

	//Bind Function
	m_NewEquipFunctions[index] = std::bind(
		&Entity_Player::AddAccessoryToFreeSlot,
		Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer(),
		accessoryID
	);

	//Setup Icon
	m_OptionIcons[index].SetTexture(data->m_IconData.m_TextureName);
	m_OptionIcons[index].SetFrame(data->m_IconData.m_Frame);
	m_OptionIcons[index].SetScale(
		data->m_IconData.m_ScaleX * 2.f,
		data->m_IconData.m_ScaleY * 2.f
	);

	//Enable option
	m_OptionEnabled[index] = true;
}

