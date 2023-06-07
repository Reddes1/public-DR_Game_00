#include "Projectile_Cross.h"
#include "Game.h"			//Manager Access

//Brevity Macros
#define GET_PROJ_POOL_00 Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::PLAYER_PROJ)
#define GET_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITY_00)

void Projectile_Cross::Update(GameTimer& gt)
{
	//Attempt to update behaviour
	if (!Flags.m_BehaviourDone)
		UpdateBehaviour(gt);

	//Attempt to off-screen check
	if (Flags.m_BehaviourDone)
		TerminationUpdate();

	//Update Position
	UpdatePosition(gt);

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().x,
		GetGeneralData().m_CollisionRadius * GetBaseSprite().GetScale().y
		}));

	//Update hit list
	UpdateHitList(gt);
}

void Projectile_Cross::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Projectile_Interface::Render(batch, heap);
}

bool Projectile_Cross::Reset()
{
	Flags.m_StageOneDone = false;
	Flags.m_BehaviourDone = false;
	m_Elapsed = 0.0f;
	return true;
}

void Projectile_Cross::Init()
{
	Reset();
	SetCollisionActive(GetBaseSprite().GetScale(), true);
}

void Projectile_Cross::Release()
{
	//Release code here (deletes, clears etc)
}

void Projectile_Cross::UpdateBehaviour(GameTimer& gt)
{
	//Uptick elapsed
	m_Elapsed += gt.DeltaTime();

	if (!Flags.m_StageOneDone)
	{
		//Check to see if the weapon is still decelerating
		if (m_Elapsed <= m_Duration && m_CurrentSpeed >= 0.f)
		{
			//Calculate current proj speed
			m_CurrentSpeed = MathHelper::EaseOut(m_Elapsed, (float)GetStats().m_Speed, (float)-GetStats().m_Speed, m_Duration);
		}
		//Decel phase is done, prep for acceleration
		else
		{
			//Flag done
			Flags.m_StageOneDone = true;
			//Reset timer
			m_Elapsed = 0.f;
			////Normalise current speed to zero (may not quite be zero)
			m_OldSpeed = m_CurrentSpeed;

			GetVelocity().x = -GetVelocity().x;
			GetVelocity().y = -GetVelocity().y;
		}
	}
	//Must be in accel phase
	else
	{
		//Check to see if acceleration done
		if (m_Elapsed <= m_Duration * 0.5f)
		{
			m_CurrentSpeed = MathHelper::EaseIn(m_Elapsed, 0.f, GetStats().m_Speed, m_Duration * 0.5f);
		}
		//At max speed so stop updating for speed
		else
		{
			//Flag updating this segment as done
			Flags.m_BehaviourDone = true;
		}
	}
}
void Projectile_Cross::UpdatePosition(GameTimer& gt)
{
	//Calculate and apply new position based on current position
	DirectX::SimpleMath::Vector2 newPos = {
		(m_CurrentSpeed * GetVelocity().x) * gt.DeltaTime(),
		(m_CurrentSpeed * GetVelocity().y) * gt.DeltaTime()
	};

	//Calculate and add to position
	GetBaseSprite().AddToPosition(newPos);

	//Update Rotation (using fraction of current speed)
	GetBaseSprite().AddToRotation((GetStats().m_Speed * 0.02f) * gt.DeltaTime());		
}

void Projectile_Cross::TerminationUpdate()
{
	//Get Game for brevity
	Game* game = Game::GetGame();
	//Get window dims
	float winX = game->GetWindowWidthF();
	float winY = game->GetWindowHeightF();

	auto pos = GetBaseSprite().GetPosition();

	//Check against screen edges (with adjustments for camera)
	if ((pos.x < 0.f - game->GetGameplayManager().GetCamera().GetSceneTranslation().x)  ||	//Left
		(pos.y < 0.f - game->GetGameplayManager().GetCamera().GetSceneTranslation().y)  ||	//Top
		(pos.x > winX - game->GetGameplayManager().GetCamera().GetSceneTranslation().x) ||	//Right
		(pos.y > winY - game->GetGameplayManager().GetCamera().GetSceneTranslation().y))	//Bottom
	{
		//Disable the projectile by using pool data to flag it free
		GET_PROJ_POOL_00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
		//Reset on the way out
		Reset();
	}
}

