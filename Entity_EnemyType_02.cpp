#include "Entity_EnemyType_02.h"
#include "CollisionTypes.h"
#include "SM_EntityManager.h"

#include "Game.h"		  //Manager Access
#include "GeneralUtils.h" //Utils

//Brevity Macros
#define GET_PROJ_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetProjectilePoolAtID(SM_EntityManager::PoolID::ENEMY_PROJ)
#define GET_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITIES_01_02_03)
#define GET_EXP_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()


void Entity_EnemyType_02::Init()
{
	//Bind base sprite with animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
	m_BaseAnimator.SetPlay(false);

	//Set player ptr for later access
	m_Player = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer();
}

void Entity_EnemyType_02::Release()
{
	//Release Code Here
}

void Entity_EnemyType_02::InitStartingPosition(InitPosition init)
{
	//Get game for brevity
	Game* game = Game::GetGame();
	//Use players relative position with window dims to offset the entity accordingly
	auto playerPos = game->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition();
	auto winDim = game->GetWindowSizeF();

	//Pre-generate scene adjustments that are used in several different cases for clarity
	float sceneAdjL = (playerPos.x - winDim.x * 0.5f);
	float sceneAdjT = (playerPos.y - winDim.y * 0.5f);
	float sceneAdjR = (playerPos.x + winDim.x * 0.5f);
	float sceneAdjB = (playerPos.y + winDim.y * 0.5f);

	//Based on init, set position
	switch (init)
	{
	case InitPosition::RANDOMISED:
		//Recall this function with a randomised index that isn't equal to random
		InitStartingPosition((InitPosition)MathHelper::Rand((int)InitPosition::RANDOM_LEFT, (int)InitPosition::RANDOM_RIGHT_BOTTOM));
		break;

	case InitPosition::RANDOM_LEFT:
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT + MathHelper::RandF(0.f, winDim.y));
		break;
	case InitPosition::RANDOM_RIGHT:
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT + MathHelper::RandF(0.f, winDim.y));
		break;
	case InitPosition::RANDOM_TOP:
		m_BaseSprite.SetPositionX(sceneAdjL + MathHelper::RandF(0.f, winDim.x));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;
	case InitPosition::RANDOM_DOWN:
		m_BaseSprite.SetPositionX(sceneAdjL + MathHelper::RandF(0.f, winDim.x));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;

	case InitPosition::RANDOM_LEFT_TOP:
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;

	case InitPosition::RANDOM_RIGHT_TOP:
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;

	case InitPosition::RANDOM_LEFT_BOTTOM:
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;

	case InitPosition::RANDOM_RIGHT_BOTTOM:
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		break;

	default:
		msg_assert(false, "InitStartingPosition(): Invalid Spawner ID passed!");

	}
}

void Entity_EnemyType_02::InitData(const EnemyCompositeData& data)
{
	//Copy data into entity
	GetStats() = data.m_Stats;
	GetGeneralData() = data.m_GeneralData;
	GetFlags() = data.m_Flags;
	m_KBData = data.m_KBData;
	ResetCollisionTimer();

	//
	//Texture + Animation
	//

	//Set texture by name
	m_BaseSprite.SetTexture(data.m_TextureData.m_TextureName);
	//If animated, store indexes of animations related to this entity
	if (data.m_TextureData.m_Animated)
	{
		//Load animation indexes into array
		for (unsigned i(0); i < data.m_TextureData.m_Frames.size(); ++i)
			m_AnimIndexes[i] = data.m_TextureData.m_Frames[i];

		m_BaseAnimator.SetAnimation(data.m_TextureData.m_Frames[0], true, true, false);
	}
	//Just set the base frame given
	else
		m_BaseSprite.SetFrame(data.m_TextureData.m_FrameIndex);

	//Set scale using general data
	m_BaseSprite.SetScale(GetGeneralData().m_Scale.x, GetGeneralData().m_Scale.y);
}

void Entity_EnemyType_02::Update(GameTimer& gt)
{
	//
	//Movement
	//

	//Check how to move the entity, based on distance from player
	float dist = DirectX::SimpleMath::Vector2::Distance(m_BaseSprite.GetPosition(), m_Player->GetRelativePosition());

	//Inner boundary check
	if (dist < MIN_DISTANCE)
	{
		//Generate velocity away from player
		m_Velocity =
		{
			-GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, m_Player->GetRelativePosition().x, false),
			-GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, m_Player->GetRelativePosition().y, false)
		};

		//Is the player to the right
		if (m_Player->GetRelativePosition().x >= m_BaseSprite.GetPosition().x)
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);
		else
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
	}
	//Outer boundary check
	else if (dist > MAX_DISTANCE)
	{
		//Generate regular velocity towards player
		m_Velocity =
		{
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, m_Player->GetRelativePosition().x, false),
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, m_Player->GetRelativePosition().y, false)
		};

		//Is the player to the right
		if (m_Player->GetRelativePosition().x >= m_BaseSprite.GetPosition().x)
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
		else
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);
	}

	//Normalise and move
	m_Velocity.Normalize();

	if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
		m_BaseSprite.AddToPosition((GetStats().m_Movespeed * m_Velocity.x) * gt.DeltaTime(), (GetStats().m_Movespeed * m_Velocity.y) * gt.DeltaTime());

	//If collision was detected, apply a reduced nudging effect to the entity to shift the entity off-course
	if (GetNudge())
	{
		SetNudge(false);
		DirectX::SimpleMath::Vector2 newV = GetNudgeDir();
		newV.Normalize();
		m_BaseSprite.AddToPosition(
			((GetStats().m_Movespeed * 0.75f) * newV.x) * gt.DeltaTime(),
			((GetStats().m_Movespeed * 0.75f) * newV.y) * gt.DeltaTime()
		);
	}

	//
	//Attacking
	//

	//if timer complete, start firing process
	if (m_FiringTimer >= GetStats().m_AttackInterval)
	{
		m_FiringTimer = 0.f;

		//Find a free projectile
		Projectile_Interface* proj = GET_PROJ_POOL->FindFreeObjectByInternalTypeID(0);
		if (proj != nullptr)
		{
			//Get stat container for brevity
			auto& projStats = proj->GetStats();
			projStats.m_Damage = GetStats().m_Damage;
			projStats.m_Speed = 300;
			projStats.m_ReHitDelay = 1.f;
			projStats.m_CollisionsRemaining = 1;

			//Reset hitlist data
			proj->ResetHitlist();

			//
			//Position & Direction Behaviour
			//

			//Generate direction towards player
			DirectX::SimpleMath::Vector2 dir = 
			{
				GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, m_Player->GetRelativePosition().x, false),
				GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, m_Player->GetRelativePosition().y, false)
			};
			dir.Normalize();

			//Set a rotation using velocity
			proj->GetBaseSprite().SetRotation(MathHelper::AngleFromXY(dir.x, dir.y));

			//Set position and velocity
			proj->InitialisePositionAndVelocity(DirectX::XMFLOAT2(m_BaseSprite.GetPosition()), dir);
		}
		//Unable to find projectile (Should never really happen for now, so just alert this)
		else
		{
			msg_assert(false, "Update(): Unable to find free projectile.");
		}
	}

	//
	//Post
	//

	//Update collision for the entity
	SetCollisionData(CollisionArea({
		m_BaseSprite.GetPosition().x, m_BaseSprite.GetPosition().y,
		GetGeneralData().m_CollisionRadius * m_BaseSprite.GetScale().x,
		GetGeneralData().m_CollisionRadius * m_BaseSprite.GetScale().y
		}));

	if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
	{
		//Update firing timer
		m_FiringTimer += gt.DeltaTime();
		//Update animation
		m_BaseAnimator.Update(gt.DeltaTime());
	}
	//Update collision timer
	m_CollisionTimer -= gt.DeltaTime();
	
}

void Entity_EnemyType_02::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{

	m_BaseSprite.Draw(batch, heap);

}

bool Entity_EnemyType_02::SpawnEntity(unsigned initPos, const EnemyCompositeData& data)
{
	InitStartingPosition((InitPosition)initPos);
	InitData(data);

	return true;
}
void Entity_EnemyType_02::InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data)
{
	m_BaseSprite.SetPosition(pos);
	InitData(data);
}

bool Entity_EnemyType_02::ProcessDamageEvent(short damage, short knockback)
{
	//Apply damage to this entity
	GetStats().m_Health -= damage;
	//If health below 0, "kill" this unit
	if (GetStats().m_Health <= 0)
	{
		//Pass to loot manager to handle drops
		Game::GetGame()->GetGameplayManager().GetLootManager().ProcessNewLootEvent(this);

		//Death event, return true
		return true;
	}

	//No death event, return false
	return false;
}
