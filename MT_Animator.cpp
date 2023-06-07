#include "MT_Animator.h"

void MT_Animator::UpdateAnimation(float dTime)
{
	//Only play if play flagged and there is more than one frame docked
	if (!m_Play || m_Animations.size() <= 1)
	{
		return;
	}

	//Uptick
	m_Elapsed += dTime;

	//Has enough time passed?
	assert(m_CurrentFrameIndex <= m_Animations.size());
	if (m_Elapsed > (1.f / m_Animations[m_CurrentFrameIndex].m_Speed))
	{
		//Reset the clock
		m_Elapsed = 0.f;
		//Increment count
		++m_CurrentFrameIndex;
		//Check if frame is out of range
		if (m_CurrentFrameIndex >= m_Animations.size())
		{
			m_CurrentFrameIndex = 0;
		}
		assert(m_CurrentFrameIndex <= m_Animations.size());
		m_Sprite->SetRect(m_Animations[m_CurrentFrameIndex].m_FrameRect);
	}
}
