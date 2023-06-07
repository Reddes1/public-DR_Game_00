#include "EM_MultiSprite_Type1.h"

EM_MultiSprite_Type1::EM_MultiSprite_Type1(std::shared_ptr<SpriteTexture> sprTex)
	:EM_Sprite(sprTex)
{

}

EM_MultiSprite_Type1::EM_MultiSprite_Type1(std::shared_ptr<SpriteTexture> sprTex, unsigned int subSprCount)
	: EM_Sprite(sprTex)
{
	m_SubSprites.reserve(subSprCount);
	m_SubSprites.insert(m_SubSprites.end(), subSprCount, SubSpriteData());
}

void EM_MultiSprite_Type1::Draw(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Draw Base Sprite
	EM_Sprite::Draw(sb, heap);

	for (auto& s : m_SubSprites)
	{
		if(s.m_DrawEnabled)
		{ 
			sb.Draw(heap->GetGpuHandle(m_DrawData.m_SRVIndex),
				m_DrawData.m_TexSize,
				m_DrawData.m_Pos,
				&s.m_FrameRect,
				m_DrawData.M_Colour,
				m_DrawData.m_Rotation,
				m_DrawData.m_Origin,
				m_DrawData.m_Scale,
				m_DrawData.m_SprEffect,
				m_DrawData.m_LayerDepth
			);
		}
	}
}

void EM_MultiSprite_Type1::CreateSubSprites(unsigned count)
{
	m_SubSprites.reserve(count);
	m_SubSprites.insert(m_SubSprites.end(), count, SubSpriteData());
}

void EM_MultiSprite_Type1::SetSubSpriteFrameAtIndex(int index, int frameIndex)
{
	assert(index >= 0 && index <= m_SubSprites.size());
	assert(frameIndex >= 0 && frameIndex <= m_Texture->m_Frames.size());

	m_SubSprites[index].m_FrameRect = m_Texture->m_Frames[frameIndex];
}

void EM_MultiSprite_Type1::SetSubSpriteRectAtIndex(int index, RECT& newRect)
{
	assert(index >= 0 && index <= m_SubSprites.size());
	m_SubSprites[index].m_FrameRect = newRect;
}

void EM_MultiSprite_Type1::DefaultSubSpriteAtIndex(int index)
{
	m_SubSprites[index].m_FrameRect = m_DrawData.m_FrameRect;
}
