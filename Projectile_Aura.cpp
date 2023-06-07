#include "Projectile_Aura.h"
#include "Game.h"			//Manager Access
#include "CollisionTypes.h"

void Projectile_Aura::Update(GameTimer& gt)
{
	//Update hit list
	UpdateHitList(gt);
	//Sync position with player
	GetBaseSprite().SetPosition(Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition());

	switch (m_State)
	{
	case StateID::ACTIVE:
		
		//Update animation
		GetBaseAnimator().Update(gt.DeltaTime());

		//If animation done, change state
		if (!GetBaseAnimator().GetPlayState())
		{
			GetBaseAnimator().RestartAnimation(false);
			m_State = StateID::INACTIVE;
			m_Elapsed = 0.f;
			SetCollisionFlag(false);
		}
		//Else, update the collision data
		else
		{
			//Update collision for the entity
			SetCollisionData(CollisionArea({
				GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().x,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().y
				}));
		}

		break;

	case StateID::INACTIVE:

		//Uptick elapsed and check against CD, re-enabling the weapon as required
		m_Elapsed += gt.DeltaTime();
		if (m_Elapsed >= GetStats().m_Lifetime)
		{
			m_State = StateID::ACTIVE;
			GetBaseAnimator().RestartAnimation(true);
			SetCollisionFlag(true);

			//Update collision for the entity
			SetCollisionData(CollisionArea({
				GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().x,
				GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().y
				}));

			//Play Aura weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::AURA, GameVolumes::SFX);
		}

		break;
	}
}

void Projectile_Aura::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Aura::Reset()
{
	m_State = StateID::ACTIVE;
	GetBaseAnimator().SetPlay(true);
	GetBaseAnimator().SetLoop(false);
	SetCollisionFlag(true);

	return true;
}

void Projectile_Aura::Init()
{
	Reset();
}

void Projectile_Aura::Release()
{
	//Release code here (deletes, clears etc)
}

