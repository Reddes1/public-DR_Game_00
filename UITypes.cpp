#include "UITypes.h"


void FixedIndexNavUI::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	if (m_Enabled)
	{
		//Preserve old positions
		DirectX::XMFLOAT2 mainPos = m_MainSprite.GetPosition();
		DirectX::XMFLOAT2 cursorPos = m_CursorSprite.GetPosition();

		//Add adjustment
		m_MainSprite.AddToPosition(m_PositionAdj);
		m_CursorSprite.AddToPosition(m_PositionAdj);

		//Draw Main UI element first
		m_MainSprite.Draw(sb, heap);
		//Draw Cursor
		m_CursorSprite.Draw(sb, heap);

		//Set old positions
		m_MainSprite.SetPosition(mainPos);
		m_CursorSprite.SetPosition(cursorPos);

		//Calculation values
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };
		DirectX::XMVECTOR pos = { 0, 0 };
		int count = 0;

		//Draw through frames
		for (int x(0); x < m_MaxIndex.x + 1; ++x)
		{
			for (int y(0); y < m_MaxIndex.y + 1; ++y)
			{
				//count can go beyond the size of the container if the last column isn't full, so check out early if so
				if (count > m_Data.size())
				{
					y = m_MaxIndex.y + 1;
					x = m_MaxIndex.x + 1;
				}
				else
				{
					//Calculate string position & origin
					pos = { m_TextAnchorPos.x + (m_Offsets.x * x) + m_PositionAdj.x, m_TextAnchorPos.y + (m_Offsets.y * y) + m_PositionAdj.y };
					CalculateStringJustifyPosition(
						m_Data[count].m_FontSF,
						m_Data[count].m_StringJustification,
						m_Data[count].m_DrawString,
						origin
					);

					//Draw String
					m_Data[count].m_FontSF->DrawString(
						&sb,
						m_Data[count].m_DrawString.c_str(),
						pos,
						m_Data[count].m_Colour,
						m_Data[count].m_Rotation,
						origin,
						m_Data[count].m_Scale,
						m_Data[count].m_Effect,
						m_Data[count].m_LayerDepth
					);
					++count;
				}
			}
		}


	}
}

void FixedIndexNavUI::RenderUIOnly(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	if (m_Enabled)
	{
		//Preserve old position
		DirectX::XMFLOAT2 mainPos = m_MainSprite.GetPosition();

		//Add adjustment
		m_MainSprite.AddToPosition(m_PositionAdj);

		//Draw
		m_MainSprite.Draw(sb, heap);

		//Set old positions back
		m_MainSprite.SetPosition(mainPos);
	}		
}

void FixedIndexNavUI::RenderTextOnly(DirectX::SpriteBatch& sb)
{
	if (m_Enabled)
	{
		//Calculation values
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };
		DirectX::XMVECTOR pos = { 0, 0 };
		int count = 0;

		//Draw through frames
		for (int x(0); x < m_MaxIndex.x + 1; ++x)
		{
			for (int y(0); y < m_MaxIndex.y + 1; ++y)
			{
				//count can go beyond the size of the container if the last column isn't full, so check out early if so
				if (count > m_Data.size())
				{
					y = m_MaxIndex.y + 1;
					x = m_MaxIndex.x + 1;
				}
				else
				{
					//Calculate string position & origin
					pos = { m_TextAnchorPos.x + (m_Offsets.x * x) + m_PositionAdj.x, m_TextAnchorPos.y + (m_Offsets.y * y) + m_PositionAdj.y };
					CalculateStringJustifyPosition(
						m_Data[count].m_FontSF,
						m_Data[count].m_StringJustification,
						m_Data[count].m_DrawString,
						origin
					);

					//Draw String
					m_Data[count].m_FontSF->DrawString(
						&sb,
						m_Data[count].m_DrawString.c_str(),
						pos,
						m_Data[count].m_Colour,
						m_Data[count].m_Rotation,
						origin,
						m_Data[count].m_Scale,
						m_Data[count].m_Effect,
						m_Data[count].m_LayerDepth
					);
					++count;
				}
			}
		}
	}
}

void FixedIndexNavUI::MoveCursorUp()
{
	//dec index
	--m_ActiveIndex.y;
	//Check if out of range
	if (m_ActiveIndex.y < 0)
		m_ActiveIndex.y = 0;

	//Set position
	m_CursorSprite.SetPosition(m_CursorAnchorPos.x + (m_Offsets.x * m_ActiveIndex.x),
		m_CursorAnchorPos.y + (m_Offsets.y * m_ActiveIndex.y));
}

void FixedIndexNavUI::MoveCursorDown()
{
	//dec index
	++m_ActiveIndex.y;
	//Check if out of range
	if (m_ActiveIndex.y > m_MaxIndex.y)
		m_ActiveIndex.y = m_MaxIndex.y;

	//Set position
	m_CursorSprite.SetPosition(m_CursorAnchorPos.x + (m_Offsets.x * m_ActiveIndex.x),
		m_CursorAnchorPos.y + (m_Offsets.y * m_ActiveIndex.y));
}

void FixedIndexNavUI::MoveCursorLeft()
{
	//dec index
	--m_ActiveIndex.x;
	//Check if out of range
	if (m_ActiveIndex.x < 0)
		m_ActiveIndex.x = 0;

	//Set position
	m_CursorSprite.SetPosition(m_CursorAnchorPos.x + (m_Offsets.x * m_ActiveIndex.x),
		m_CursorAnchorPos.y + (m_Offsets.y * m_ActiveIndex.y));
}

void FixedIndexNavUI::MoveCursorRight()
{
	//dec index
	++m_ActiveIndex.x;
	//Check if out of range
	if (m_ActiveIndex.x > m_MaxIndex.x)
		m_ActiveIndex.x = m_MaxIndex.x;

	//Set position
	m_CursorSprite.SetPosition(m_CursorAnchorPos.x + (m_Offsets.x * m_ActiveIndex.x),
		m_CursorAnchorPos.y + (m_Offsets.y * m_ActiveIndex.y));
}

void FixedIndexNavUI::SetCursorToDefault()
{
	//Set to default
	m_ActiveIndex = m_DefaultIndex;
	//Set position
	m_CursorSprite.SetPosition(m_CursorAnchorPos.x + (m_Offsets.x * m_ActiveIndex.x),
		m_CursorAnchorPos.y + (m_Offsets.y * m_ActiveIndex.y));
}

void FixedIndexNavUI::ResetAllStrings()
{
	for (auto& d : m_Data)
		d.ResetString();
}

void FixedIndexNavUI::AppendStringAtIndex(std::string& str, unsigned int index)
{
	m_Data[index].m_DrawString.append(str);
}

void FixedIndexNavUI::AppendStringAtIndex(std::stringstream& str, unsigned int index)
{
	m_Data[index].m_DrawString.append(str.str());
}

void FixedIndexNavUI::Release()
{
	m_Data.clear();
}

void NonNavUI::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	if (m_Enabled)
	{
		//Preserve old position
		DirectX::XMFLOAT2 mainPos = m_Sprite.GetPosition();

		//Add adjustment
		m_Sprite.AddToPosition(m_PositionAdj);

		//Draw Main UI element first
		m_Sprite.Draw(sb, heap);

		//Set old position
		m_Sprite.SetPosition(mainPos);

		//Hold origin calc value here
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };
		//Draw through each string
		for (auto& d : m_Data)
		{
			CalculateStringJustifyPosition(
				d.m_FontSF,
				d.m_StringJustification,
				d.m_DrawString,
				origin
			);

			//Calc new position using base + global
			DirectX::XMFLOAT2 pos = { d.m_Position.x + m_PositionAdj.x, d.m_Position.y + m_PositionAdj.y };

			//Draw String
			d.m_FontSF->DrawString(
				&sb,
				d.m_DrawString.c_str(),
				pos,
				d.m_Colour,
				d.m_Rotation,
				origin,
				d.m_Scale,
				d.m_Effect,
				d.m_LayerDepth
			);
		}

	}
}

void NonNavUI::RenderUIOnly(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	if (m_Enabled)
	{
		//Preserve old position
		DirectX::XMFLOAT2 mainPos = m_Sprite.GetPosition();

		//Add adjustment
		m_Sprite.AddToPosition(m_PositionAdj);

		//Draw
		m_Sprite.Draw(sb, heap);

		//Set old positions back
		m_Sprite.SetPosition(mainPos);
	}
}

void NonNavUI::RenderTextOnly(DirectX::SpriteBatch& sb)
{
	if (m_Enabled)
	{
		//Hold origin calc value here
		DirectX::SimpleMath::Vector2 origin = { 0, 0 };
		//Draw through each string
		for (auto& d : m_Data)
		{
			CalculateStringJustifyPosition(
				d.m_FontSF,
				d.m_StringJustification,
				d.m_DrawString,
				origin
			);

			//Calc new position using base + global
			DirectX::XMFLOAT2 pos = { d.m_Position.x + m_PositionAdj.x, d.m_Position.y + m_PositionAdj.y };

			//Draw String
			d.m_FontSF->DrawString(
				&sb,
				d.m_DrawString.c_str(),
				d.m_Position,
				d.m_Colour,
				d.m_Rotation,
				origin,
				d.m_Scale,
				d.m_Effect,
				d.m_LayerDepth
			);
		}
	}

}

void NonNavUI::ResetAllStrings()
{
	for (auto& d : m_Data)
		d.ResetString();
}

void NonNavUI::AppendStringAtIndex(std::string& str, unsigned int index)
{
	m_Data[index].m_DrawString.append(str);
}

void NonNavUI::AppendStringAtIndex(std::stringstream& str, unsigned int index)
{
	m_Data[index].m_DrawString.append(str.str());
}

void NonNavUI::Release()
{
	m_Data.clear();
}
