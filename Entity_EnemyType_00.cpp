#include "Entity_EnemyType_00.h"
#include "CollisionTypes.h"
#include "SM_EntityManager.h"

#include "Game.h"		  //Manager Access
#include "GeneralUtils.h" //Utils
#include "AnimationDefinitions.h"

void Entity_EnemyType_00::Init()
{
	//Bind base sprite with animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
	m_BaseAnimator.SetPlay(false);

	//Set player ptr for later access
	m_Player = Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer();
}

void Entity_EnemyType_00::Release()
{
	//Release Code Here
}

bool Entity_EnemyType_00::SpawnEntity(unsigned initPos, const EnemyCompositeData& data)
{
	InitStartingPosition((InitPosition)initPos);
	InitData(data);

	return true;
}

void Entity_EnemyType_00::InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data)
{
	m_BaseSprite.SetPosition(pos);
	InitData(data);
}

void Entity_EnemyType_00::InitStartingPosition(InitPosition init)
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

void Entity_EnemyType_00::InitData(const EnemyCompositeData& data)
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

void Entity_EnemyType_00::Update(GameTimer& gt)
{
	//
	//Movement/Collision
	//

	//Uptick KB timer
	m_KBData.m_Elapsed += gt.DeltaTime();

	//Check to see if immunity window over
	if (m_KBData.m_Elapsed >= m_KBData.m_ImmunityWindow)
		m_KBData.m_Immune = false;

	//If KB flag active
	if (m_KBData.m_Active)
	{
		//If below the duration window
		if (m_KBData.m_Elapsed <= m_KBData.m_Duration)
		{
			//Apply new position
			m_BaseSprite.AddToPosition(
				(m_KBData.m_Speed * m_Velocity.x) * gt.DeltaTime(),
				(m_KBData.m_Speed * m_Velocity.y) * gt.DeltaTime()
			);
		}
		//Else, flag active as over
		else
			m_KBData.m_Active = false;
	}
	else {
		//Generate new direction by getting distance between this entity and player target
		m_Velocity =
		{
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, m_Player->GetRelativePosition().x, false),
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, m_Player->GetRelativePosition().y, false)
		};
		m_Velocity.Normalize();
		//Attempt to apply movement normally
		if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
			m_BaseSprite.AddToPosition((GetStats().m_Movespeed * m_Velocity.x) * gt.DeltaTime(), (GetStats().m_Movespeed * m_Velocity.y) * gt.DeltaTime());

		
		//If collision was detected, apply a reduced nudging effect to the entity to shift the entity off-course
		if (GetNudge())
		{
			m_Velocity = GetNudgeDir();
			m_Velocity.Normalize();
			m_BaseSprite.AddToPosition(
				((GetStats().m_Movespeed * 0.75f) * m_Velocity.x) * gt.DeltaTime(),
				((GetStats().m_Movespeed * 0.75f) * m_Velocity.y) * gt.DeltaTime()
			);

			SetNudge(false);
		}
	}

	//
	//Post
	//

	//Change animation/direction based on player position
	if (m_Player->GetRelativePosition().x >= m_BaseSprite.GetPosition().x)
		m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
	else
		m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);

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

void Entity_EnemyType_00::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	m_BaseSprite.Draw(batch, heap);
}

void Entity_EnemyType_00::ResetEntityToDefaults()
{

}

bool Entity_EnemyType_00::ProcessDamageEvent(short damage, short knockback)
{
	//Apply damage to this entity
	GetStats().m_Health -= damage;
	//If health below 0, kill this unit
	if (GetStats().m_Health <= 0)
	{
		//Pass to loot manager to handle drops
		Game::GetGame()->GetGameplayManager().GetLootManager().ProcessNewLootEvent(this);

		//Place effect at death location
		Effects_AnimSprite* effect = Game::GetGame()->GetGameplayManager().GetEntityManager().GetEffectsPool()->FindFreeObject();
		if (effect != nullptr)
		{
			effect->InitialiseTexAndAnim(std::string("Effects_SS_00"), (int)EffectsSS00Animations::BLOODSPLATTER_01);
			effect->InitData(
				Effects_AnimSprite::OperationStateID::ANIM_END,
				m_BaseSprite.GetPosition(),
				DirectX::XMFLOAT2(2.f, 2.f)
			);
		}
		//Death event, return true
		return true;
	}

	//Check if kb valid, and immunity not active via mod/flag
	if (knockback > 0 && GetStats().m_KnockbackEffectMod > 0.0f && !m_KBData.m_Immune)
	{
		//Apply modified KB speed
		m_KBData.m_Speed = static_cast<float>(knockback) * GetStats().m_KnockbackEffectMod;

		//Invert current velocity (opposite direction of player)
		m_Velocity.x = -m_Velocity.x;
		m_Velocity.y = -m_Velocity.y;

		//Flags and reset time
		m_KBData.m_Active = true;
		m_KBData.m_Immune = true;
		m_KBData.m_Elapsed = 0.f;
	}

	//No death event, return false
	return false;
}
