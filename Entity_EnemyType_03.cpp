#include "Entity_EnemyType_03.h"

#include "Game.h"		  //Manager Access
#include "GeneralUtils.h" //Utils

#define GET_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetEntityPoolAtID(SM_EntityManager::PoolID::ENTITIES_01_02_03)
#define GET_EXP_POOL Game::GetGame()->GetGameplayManager().GetEntityManager().GetLootPool()

void Entity_EnemyType_03::Init()
{
	//Bind Animator/Sprite
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
}

void Entity_EnemyType_03::Release()
{
	//Release Code Here
}

void Entity_EnemyType_03::Update(GameTimer& gt)
{
	//
	//Movement/Collision
	//

	//Move the entity normally

	if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
		m_BaseSprite.AddToPosition((GetStats().m_Movespeed * m_Velocity.x) * gt.DeltaTime(), (GetStats().m_Movespeed * m_Velocity.y) * gt.DeltaTime());

	//If collision was detected, apply a reduced nudging effect to the entity to shift the entity off-course
	if (GetNudge())
	{
		SetNudge(false);

		DirectX::SimpleMath::Vector2& newV = DirectX::SimpleMath::Vector2(GetNudgeDir());
		newV.Normalize();
		m_BaseSprite.AddToPosition(
			((GetStats().m_Movespeed * 0.75f) * newV.x) * gt.DeltaTime(),
			((GetStats().m_Movespeed * 0.75f) * newV.y) * gt.DeltaTime()
		);
	}

	//
	//Termination
	//

	//If the spawn buffer is done, edge of screen checks
	if (m_SpawnBufferTimer < 0.f)
	{
		//Get data for brevity
		CameraMovement& cam = Game::GetGame()->GetGameplayManager().GetCamera();
		auto winDims = Game::GetGame()->GetWindowSizeF();
		auto& pos = GetBaseSprite().GetPosition();

		//Check against screen edges + offset
		if ((pos.x < (-winDims.x * 0.1f) - cam.GetSceneTranslation().x)	||
			 pos.y < (-winDims.y * 0.1f) - cam.GetSceneTranslation().y	||
			 pos.x > (winDims.x * 1.1f) - cam.GetSceneTranslation().x   ||
			 pos.y > (winDims.y * 1.1f) - cam.GetSceneTranslation().y)
		{
			//Disable this entity via pool flag and reset on the way out
			GET_POOL->SetObjectStatus(GetPoolData().m_PoolIndex, false);
			ResetEntityToDefaults();
		}
	}
	else
		m_SpawnBufferTimer -= gt.DeltaTime();

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
		//Update animation
		m_BaseAnimator.Update(gt.DeltaTime());
	//Update collision timer
	m_CollisionTimer -= gt.DeltaTime();
}

void Entity_EnemyType_03::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	m_BaseSprite.Draw(batch, heap);
}

bool Entity_EnemyType_03::SpawnEntity(unsigned initPos, const EnemyCompositeData& data)
{
	InitStartingPosition((InitPosition)initPos);
	InitData(data);
	return true;
}
void Entity_EnemyType_03::InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data)
{
	m_BaseSprite.SetPosition(pos);
	InitData(data);
}

bool Entity_EnemyType_03::ProcessDamageEvent(short damage, short knockback)
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

void Entity_EnemyType_03::ResetEntityToDefaults()
{
	//Reset offscreen timer
	m_SpawnBufferTimer = BUFFER_TIME;
}

void Entity_EnemyType_03::InitStartingPosition(InitPosition init)
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

	//Based on init, set position and velocity
	switch (init)
	{
	case InitPosition::RANDOMISED_CORNERS:
		//Recall this function with a range of indexes appropriate to initial call
		InitStartingPosition((InitPosition)MathHelper::Rand((int)InitPosition::LEFT_TOP, (int)InitPosition::RIGHT_BOTTOM));
		break;
	case InitPosition::RANDOMISED_SIDES:
		//Recall this function with a range of indexes appropriate to initial call
		InitStartingPosition((InitPosition)MathHelper::Rand((int)InitPosition::TOP_TO_BOTTOM, (int)InitPosition::RIGHT_TO_LEFT));
		break;

	case InitPosition::RANDOMISED_ALL:
		//Recall this function with total range of options (aside random) available
		InitStartingPosition((InitPosition)MathHelper::Rand((int)InitPosition::LEFT_TOP, (int)InitPosition::RIGHT_TO_LEFT));
		break;

	case InitPosition::LEFT_TOP:
		//Left-Top to Right-Bottom
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_Velocity = { 1.f, 0.6f };	
		break;

	case InitPosition::RIGHT_TOP:
		//Right-Top to Left-Bottom
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_Velocity = { -1.f, 0.6f };
		break;
	
	case InitPosition::LEFT_BOTTOM:
		//Left-Bottom to Right-Top
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_Velocity = { 1.f, -0.6f };
		break;

	case InitPosition::RIGHT_BOTTOM:
		//Right-Bottom to Left-Top
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_Velocity = { -1.f, -0.6f };
		break;

	case InitPosition::TOP_TO_BOTTOM:
		//Position randomly along to the top of the area and generate velocity to opposite of this position
		m_BaseSprite.SetPositionX(sceneAdjL + MathHelper::RandF(0.f, winDim.x));
		m_BaseSprite.SetPositionY(sceneAdjT - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		GenerateVelocity(sceneAdjR, sceneAdjL, sceneAdjB, sceneAdjT);
		break;

	case InitPosition::BOTTOM_TO_TOP:
		//Position randomly along to the bottom of the area and generate velocity to opposite of this position
		m_BaseSprite.SetPositionX(sceneAdjL + MathHelper::RandF(0.f, winDim.x));
		m_BaseSprite.SetPositionY(sceneAdjB + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		GenerateVelocity(sceneAdjR, sceneAdjL, sceneAdjB, sceneAdjT);
		break;

	case InitPosition::LEFT_TO_RIGHT:
		//Position randomly along to the left of the area and generate velocity to opposite of this position
		m_BaseSprite.SetPositionX(sceneAdjL - MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT + MathHelper::RandF(0.f, winDim.y));
		//Generate velocity to opposite point towards the bottom
		GenerateVelocity(sceneAdjR, sceneAdjL, sceneAdjB, sceneAdjT);
		break;

	case InitPosition::RIGHT_TO_LEFT:
		//Position randomly along to the left of the area and generate velocity to opposite of this position
		m_BaseSprite.SetPositionX(sceneAdjR + MathHelper::RandF(ENTITY_SCREEN_OFFSET_MIN, ENTITY_SCREEN_OFFSET_MAX));
		m_BaseSprite.SetPositionY(sceneAdjT + MathHelper::RandF(0.f, winDim.y));
		GenerateVelocity(sceneAdjR, sceneAdjL, sceneAdjB, sceneAdjT);
		break;

	default:
		msg_assert(false, "InitStartingPosition(): Invalid Spawner ID passed!");
	}
	//Normalise velocity
	m_Velocity.Normalize();
}

void Entity_EnemyType_03::InitData(const EnemyCompositeData& data)
{
	//Copy data into entity
	GetStats() = data.m_Stats;
	GetGeneralData() = data.m_GeneralData;
	GetFlags() = data.m_Flags;
	m_KBData = data.m_KBData;

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

	//Set animation based on velocity
	if (m_Velocity.x > 0.f)
		m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
	else
		m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);

	//Set scale using general data
	m_BaseSprite.SetScale(GetGeneralData().m_Scale.x, GetGeneralData().m_Scale.y);
}

void Entity_EnemyType_03::GenerateVelocity(float& adjR, float& adjL, float& adjB, float& adjT)
{
	m_Velocity =
	{
		GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, (adjR - m_BaseSprite.GetPosition().x) + adjL, false),
		GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, (adjB - m_BaseSprite.GetPosition().y) + adjT, false)
	};
}
