#include "MT_Type1.h"

void MT_Type1::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	for (unsigned i(0); i < TileSpriteIndexes::TS_COUNT; ++i)
	{
		if (m_Enabled[i])
			m_Sprites[i].Draw(sb, heap);
	}
}

void MT_Type1::Update(float dTime)
{
	for (unsigned i(0); i < TileSpriteIndexes::TS_COUNT; ++i)
	{
		if (m_Enabled[i])
			m_Animator[i].UpdateAnimation(dTime);
	}
}

void MT_Type1::EnableHighlight()
{
	m_Enabled[TileSpriteIndexes::TS_HIGHLIGHT_TILE_00] = true;
}

void MT_Type1::EnableHighlight(unsigned index)
{
	m_Sprites[TileSpriteIndexes::TS_HIGHLIGHT_TILE_00].SetFrame(index);
	m_Enabled[TileSpriteIndexes::TS_HIGHLIGHT_TILE_00] = true;
}

void MT_Type1::DisableHighlight()
{
	m_Enabled[TileSpriteIndexes::TS_HIGHLIGHT_TILE_00] = false;
}

MT_Animator& MT_Type1::GetAnimatorAtIndex(unsigned index)
{
	assert(index <= TileSpriteIndexes::TS_COUNT);
	return m_Animator[index];
}

EM_Sprite& MT_Type1::GetSpriteAtIndex(unsigned index)
{
	assert(index <= TileSpriteIndexes::TS_COUNT);
	return m_Sprites[index];
}

MT_Type1* MT_Type1::GetNeighbourTileAtIndex(unsigned int index)
{
	assert(index <= TNIndexes::TNI_COUNT);
	return m_NeighbourTiles[index];
}

bool MT_Type1::GetEnabledStateAtIndex(unsigned int index)
{
	assert(index <= TileSpriteIndexes::TS_COUNT);
	return m_Enabled[index];
}

void MT_Type1::SetNeighbourTileAtIndex(unsigned int index, MT_Type1* tile)
{
	assert(index <= 8);
	m_NeighbourTiles[index] = tile;
}

void MT_Type1::ExtractPropertiesFromMapTileProperties(TMM_Properties_Type1& mapProps)
{
	m_Properties.m_Name = mapProps.m_LoadedProperties.m_Name;
	m_Properties.m_Collideable = mapProps.m_LoadedProperties.m_Collideable;
	m_Properties.m_ColliosnLayerID = mapProps.m_LoadedProperties.m_CollisionLayerID;
}

void MT_Type1::SetEnabledAtIndex(unsigned index, bool enable)
{
	assert(index <= TileSpriteIndexes::TS_COUNT);
	m_Enabled[index] = enable;
}

void MT_Type1::Init()
{
	for (unsigned i(0); i < TileSpriteIndexes::TS_COUNT; ++i)
	{
		m_Animator[i].SetSprite(&m_Sprites[i]);
	}
}

void MT_Type1::Release()
{

}
