#include "UI_PlayerInfo.h"

#include "Game.h"				  //Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils
#include "EngineDefinitions.h"

void UI_PlayerInfo::Update(GameTimer& gt)
{
	//////////////////
	/// Health Bar ///
	//////////////////

	//Get player health and regen values
	int cHP = m_Player->GetStats().m_Health;
	int mHP = m_Player->GetStats().m_MaxHealth;
	unsigned regen = (unsigned)m_Player->GetStats().m_Regen;

	//Adjust inner bar length relative to player health
	float xScale = ((float)cHP / (float)mHP);
	//Clamp Scale
	if (xScale > 1.f)
		xScale = 1.f;
	m_HealthFrames[(unsigned)HealthFrameIDs::HEALTH_BAR].SetScale(
		xScale,
		m_HealthFrames[(unsigned)HealthFrameIDs::HEALTH_BAR].GetScale().y
	);

	//Build text string for hp
	m_Texts[(unsigned)TextIDs::HEALTH].m_DrawString.clear();
	m_Texts[(unsigned)TextIDs::HEALTH].m_DrawString = std::to_string(cHP).append(" / ").append(std::to_string(mHP));

	//Build text string for regen
	m_Texts[(unsigned)TextIDs::REGEN].m_DrawString.clear();
	m_Texts[(unsigned)TextIDs::REGEN].m_DrawString.append("+").append(std::to_string(regen));

	///////////////////
	/// Ability Bar ///
	///////////////////

	int cCD = static_cast<int>(std::round(m_Player->GetAbility().GetCurrentCD()));
	int mCD = static_cast<int>(std::round(m_Player->GetAbility().GetStats().m_Cooldown));

	xScale = ((float)cCD / (float)mCD);
	if (xScale > 1.f)
		xScale = 1.f;
	m_AbilityFrames[(unsigned)AbilityFrameIDs::ABILITY_BAR].SetScale(
		xScale,
		m_AbilityFrames[(unsigned)AbilityFrameIDs::ABILITY_BAR].GetScale().y
	);

	//Build text for ability bar
	m_Texts[(unsigned)TextIDs::ABILITY_CD].m_DrawString.clear();
	m_Texts[(unsigned)TextIDs::ABILITY_CD].m_DrawString = std::to_string(cCD).append(" / ").append(std::to_string(mCD));

}

void UI_PlayerInfo::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Draw Sprites
	for (auto& a : m_HealthFrames)
		a.Draw(sb, heap);
	for (auto& a : m_AbilityFrames)
		a.Draw(sb, heap);
	for (auto& a : m_WeaponFrames)
		a.Draw(sb, heap);
	for (auto& a : m_AccessoryFrames)
		a.Draw(sb, heap);

	//Draw Icons (if enabled)
	for (unsigned i(0); i < (unsigned)WeaponFrameIDs::COUNT; ++i)
		if (m_WeaponFlags[i] == true)
			m_WeaponIcons[i].Draw(sb, heap);
	for (unsigned i(0); i < (unsigned)AccessoryFrameIDs::COUNT; ++i)
		if (m_AccessoryFlags[i] == true)
			m_AccessoryIcons[i].Draw(sb, heap);

	//Draw Strings
	DirectX::SimpleMath::Vector2 origin = { 0, 0 };

	for (auto& a : m_Texts)
	{
		CalculateStringJustifyPosition(
			a.m_FontSF,
			a.m_StringJustification,
			a.m_DrawString,
			origin
		);

		//Outline effect
		a.m_FontSF->DrawString(
			&sb,
			a.m_DrawString.c_str(),
			a.m_Position + DirectX::SimpleMath::Vector2(1.f, 1.f),
			DirectX::Colors::Black,
			a.m_Rotation,
			origin,
			a.m_Scale,
			a.m_Effect,
			DRAW_DEPTH_UI_2
		);
		a.m_FontSF->DrawString(
			&sb,
			a.m_DrawString.c_str(),
			a.m_Position + DirectX::SimpleMath::Vector2(-1.f, 1.f),
			DirectX::Colors::Black,
			a.m_Rotation,
			origin,
			a.m_Scale,
			a.m_Effect,
			DRAW_DEPTH_UI_2
		);
		a.m_FontSF->DrawString(
			&sb,
			a.m_DrawString.c_str(),
			a.m_Position + DirectX::SimpleMath::Vector2(1.f, -1.f),
			DirectX::Colors::Black,
			a.m_Rotation,
			origin,
			a.m_Scale,
			a.m_Effect,
			DRAW_DEPTH_UI_2
		);
		a.m_FontSF->DrawString(
			&sb,
			a.m_DrawString.c_str(),
			a.m_Position + DirectX::SimpleMath::Vector2(-1.f, -1.f),
			DirectX::Colors::Black,
			a.m_Rotation,
			origin,
			a.m_Scale,
			a.m_Effect,
			DRAW_DEPTH_UI_2
		);

		//Original String
		a.m_FontSF->DrawString(
			&sb,
			a.m_DrawString.c_str(),
			a.m_Position,
			a.m_Colour,
			a.m_Rotation,
			origin,
			a.m_Scale,
			a.m_Effect,
			a.m_LayerDepth
		);
	}

	for (unsigned i(0); i < (unsigned)WeaponFrameIDs::COUNT; ++i)
	{
		if (m_WeaponFlags[i] == true)
		{
			CalculateStringJustifyPosition(
				m_WeaponTexts[i].m_FontSF,
				m_WeaponTexts[i].m_StringJustification,
				m_WeaponTexts[i].m_DrawString,
				origin
			);

			//Outline Effect
			m_WeaponTexts[i].m_FontSF->DrawString(
				&sb,
				m_WeaponTexts[i].m_DrawString.c_str(),
				m_WeaponTexts[i].m_Position + DirectX::SimpleMath::Vector2(1.f, 1.f),
				DirectX::Colors::Black,
				m_WeaponTexts[i].m_Rotation,
				origin,
				m_WeaponTexts[i].m_Scale,
				m_WeaponTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_WeaponTexts[i].m_FontSF->DrawString(
				&sb,
				m_WeaponTexts[i].m_DrawString.c_str(),
				m_WeaponTexts[i].m_Position + DirectX::SimpleMath::Vector2(-1.f, 1.f),
				DirectX::Colors::Black,
				m_WeaponTexts[i].m_Rotation,
				origin,
				m_WeaponTexts[i].m_Scale,
				m_WeaponTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_WeaponTexts[i].m_FontSF->DrawString(
				&sb,
				m_WeaponTexts[i].m_DrawString.c_str(),
				m_WeaponTexts[i].m_Position + DirectX::SimpleMath::Vector2(-1.f, -1.f),
				DirectX::Colors::Black,
				m_WeaponTexts[i].m_Rotation,
				origin,
				m_WeaponTexts[i].m_Scale,
				m_WeaponTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_WeaponTexts[i].m_FontSF->DrawString(
				&sb,
				m_WeaponTexts[i].m_DrawString.c_str(),
				m_WeaponTexts[i].m_Position + DirectX::SimpleMath::Vector2(1.f, -1.f),
				DirectX::Colors::Black,
				m_WeaponTexts[i].m_Rotation,
				origin,
				m_WeaponTexts[i].m_Scale,
				m_WeaponTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);

			//Main String
			m_WeaponTexts[i].m_FontSF->DrawString(
				&sb,
				m_WeaponTexts[i].m_DrawString.c_str(),
				m_WeaponTexts[i].m_Position,
				m_WeaponTexts[i].m_Colour,
				m_WeaponTexts[i].m_Rotation,
				origin,
				m_WeaponTexts[i].m_Scale,
				m_WeaponTexts[i].m_Effect,
				m_WeaponTexts[i].m_LayerDepth
			);
		}
	}


	for (unsigned i(0); i < (unsigned)AccessoryFrameIDs::COUNT; ++i)
	{
		if (m_AccessoryFlags[i] == true)
		{
			CalculateStringJustifyPosition(
				m_AccessoryTexts[i].m_FontSF,
				m_AccessoryTexts[i].m_StringJustification,
				m_AccessoryTexts[i].m_DrawString,
				origin
			);

			//Outline Effect
			m_AccessoryTexts[i].m_FontSF->DrawString(
				&sb,
				m_AccessoryTexts[i].m_DrawString.c_str(),
				m_AccessoryTexts[i].m_Position + DirectX::SimpleMath::Vector2(1.f, 1.f),
				DirectX::Colors::Black,
				m_AccessoryTexts[i].m_Rotation,
				origin,
				m_AccessoryTexts[i].m_Scale,
				m_AccessoryTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_AccessoryTexts[i].m_FontSF->DrawString(
				&sb,
				m_AccessoryTexts[i].m_DrawString.c_str(),
				m_AccessoryTexts[i].m_Position + DirectX::SimpleMath::Vector2(-1.f, 1.f),
				DirectX::Colors::Black,
				m_AccessoryTexts[i].m_Rotation,
				origin,
				m_AccessoryTexts[i].m_Scale,
				m_AccessoryTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_AccessoryTexts[i].m_FontSF->DrawString(
				&sb,
				m_AccessoryTexts[i].m_DrawString.c_str(),
				m_AccessoryTexts[i].m_Position + DirectX::SimpleMath::Vector2(-1.f, -1.f),
				DirectX::Colors::Black,
				m_AccessoryTexts[i].m_Rotation,
				origin,
				m_AccessoryTexts[i].m_Scale,
				m_AccessoryTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);
			m_AccessoryTexts[i].m_FontSF->DrawString(
				&sb,
				m_AccessoryTexts[i].m_DrawString.c_str(),
				m_AccessoryTexts[i].m_Position + DirectX::SimpleMath::Vector2(1.f, -1.f),
				DirectX::Colors::Black,
				m_AccessoryTexts[i].m_Rotation,
				origin,
				m_AccessoryTexts[i].m_Scale,
				m_AccessoryTexts[i].m_Effect,
				DRAW_DEPTH_UI_2
			);

			//Main String
			m_AccessoryTexts[i].m_FontSF->DrawString(
				&sb,
				m_AccessoryTexts[i].m_DrawString.c_str(),
				m_AccessoryTexts[i].m_Position,
				m_AccessoryTexts[i].m_Colour,
				m_AccessoryTexts[i].m_Rotation,
				origin,
				m_AccessoryTexts[i].m_Scale,
				m_AccessoryTexts[i].m_Effect,
				m_AccessoryTexts[i].m_LayerDepth
			);
		}
	}
}

void UI_PlayerInfo::InitialiseUI(std::string& fp, unsigned index)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Get array index for brevity
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	/////////////////////
	/// Health Frames ///
	/////////////////////

	//Get health frame array for brevity
	rapidjson::Value& hArr = arr[index]["Frame Setup"]["Health Frames"].GetArray();
	for (unsigned i(0); i < (unsigned)HealthFrameIDs::COUNT; ++i)
	{
		m_HealthFrames[i].SetTexture(hArr[i]["Texture Name"].GetString());
		m_HealthFrames[i].SetFrame(hArr[i]["Frame ID"].GetUint());
		m_HealthFrames[i].SetOriginToPoint((JustificationPosition)hArr[i]["Origin ID"].GetUint());
		m_HealthFrames[i].SetPosition(hArr[i]["Pos X"].GetFloat(), hArr[i]["Pos Y"].GetFloat());
		m_HealthFrames[i].SetScale(
			hArr[i]["Scale X"].GetFloat(),
			hArr[i]["Scale Y"].GetFloat()
		);
		m_HealthFrames[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	//////////////////////
	/// Ability Frames ///
	//////////////////////

	//Get health frame array for brevity
	rapidjson::Value& aArr = arr[index]["Frame Setup"]["Ability Frames"].GetArray();
	for (unsigned i(0); i < (unsigned)AbilityFrameIDs::COUNT; ++i)
	{
		m_AbilityFrames[i].SetTexture(aArr[i]["Texture Name"].GetString());
		m_AbilityFrames[i].SetFrame(aArr[i]["Frame ID"].GetUint());
		m_AbilityFrames[i].SetOriginToPoint((JustificationPosition)aArr[i]["Origin ID"].GetUint());
		m_AbilityFrames[i].SetPosition(aArr[i]["Pos X"].GetFloat(), aArr[i]["Pos Y"].GetFloat());
		m_AbilityFrames[i].SetScale(
			aArr[i]["Scale X"].GetFloat(),
			aArr[i]["Scale Y"].GetFloat()
		);
		m_AbilityFrames[i].SetLayerDepth(DRAW_DEPTH_UI_0);
	}

	////////////////////////
	/// Equipment Frames ///
	////////////////////////

	//
	//Weapons
	// 

	//Get offset for placing frames
	DirectX::XMFLOAT2 offset =
	{
		arr[index]["Frame Setup"]["Weapon Frames"]["Offset X"].GetFloat(),
		arr[index]["Frame Setup"]["Weapon Frames"]["Offset Y"].GetFloat()
	};

	for (unsigned i(0); i < (unsigned)WeaponFrameIDs::COUNT; ++i)
	{
		//
		//Main Frames
		//

		m_WeaponFrames[i].SetTexture(arr[index]["Frame Setup"]["Weapon Frames"]["Texture Name"].GetString());
		m_WeaponFrames[i].SetFrame(arr[index]["Frame Setup"]["Weapon Frames"]["Frame ID"].GetUint());
		m_WeaponFrames[i].SetOriginToPoint((JustificationPosition)arr[index]["Frame Setup"]["Weapon Frames"]["Origin ID"].GetUint());
		m_WeaponFrames[i].SetPosition(
			arr[index]["Frame Setup"]["Weapon Frames"]["Pos X"].GetFloat() + (offset.x * i),
			arr[index]["Frame Setup"]["Weapon Frames"]["Pos Y"].GetFloat() + (offset.y * i)
		);
		m_WeaponFrames[i].SetScale(
			arr[index]["Frame Setup"]["Weapon Frames"]["Scale X"].GetFloat(),
			arr[index]["Frame Setup"]["Weapon Frames"]["Scale Y"].GetFloat()
		);
		m_WeaponFrames[i].SetLayerDepth(DRAW_DEPTH_UI_0);

		//
		//Icons
		//

		m_WeaponIcons[i].SetTexture("UI_00");
		m_WeaponIcons[i].SetPosition(m_WeaponFrames[i].GetPosition());
		m_WeaponIcons[i].SetOriginToCenter();
		m_WeaponIcons[i].SetLayerDepth(DRAW_DEPTH_UI_1);

		//
		//Texts
		//

		m_WeaponTexts[i].m_Position =
		{
			m_WeaponFrames[i].GetPosition().x + arr[index]["Frame Setup"]["Weapon Frames"]["Text Offset X"].GetFloat(),
			m_WeaponFrames[i].GetPosition().y + arr[index]["Frame Setup"]["Weapon Frames"]["Text Offset Y"].GetFloat()
		};

		m_WeaponTexts[i].m_FontSF = Game::GetGame()->GetPassData().m_Spritefonts[arr[index]["Frame Setup"]["Weapon Frames"]["Text Font ID"].GetUint()].get();
		m_WeaponTexts[i].m_StringJustification = (short)arr[index]["Frame Setup"]["Weapon Frames"]["Text Origin ID"].GetInt();
		m_WeaponTexts[i].m_Colour = DirectX::Colors::AntiqueWhite;
		m_WeaponTexts[i].m_LayerDepth = DRAW_DEPTH_UI_3;
	}

	//
	//Accessory
	// 

	//Reset offset
	offset =
	{
		arr[index]["Frame Setup"]["Weapon Frames"]["Offset X"].GetFloat(),
		arr[index]["Frame Setup"]["Weapon Frames"]["Offset Y"].GetFloat()
	};

	for (unsigned i(0); i < (unsigned)AccessoryFrameIDs::COUNT; ++i)
	{
		//
		//Main Frames
		//

		m_AccessoryFrames[i].SetTexture(arr[index]["Frame Setup"]["Accessory Frames"]["Texture Name"].GetString());
		m_AccessoryFrames[i].SetFrame(arr[index]["Frame Setup"]["Accessory Frames"]["Frame ID"].GetUint());
		m_AccessoryFrames[i].SetOriginToPoint((JustificationPosition)arr[index]["Frame Setup"]["Accessory Frames"]["Origin ID"].GetUint());
		m_AccessoryFrames[i].SetPosition(
			arr[index]["Frame Setup"]["Accessory Frames"]["Pos X"].GetFloat() + (offset.x * i),
			arr[index]["Frame Setup"]["Accessory Frames"]["Pos Y"].GetFloat() + (offset.y * i)
		);
		m_AccessoryFrames[i].SetScale(
			arr[index]["Frame Setup"]["Accessory Frames"]["Scale X"].GetFloat(),
			arr[index]["Frame Setup"]["Accessory Frames"]["Scale Y"].GetFloat()
		);
		m_AccessoryFrames[i].SetLayerDepth(DRAW_DEPTH_UI_0);

		//
		//Icons
		//

		m_AccessoryIcons[i].SetTexture("UI_00");
		m_AccessoryIcons[i].SetPosition(m_AccessoryFrames[i].GetPosition());
		m_AccessoryIcons[i].SetOriginToCenter();
		m_AccessoryIcons[i].SetLayerDepth(DRAW_DEPTH_UI_1);

		//
		//Texts
		//

		m_AccessoryTexts[i].m_Position =
		{
			m_AccessoryFrames[i].GetPosition().x + arr[index]["Frame Setup"]["Accessory Frames"]["Text Offset X"].GetFloat(),
			m_AccessoryFrames[i].GetPosition().y + arr[index]["Frame Setup"]["Accessory Frames"]["Text Offset Y"].GetFloat()
		};

		m_AccessoryTexts[i].m_FontSF = Game::GetGame()->GetPassData().m_Spritefonts[arr[index]["Frame Setup"]["Accessory Frames"]["Text Font ID"].GetUint()].get();
		m_AccessoryTexts[i].m_StringJustification = (short)arr[index]["Frame Setup"]["Accessory Frames"]["Text Origin ID"].GetInt();
		m_AccessoryTexts[i].m_Colour = DirectX::Colors::AntiqueWhite;
		m_AccessoryTexts[i].m_LayerDepth = DRAW_DEPTH_UI_3;
	}

	/////////////
	/// Texts ///
	/////////////

	for (unsigned i(0); i < (unsigned)TextIDs::COUNT; ++i)
	{
		m_Texts[i].m_Position =
		{
			arr[index]["Texts"][i]["Pos X"].GetFloat(),
			arr[index]["Texts"][i]["Pos Y"].GetFloat()
		};
		m_Texts[i].m_FontSF = Game::GetGame()->GetPassData().m_Spritefonts[arr[index]["Texts"][i]["Font ID"].GetUint()].get();
		m_Texts[i].m_StringJustification = (short)arr[index]["Texts"][i]["Origin ID"].GetInt();
		m_Texts[i].m_Colour = DirectX::Colors::White;
		m_Texts[i].m_LayerDepth = DRAW_DEPTH_UI_3;
	}

	//Post
	m_Texts[(unsigned)TextIDs::REGEN].m_Colour = DirectX::Colors::LawnGreen;

	//Set flags
	for (unsigned i(0); i < (unsigned)WeaponFrameIDs::COUNT; ++i)
	{
		m_WeaponFlags[i] = false;
		m_AccessoryFlags[i] = false;
	}
}

void UI_PlayerInfo::SyncUIWithPlayer()
{
	//Get player equipment for brevity
	auto* wArr = m_Player->GetWeapons();
	auto* aArr = m_Player->GetAccessories();

	//Cycle through player weapons, setting icon with information provided
	for (unsigned i(0); i < (unsigned)WeaponFrameIDs::COUNT; ++i)
	{
		if (wArr[i].GetWeaponID() != WeaponTypeID::UNDEFINED)
		{
			m_WeaponIcons[i].SetTexture(wArr[i].GetIconData().m_TextureName);
			m_WeaponIcons[i].SetFrame(wArr[i].GetIconData().m_Frame);
			m_WeaponIcons[i].SetScale(
				wArr[i].GetIconData().m_ScaleX,
				wArr[i].GetIconData().m_ScaleY
			);

			m_WeaponTexts[i].m_DrawString.clear();
			m_WeaponTexts[i].m_DrawString.append("Lv:").append(std::to_string(wArr[i].GetLevelingSystem().m_WeaponLevel + 1));
		
			m_WeaponFlags[i] = true;
		}
		else
		{
			m_WeaponFlags[i] = false;
		}
	}

	//Cycle through player accessories, setting icon with information provided
	for (unsigned i(0); i < (unsigned)AccessoryFrameIDs::COUNT; ++i)
	{
		if (aArr[i].GetAccessoryID() != AccessoryTypeID::UNDEFINED)
		{
			m_AccessoryIcons[i].SetTexture(aArr[i].GetIconData().m_TextureName);
			m_AccessoryIcons[i].SetFrame(aArr[i].GetIconData().m_Frame);
			m_AccessoryIcons[i].SetScale(
				aArr[i].GetIconData().m_ScaleX,
				aArr[i].GetIconData().m_ScaleY
			);

			m_AccessoryTexts[i].m_DrawString.clear();
			m_AccessoryTexts[i].m_DrawString.append("Lv:").append(std::to_string(aArr[i].GetLevelingSystem().m_WeaponLevel + 1));
		
			m_AccessoryFlags[i] = true;
		}
		else
		{
			m_AccessoryFlags[i] = false;
		}
	}
}

void UI_PlayerInfo::SetPlayerPointer(Entity_Player* player)
{
	msg_assert(player != nullptr, "SetPlayerPointer(): Invalid player pointer.");

	if (!m_Player)
		m_Player = player;
}
