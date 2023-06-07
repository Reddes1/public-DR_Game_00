#include "Game.h"
#include "EM_Sprite.h"

#include <assert.h>

EM_Sprite::EM_Sprite(std::shared_ptr<SpriteTexture> texPtr)
{
	SetTexture(texPtr);
}

void EM_Sprite::Draw(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	batch.Draw(heap->GetGpuHandle(m_DrawData.m_SRVIndex),
		m_DrawData.m_TexSize,
		m_DrawData.m_Pos,
		&m_DrawData.m_FrameRect,
		m_DrawData.M_Colour,
		m_DrawData.m_Rotation,
		m_DrawData.m_Origin,
		m_DrawData.m_Scale,
		m_DrawData.m_SprEffect,
		m_DrawData.m_LayerDepth
	);
}

void EM_Sprite::DefaultSprite()
{
	DirectX::XMFLOAT2 windowDim = {
		static_cast<float>(Game::GetGame()->GetWindowData().m_Width),
		static_cast<float>(Game::GetGame()->GetWindowData().m_Height)
	};

	//Normalise the reset to safe values
	m_DrawData.m_TexSize = DirectX::GetTextureSize(m_Texture.get()->m_Resource.Get());
	m_DrawData.m_FrameRect = { 0, 0, static_cast<long>(m_DrawData.m_TexSize.x), static_cast<long>(m_DrawData.m_TexSize.y) };
	m_DrawData.m_Origin = { m_DrawData.m_TexSize.x * 0.5f, m_DrawData.m_TexSize.y * 0.5f };
	m_DrawData.m_Pos = { windowDim.x * 0.5f, windowDim.y * 0.5f };
	m_DrawData.M_Colour = { 1, 1, 1, 1 };
	m_DrawData.m_LayerDepth = 1.f;
	m_DrawData.m_Rotation = 0.f;
	m_DrawData.m_Scale = { 1.0f, 1.0f };
	m_DrawData.m_SprEffect = DirectX::SpriteEffects::SpriteEffects_None;
}

void EM_Sprite::SetTexture(std::shared_ptr<SpriteTexture> tex, bool setDrawValues)
{
	assert(tex);
	m_Texture = tex;
	m_DrawData.m_SRVIndex = m_Texture.get()->m_SrvIndex;
	m_DrawData.m_TexSize = DirectX::GetTextureSize(m_Texture.get()->m_Resource.Get());
	m_DrawData.m_FrameRect = { 
		0,
		0,
		static_cast<long>(m_DrawData.m_TexSize.x),
		static_cast<long>(m_DrawData.m_TexSize.y)
	};
}

void EM_Sprite::SetTexture(std::string texName, bool setDrawValues)
{
	m_Texture = Game::GetGame()->GetTextureManager().GetSpriteTextureData(texName);
	assert(m_Texture.get());
	m_DrawData.m_SRVIndex = m_Texture.get()->m_SrvIndex;
	m_DrawData.m_TexSize = DirectX::GetTextureSize(m_Texture.get()->m_Resource.Get());
	m_DrawData.m_FrameRect = { 
		0,
		0,
		static_cast<long>(m_DrawData.m_TexSize.x),
		static_cast<long>(m_DrawData.m_TexSize.y)
	};
}

void EM_Sprite::SetFrame(int index)
{
	assert(index >= 0 && index <= m_Texture->m_Frames.size());
	m_DrawData.m_FrameRect = m_Texture->m_Frames[index];
	m_DrawData.m_Origin = m_Texture->m_Origins[index];
}

void EM_Sprite::AddToPosition(DirectX::XMFLOAT2& addPos)
{
	m_DrawData.m_Pos.x += addPos.x;
	m_DrawData.m_Pos.y += addPos.y;

}

void EM_Sprite::AddToPosition(float addX, float addY)
{
	m_DrawData.m_Pos.x += addX;
	m_DrawData.m_Pos.y += addY;
}

void EM_Sprite::SetAllDrawValues(int srvIndex, DirectX::XMUINT2& textureSize,
	DirectX::XMFLOAT2& pos, RECT& rect, DirectX::XMVECTOR& colour,
	float rotation, DirectX::XMFLOAT2& origin, DirectX::XMFLOAT2& scale,
	DirectX::SpriteEffects effect, float layerDepth)
{
	m_DrawData.m_SRVIndex = srvIndex;
	m_DrawData.m_TexSize = textureSize;
	m_DrawData.m_Pos = pos;
	m_DrawData.m_FrameRect = rect;
	m_DrawData.M_Colour = colour;
	m_DrawData.m_Rotation = rotation;
	m_DrawData.m_Origin = origin;
	m_DrawData.m_Scale = scale;
	m_DrawData.m_SprEffect = effect;
	m_DrawData.m_LayerDepth = layerDepth;
}

void EM_Sprite::SetOriginToPoint(JustificationPosition posID)
{
	switch (static_cast<short>(posID))
	{
	case static_cast<short>(JustificationPosition::LEFT_TOP):
		m_DrawData.m_Origin = { 0.f, 0.f };
		break;

	case static_cast<short>(JustificationPosition::LEFT):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.0f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.5f
		};
		break;

	case static_cast<short>(JustificationPosition::LEFT_BOTTOM):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.0f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 1.0f
		};
		break;

	case static_cast<short>(JustificationPosition::RIGHT_TOP):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 1.0f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.0f
		};
		break;

	case static_cast<short>(JustificationPosition::RIGHT):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 1.0f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.5f
		};
		break;

	case static_cast<short>(JustificationPosition::RIGHT_BOTTOM):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 1.0f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 1.0f
		};
		break;

	case static_cast<short>(JustificationPosition::TOP):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.5f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.0f
		};
		break;

	case static_cast<short>(JustificationPosition::BOTTOM):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.5f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 1.0f
		};
		break;

	case static_cast<short>(JustificationPosition::CENTER):
		m_DrawData.m_Origin = {
			(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.5f,
			(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.5f
		};
		break;

	default:
		m_DrawData.m_Origin = { 0.f, 0.f };
	}

}

void EM_Sprite::SetOriginToCenter()
{
	m_DrawData.m_Origin = {
		(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left) * 0.5f,
		(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top) * 0.5f
	};
}

DirectX::SimpleMath::Vector4 EM_Sprite::GetRectCollision()
{
	return DirectX::SimpleMath::Vector4(
		m_DrawData.m_Pos.x,	m_DrawData.m_Pos.y,
		GetScaleAdjustedRectX(), GetScaleAdjustedRectY()
	);
}
