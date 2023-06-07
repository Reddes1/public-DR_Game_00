#include "Loot_Interface.h"

#include "GeneralUtils.h" //Utils

void Loot_Interface::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Default render
	m_BaseSprite.Draw(batch, heap);
}

void Loot_Interface::InitialiseVisuals(std::string& texName, unsigned frame)
{
	m_BaseSprite.SetTexture(texName);
	m_BaseSprite.SetFrame(frame);
	m_BaseSprite.SetOriginToCenter();
}

void Loot_Interface::InitialisePosition(DirectX::XMFLOAT2& pos)
{
	m_BaseSprite.SetPosition(pos);
	
}

void Loot_Interface::InitPoolData(unsigned short poolID, unsigned short poolIndex)
{
	m_PoolData.m_PoolID = poolID;
	m_PoolData.m_PoolIndex = poolIndex;
}
void Loot_Interface::Init()
{
	//Bind sprite to animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
}

void Loot_Interface::Release()
{
}

void Loot_Interface::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	
}