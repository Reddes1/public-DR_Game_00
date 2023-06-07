#include "MT_Type2.h"

void MT_Type2::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Draw the tile with its given sprite/texture
	m_Sprite.Draw(sb, heap);
}

void MT_Type2::Update(float dTime)
{
	//This is where animaiton update will go if we add animated
	//tiles for the new setup
}

void MT_Type2::Init()
{
	//Animator init, if/when it gets added to these tiles
}

void MT_Type2::Release()
{
}
