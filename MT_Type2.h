#pragma once
#include "D3DUtils.h"	//Types & Utils
#include "GameTimer.h"	//Game time

#include "EM_Sprite.h"

class MT_Type2
{
public:
	MT_Type2() { Init(); }
	~MT_Type2() { Release(); }

	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void Update(float dTime);

	EM_Sprite& GetSprite() { return m_Sprite; }
private:
	void Init();
	void Release();

	EM_Sprite m_Sprite;
};

