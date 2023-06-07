#include "Projectile_Interface.h"

#include "GeneralUtils.h" //Utils

void Projectile_Interface::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Default render
	m_BaseSprite.Draw(batch, heap);
}

void Projectile_Interface::InitialiseVisuals(std::string& texName, unsigned frame)
{
	m_BaseSprite.SetTexture(texName);
	m_BaseSprite.SetFrame(frame);
	m_BaseSprite.SetOriginToCenter();
}

void Projectile_Interface::InitialiseVisuals(std::string& texName, unsigned animID, bool play, bool loop, bool reverse)
{
	m_BaseSprite.SetTexture(texName);	
	m_BaseAnimator.SetAnimation(animID, play, loop, reverse);
}

void Projectile_Interface::InitialisePosition(DirectX::XMFLOAT2& pos)
{
	m_BaseSprite.SetPosition(pos);
}

void Projectile_Interface::InitialisePositionAndVelocity(DirectX::XMFLOAT2& pos, DirectX::XMFLOAT2& vel)
{
	//Set start position
	m_BaseSprite.SetPosition(pos);
	m_DirVel = vel;
}

void Projectile_Interface::InitPoolData(unsigned short poolID, unsigned short poolIndex)
{
	m_PoolData.m_PoolID = poolID;
	m_PoolData.m_PoolIndex = poolIndex;
}

bool Projectile_Interface::IsEntityInHitList(EM_EntityInterface2D* const ent)
{
	//Search for entity in current list
	auto it = std::find(m_HitList.begin(), m_HitList.end(), ent);

	//If container end hit, then process entity
	if (it == m_HitList.end())
	{
		//Before trying to insert, check if at list limit
		if (m_HitList.size() >= MAX_HITLIST_SIZE)
		{
			//Clear the list and clear timer
			m_HitList.clear();
			m_HLElapsed = 0.f;
		}
		//Ensure timer is active, and store ent in list
		m_HLTimerActive = true;
		m_HitList.push_back(ent);
		return false;
	}
	//Entity was in the list, so return true
	return true;
}

void Projectile_Interface::UpdateHitList(GameTimer& gt)
{
	//If timer is active
	if (m_HLTimerActive)
	{
		//Uptick elapsed
		m_HLElapsed += gt.DeltaTime();
		//If elapsed is now past point, disable timer and reset list/elapsed (re-enabled when new hit detected)
		if (m_HLElapsed >= m_Stats.m_ReHitDelay)
		{
			ResetHitlist();
		}
	}
}

void Projectile_Interface::ResetHitlist()
{
	m_HLTimerActive = false;
	m_HLElapsed = 0.f;
	m_HitList.clear();
}

void Projectile_Interface::Init()
{
	//Bind sprite to animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
}

void Projectile_Interface::Release()
{
}
