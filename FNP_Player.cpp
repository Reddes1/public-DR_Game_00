#include "FNP_Player.h"

#include "Game.h"				  //Manager Access
#include "RapidJSONLoaderUtils.h" //Loader Utils
#include "EngineDefinitions.h"


void UI_FNP_Player::Update(GameTimer& gt)
{
	//////////////////
	/// Health Bar ///
	//////////////////

	//Get player health and regen values
	int cHP = m_Player->GetStats().m_Health;
	int mHP = m_Player->GetStats().m_MaxHealth;

	//Adjust inner bar length relative to player health
	float xScale = ((float)cHP / (float)mHP);
	//Clamp Scale
	if (xScale > 1.f)
		xScale = 1.f;
	m_Frames[(unsigned)BarIDs::HEALTH_BAR].SetScale(
		xScale,
		m_Frames[(unsigned)BarIDs::HEALTH_BAR].GetScale().y
	);

	///////////////////
	/// Ability Bar ///
	///////////////////

	int cCD = static_cast<int>(std::round(m_Player->GetAbility().GetCurrentCD()));
	int mCD = static_cast<int>(std::round(m_Player->GetAbility().GetStats().m_Cooldown));

	xScale = ((float)cCD / (float)mCD);
	if (xScale > 1.f)
		xScale = 1.f;
	m_Frames[(unsigned)BarIDs::ABILITY_BAR].SetScale(
		xScale,
		m_Frames[(unsigned)BarIDs::ABILITY_BAR].GetScale().y
	);
}

void UI_FNP_Player::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Draw Sprites
	for (auto& a : m_Frames)
		a.Draw(sb, heap);
}

void UI_FNP_Player::InitialiseUI(std::string& fp, unsigned index)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Get array index for brevity
	rapidjson::Value& arr = doc["Custom UI Elements"].GetArray();

	//Get frames data for brevity
	rapidjson::Value& fArr = arr[index]["Frame Setup"].GetArray();
	for (unsigned i(0); i < (unsigned)BarIDs::COUNT; ++i)
	{
		m_Frames[i].SetTexture(fArr[i]["Texture Name"].GetString());
		m_Frames[i].SetFrame(fArr[i]["Frame ID"].GetUint());
		m_Frames[i].SetOriginToPoint((JustificationPosition)fArr[i]["Origin ID"].GetUint());
		m_Frames[i].SetPosition(fArr[i]["Pos X"].GetFloat(), fArr[i]["Pos Y"].GetFloat());
		m_Frames[i].SetScale(
			fArr[i]["Scale X"].GetFloat(),
			fArr[i]["Scale Y"].GetFloat()
		);
		m_Frames[i].SetLayerDepth(DRAW_DEPTH_UI_0);

		if(i < 2)
			m_Frames[i].SetLayerDepth(DRAW_DEPTH_UI_0);
		else
			m_Frames[i].SetLayerDepth(DRAW_DEPTH_UI_1);

	}


}

void UI_FNP_Player::SetPlayerPointer(Entity_Player* player)
{
	msg_assert(player != nullptr, "SetPlayerPointer(): Invalid player pointer.");

	if (!m_Player)
		m_Player = player;
}
