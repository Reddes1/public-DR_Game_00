#include "SM_FloatingCombatText.h"

#include "Game.h"
#include "SpriteFont.h"
#include "SimpleMath.h"

void SM_FloatingCombatText::Update(GameTimer& gt)
{
	for (unsigned i(0); i < DoD_FCT::MAX_COUNT; ++i)
	{
		m_Data->m_Lifetime[i] -= gt.DeltaTime();
		m_Data->m_Colour[i].w = m_Data->m_Lifetime[i];
	}
}

void SM_FloatingCombatText::Render()
{
	//Get pass data from game
	PassData& passData = Game::GetGame()->GetPassData();

	for (unsigned i(0); i < DoD_FCT::MAX_COUNT; ++i)
	{
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };
		
		if (m_Data->m_Lifetime[i] > 0.f)
		{
			CalculateStringJustifyPosition(
				passData.m_Spritefonts[m_Data->m_FontID[i]].get(),
				(short)JustificationPosition::CENTER,
				std::string(m_Data->m_DrawString[i]),
				origin
			);

			DirectX::SimpleMath::Vector4 colour = DirectX::Colors::Black;
			colour.w = m_Data->m_Colour[i].w;

			//Outline Draws
			passData.m_Spritefonts[m_Data->m_FontID[i]]->DrawString(
				passData.m_Spritebatches[SpritebatchDefinitions::MAIN_SB].get(),
				m_Data->m_DrawString[i],
				DirectX::SimpleMath::Vector2(m_Data->m_PosX[i] + 1.f, m_Data->m_PosY[i] + 1.f),
				colour,
				0.f,
				origin,
				1.0f,
				DirectX::SpriteEffects_None,
				DRAW_DEPTH_MISC_HIGH_8
			);
			passData.m_Spritefonts[m_Data->m_FontID[i]]->DrawString(
				passData.m_Spritebatches[SpritebatchDefinitions::MAIN_SB].get(),
				m_Data->m_DrawString[i],
				DirectX::SimpleMath::Vector2(m_Data->m_PosX[i] - 1.f, m_Data->m_PosY[i] + 1.f),
				colour,
				0.f,
				origin,
				1.0f,
				DirectX::SpriteEffects_None,
				DRAW_DEPTH_MISC_HIGH_8
			);
			passData.m_Spritefonts[m_Data->m_FontID[i]]->DrawString(
				passData.m_Spritebatches[SpritebatchDefinitions::MAIN_SB].get(),
				m_Data->m_DrawString[i],
				DirectX::SimpleMath::Vector2(m_Data->m_PosX[i] - 1.f, m_Data->m_PosY[i] - 1.f),
				colour,
				0.f,
				origin,
				1.0f,
				DirectX::SpriteEffects_None,
				DRAW_DEPTH_MISC_HIGH_8
			);
			passData.m_Spritefonts[m_Data->m_FontID[i]]->DrawString(
				passData.m_Spritebatches[SpritebatchDefinitions::MAIN_SB].get(),
				m_Data->m_DrawString[i],
				DirectX::SimpleMath::Vector2(m_Data->m_PosX[i] + 1.f, m_Data->m_PosY[i] - 1.f),
				colour,
				0.f,
				origin,
				1.0f,
				DirectX::SpriteEffects_None,
				DRAW_DEPTH_MISC_HIGH_8
			);

			//Original String
			passData.m_Spritefonts[m_Data->m_FontID[i]]->DrawString(
				passData.m_Spritebatches[SpritebatchDefinitions::MAIN_SB].get(),
				m_Data->m_DrawString[i],
				DirectX::XMFLOAT2(m_Data->m_PosX[i], m_Data->m_PosY[i]),
				m_Data->m_Colour[i],
				0.f,
				origin,
				0.9f,
				DirectX::SpriteEffects_None,
				DRAW_DEPTH_MISC_HIGH_9
			);
		}
	}
}

bool SM_FloatingCombatText::SetupFreeFCTEntry(DirectX::XMFLOAT2& pos, short dmgValue, float yOffset)
{
	//Find free text entry
	for (unsigned i(0); i < DoD_FCT::MAX_COUNT; ++i)
	{
		if (m_Data->m_Lifetime[i] <= 0.f)
		{
			//Setup position
			m_Data->m_PosX[i] = pos.x;
			m_Data->m_PosY[i] = pos.y + yOffset;

			m_Data->m_Lifetime[i] = DoD_FCT::LIFE_TIME_MAX;
			m_Data->m_Colour[i] = DirectX::Colors::White;

			strcpy(m_Data->m_DrawString[i], std::to_string(dmgValue).c_str());
			return true;
		}
	}
	//No free text entry found
	return false;
}

bool SM_FloatingCombatText::SetupFreeFCTEntry(DirectX::XMFLOAT2& pos, short value, float yOffset, DirectX::SimpleMath::Vector4& colour)
{
	//Find free text entry
	for (unsigned i(0); i < DoD_FCT::MAX_COUNT; ++i)
	{
		if (m_Data->m_Lifetime[i] <= 0.f)
		{
			//Setup position
			m_Data->m_PosX[i] = pos.x;
			m_Data->m_PosY[i] = pos.y + yOffset;

			m_Data->m_Lifetime[i] = DoD_FCT::LIFE_TIME_MAX;
			m_Data->m_Colour[i] = colour;

			strcpy(m_Data->m_DrawString[i], std::to_string(value).c_str());
			return true;
		}
	}
	//No free text entry found
	return false;
}

void SM_FloatingCombatText::Init()
{
	m_Data = new DoD_FCT();

	//Setup data
	for (unsigned i(0); i < DoD_FCT::MAX_COUNT; ++i)
	{
		m_Data->m_PosX[i] = 0.f;
		m_Data->m_PosY[i] = 0.f;
		strcpy(m_Data->m_DrawString[i], "00000");
		m_Data->m_Lifetime[i] = -1.f;
		m_Data->m_FontID[i] = FontDescriptions::EXPRESSION_PRO_32;

		m_Data->m_Colour[i] = DirectX::Colors::White;
	}
}

void SM_FloatingCombatText::Release()
{
	delete m_Data;
	m_Data = nullptr;
}
