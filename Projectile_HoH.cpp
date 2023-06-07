#include "Projectile_HoH.h"

#include "AnimationDefinitions.h"
#include "Game.h"	//Manager Access

void Projectile_HoH::Update(GameTimer& gt)
{	
	//Update animator
	GetBaseAnimator().Update(gt.DeltaTime());

	//Get player position and set equal to + offsets
	DirectX::XMFLOAT2 pos = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();
	GetBaseSprite().SetPosition(pos.x, pos.y + m_OffsetY);

	//Update differently based on state
	switch (m_State)
	{
	case StateID::SPAWNING:

		//Wait for spawn animation to finish before firing projectiles
		if (!GetBaseAnimator().GetPlayState())
		{
			m_State = StateID::ACTIVE;
			GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::ANGEL_IDLE_00, true, true, false, true);
		}
		break;

	case StateID::ACTIVE:

		m_Elapsed += gt.DeltaTime();
		if (m_Elapsed >= m_Interval)
		{
			//Reset and run state checking
			m_Elapsed = 0.f;
			if (--m_RemainingProj <= 0)
			{
				m_State = StateID::DESPAWNING;
			}
			//Find projectile
			Projectile_Interface* proj = Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID::PLAYER_PROJ)->FindFreeObjectByInternalTypeID((unsigned)WeaponTypeID::HEAVEN_ON_HIGH_PROJECTILE);

			if (proj != nullptr)
			{
				//Get stat container for brevity
				auto& projStats = proj->GetStats();

				//Reset Projectile
				proj->Reset();
				proj->ResetHitlist();

				//Pass copy of stats over to projectile
				projStats = GetStats();

				//As there only two enemy pools, randomly bool check again one or the other and only check the other if no return
				bool check = MathHelper::Rand(0, 1);
				EM_EntityInterface2D* ent = Game::GetGame()->GetGameplayManager().GetEntityManager().GetRandomActiveEntityFromPoolScreenCheck((SM_EntityManager::PoolID)check);
				if (ent == nullptr)
				{
					check = !check;
					ent = Game::GetGame()->GetGameplayManager().GetEntityManager().GetRandomActiveEntityFromPoolScreenCheck((SM_EntityManager::PoolID)check);
				}

				//If entity found, setup position at that entity
				if (ent != nullptr)
				{
					proj->InitialisePosition(DirectX::XMFLOAT2(ent->GetCollisionData().m_CollisonBox.posX, ent->GetCollisionData().m_CollisonBox.posY));
				}
				//Edge case where no entities are found, just randomly fire in screen space
				else
				{
					//Get positions for positon setting
					DirectX::XMFLOAT2 playerPos = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();
					float winX = (Game::GetGame()->GetWindowWidthF() * 0.85f) * 0.5f;
					float winY = (Game::GetGame()->GetWindowHeightF() * 0.85f) * 0.5f;

					//Generate random screen position
					DirectX::XMFLOAT2 pos = {
						MathHelper::RandF(playerPos.x - winX, playerPos.x + winX),
						MathHelper::RandF(playerPos.y - winY, playerPos.y + winY)
					};
					proj->InitialisePosition(pos);
				}
				if (m_SoundDelay == HOH_DELAY)
				{
					//Play Smite effect
					Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::SMITE, GameVolumes::SFX);
					m_SoundDelay = 0;
				}
				++m_SoundDelay;
			}
		}
		break;

	case StateID::DESPAWNING:


		//Scale colour alpha down to zero
		DirectX::SimpleMath::Vector4 col = GetBaseSprite().GetColour();
		col.w -= gt.DeltaTime() * 0.5f;

		if (col.w <= 0.f)
		{
			Reset();
			Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(
				SM_EntityManager::PoolID(GetPoolData().m_PoolID))->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		}
		else
			GetBaseSprite().SetColour(col);

		break;
	}
}

void Projectile_HoH::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_HoH::Reset()
{
	GetBaseSprite().SetColour(DirectX::Colors::White);
	m_State = StateID::SPAWNING;
	SetCollisionFlag(false);
	GetBaseAnimator().SetAnimation((int)WeaponsSS01Animations::ANGEL_SPAWN_00, true, false, false, true);
	GetBaseSprite().SetScale(1.5f, 1.5f);

	return false;
}

void Projectile_HoH::SetFunctionalityData(float interval, short projCount, float posYoffset)
{
	m_Interval = interval;
	m_RemainingProj = projCount;
	m_OffsetY = posYoffset;
}

void Projectile_HoH::Init()
{

}

void Projectile_HoH::Release()
{

}
