#include "Projectile_Bomb.h"
#include "Game.h"			//Manager Access
#include "AnimationDefinitions.h"

//Brevity Macros
#define GET_PROJ_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ)
#define GET_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)

void Projectile_Bomb::Update(GameTimer& gt)
{
	//Uptick elapsed
	m_Elapsed += gt.DeltaTime();

	switch (m_State)
	{
	case StateID::STABLE:
		//If lifetime done
		if (m_Elapsed >= GetStats().m_Lifetime)
		{
			//Change state to stable, and change animation
			m_State = StateID::EXPLODING;
			GetBaseAnimator().SetAnimation((int)Weapon00Desc::BOMB_EXP_00, true, false, false, true);
		}
		break;

	case StateID::EXPLODING:
		//If exploding wind-up done
		if (!GetBaseAnimator().GetPlayState())
		{
			//Play bomb weapon effect
			Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::BOMB, GameVolumes::SFX);

			//Change state, and change texture and animation to effect
			m_State = StateID::EXPLODED;
			GetBaseAnimator().SetAnimation((int)Weapon00Desc::BOMB_EXP_EFFECT_00, true, false, false, true);
			SetCollisionActive(GetBaseSprite().GetScale(), true);
			GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_2);
		}
		break;

	case StateID::EXPLODED:
		//If explosion effect complete, reset and release
		if (!GetBaseAnimator().GetPlayState())
		{
			Reset();
			GET_PROJ_POOL_00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		break;
	}

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().y
		}));

	//Update hit list
	UpdateHitList(gt);

	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Projectile_Bomb::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Bomb::Reset()
{
	SetCollisionActive(GetBaseSprite().GetScale(), false);
	m_Elapsed = 0.0f;

	m_State = StateID::STABLE;
	GetBaseAnimator().SetAnimation((int)Weapon00Desc::BOMB_IDLE_00, true, true, false, true);
	GetBaseSprite().SetLayerDepth(DRAW_DEPTH_PROJECTILE_1);

	return true;
}

void Projectile_Bomb::Init()
{

}

void Projectile_Bomb::Release()
{
	//Release code here (deletes, clears etc)
}

