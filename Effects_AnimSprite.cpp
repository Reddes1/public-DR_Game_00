#include "Effects_AnimSprite.h"
#include "Game.h"	//Manager Access

void Effects_AnimSprite::InitialiseTexAndAnim(std::string& name, int animIndex)
{
	m_BaseSprite.SetTexture(name);
	m_BaseAnimator.SetAnimation(animIndex, false, false, false, true);
}

void Effects_AnimSprite::InitData(OperationStateID state, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, float duration, unsigned short loopCount)
{
	//Set Data
	m_State = state;
	m_Elapsed = 0.f;
	m_ElapsedEnd = duration;
	m_LoopCount = 0;
	m_LoopMax = loopCount;

	m_BaseSprite.SetPosition(pos);
	m_BaseSprite.SetScale(scale);

	//Set animation to play (will begin when instanced)
	m_BaseAnimator.SetPlay(true);
}

void Effects_AnimSprite::Update(GameTimer& gt)
{
	//Update animator
	m_BaseAnimator.Update(gt.DeltaTime());

	//Based on operational state, run different behaviour
	switch (m_State)
	{
	case OperationStateID::ANIM_END:
		//Check to see if animation complete, and flag free in pool if so
		if (!m_BaseAnimator.GetPlayState())
			FlagFreeInPool();
		break;
	}
}

void Effects_AnimSprite::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	m_BaseSprite.Draw(batch, heap);
}

void Effects_AnimSprite::InitPoolData(unsigned short poolID, unsigned short poolIndex)
{
	m_PoolData.m_PoolID = poolID;
	m_PoolData.m_PoolIndex = poolIndex;
}

void Effects_AnimSprite::Init()
{
	//Bind Sprite to Animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
}

void Effects_AnimSprite::Release()
{

}

void Effects_AnimSprite::FlagFreeInPool()
{
	//Free from target pool
	Game::GetGame()->GetGameplayManager().GetEntityManager().GetEffectsPool()->SetObjectStatus(m_PoolData.m_PoolIndex, false);
}
