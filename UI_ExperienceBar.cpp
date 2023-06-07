#include "UI_ExperienceBar.h"

#include "Game.h"				  //Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils

void UI_ExperienceBar::Update(GameTimer& gt)
{
	auto& progMgr = Game::GetGame()->GetGameplayManager().GetProgressManager();
	auto& spawnMgr = Game::GetGame()->GetGameplayManager().GetSpawnManager();

	//Get the difference between current XP and next level and represent as bar
	float xpReq = (float)progMgr.GetExperienceForLevel(progMgr.GetCurrentLevel()) - (float)progMgr.GetExperienceForLevel(progMgr.GetCurrentLevel()-1);
	//Scale XP bar based on xp
	float xScale = ((float)progMgr.GetCurrentExperience() - (float)progMgr.GetExperienceForLevel(progMgr.GetCurrentLevel() - 1)) / xpReq;
	m_Frames[(unsigned)FrameIDs::BAR].SetScale(xScale, m_Frames[(unsigned)FrameIDs::BAR].GetScale().y);

	//Update draw string with current level
	m_Texts[(unsigned)TextIDs::LEVEL].m_DrawString.clear();
	m_Texts[(unsigned)TextIDs::LEVEL].m_DrawString.append("Lv: ").append(std::to_string(progMgr.GetCurrentLevel()));

	//Update draw string with current time
	m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.clear();

	unsigned mins = spawnMgr.GetCurrentWaveIndex();
	unsigned secs = (unsigned)std::floor(spawnMgr.GetElapsed());

	//Setup string within constraints of a timer
	if(mins <= 9)
		m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.append("0").append(std::to_string(mins));
	else
		m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.append(std::to_string(mins));
	m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.append(":");

	if (secs <= 9)
		m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.append("0").append(std::to_string(secs));
	else
		m_Texts[(unsigned)TextIDs::TIMER].m_DrawString.append(std::to_string(secs));

	m_Texts[(unsigned)TextIDs::KILL_COUNT].m_DrawString.clear();
	m_Texts[(unsigned)TextIDs::KILL_COUNT].m_DrawString.append("KC: ").append(std::to_string(progMgr.GetKillCount()));
}

void UI_ExperienceBar::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Draw Sprites
	for (auto& a : m_Frames)
		a.Draw(sb, heap);

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
			DRAW_DEPTH_MISC_HIGH_8
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
			DRAW_DEPTH_MISC_HIGH_8
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
			DRAW_DEPTH_MISC_HIGH_8
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
			DRAW_DEPTH_MISC_HIGH_8
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
}

void UI_ExperienceBar::InitialiseUI(std::string& fp, unsigned index)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Get array index for brevity
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	//////////////
	/// Frames ///
	//////////////

	//Get array for brevity
	rapidjson::Value& fArr = doc["Custom UI Elements"][index]["Frame Setup"].GetArray();
	for (unsigned i(0); i < (unsigned)FrameIDs::COUNT; ++i)
	{
		m_Frames[i].SetTexture(fArr[i]["Texture Name"].GetString());
		m_Frames[i].SetFrame(fArr[i]["Frame ID"].GetUint());
		m_Frames[i].SetOriginToPoint((JustificationPosition)fArr[i]["Origin ID"].GetUint());
		m_Frames[i].SetPosition(fArr[i]["Pos X"].GetFloat(), fArr[i]["Pos Y"].GetFloat());
		m_Frames[i].SetScale(
			fArr[i]["Scale X"].GetFloat(),
			fArr[i]["Scale Y"].GetFloat()
		);
		m_Frames[i].SetLayerDepth(DRAW_DEPTH_MISC_HIGH_7);
	}

	m_Frames[(unsigned)FrameIDs::BAR].SetLayerDepth(DRAW_DEPTH_MISC_HIGH_8);

	////////////
	/// Text ///
	////////////

	auto& data = Game::GetGame()->GetPassData();
	//Get array for brevity
	rapidjson::Value& tArr = doc["Custom UI Elements"][index]["Texts"].GetArray();
	for (unsigned i(0); i < (unsigned)TextIDs::COUNT; ++i)
	{
		m_Texts[i].m_Position =
		{
			tArr[i]["Pos X"].GetFloat(),
			tArr[i]["Pos Y"].GetFloat()
		};
		m_Texts[i].m_StringJustification = (short)tArr[i]["Origin ID"].GetInt();
		m_Texts[i].m_Colour = DirectX::Colors::White;
		m_Texts[i].m_FontSF = data.m_Spritefonts[tArr[i]["Font ID"].GetUint()].get();
		m_Texts[i].m_LayerDepth = DRAW_DEPTH_MISC_HIGH_9;
	}
}
