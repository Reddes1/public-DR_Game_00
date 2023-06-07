#pragma once

#include "EM_EntityInterface.h"	//Parent
#include "GameTypes.h"			//Enemy Data
#include "SM_EntityManager.h"

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"

/*
	Enemy Type 00 - The Chase Is On!
*/
class Entity_EnemyType_00 : public EM_EntityInterface2D
{
public:

	////////////
	/// Data ///
	////////////

	//Contact damage CD
	static constexpr float COLLISION_CD = 1.f;

	///////////////////
	/// Definitions ///
	///////////////////

	//Defines the different starting conditions for the entity
	enum class InitPosition
	{
		RANDOMISED,
		RANDOM_LEFT,
		RANDOM_RIGHT,
		RANDOM_TOP,
		RANDOM_DOWN,
		RANDOM_LEFT_TOP,
		RANDOM_RIGHT_TOP,
		RANDOM_LEFT_BOTTOM,
		RANDOM_RIGHT_BOTTOM
	};

	//Describes what the local animation indexes point to.
	//Note: Loaded indexes from file must conform to this ordering (else incorrect animations will play)
	enum AnimIndexes : unsigned
	{
		ATTACK_L,
		ATTACK_R,
		IDLE_L,
		IDLE_R,
		MOVE_L,
		MOVE_R,
		MISC_00,
		MISC_01,
		MISC_02,
		MISC_03,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Entity_EnemyType_00() { Init(); }
	~Entity_EnemyType_00() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//
	//Operations
	//

	void Update(GameTimer& gt) override;
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//
	//Spawning
	//

	bool SpawnEntity(unsigned initPos, const EnemyCompositeData& data) override;
	void InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data) override;

	//
	//Combat
	//

	bool ProcessDamageEvent(short damage, short knockback) override;
	bool ProcessCollisionEvent() override { return false; }
	bool CanCollisionDamageOccur() override { return m_CollisionTimer < 0.f; }
	void ResetCollisionTimer() override { m_CollisionTimer = COLLISION_CD; }
	short GetDamageValue() override { return (short)(GetStats().m_Damage * GetStats().m_DamageMod); }

	//
	//Utilities
	//

	void ResetEntityToDefaults() override;

	//////////////////
	/// Operations ///
	//////////////////



	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }

	GeneralEntityData& GetGeneralData() { return EM_EntityInterface2D::GetGeneralData(); }
	EnemyStats& GetStats() { return EM_EntityInterface2D::GetEnemyStats(); }

	///////////
	/// Set ///
	///////////

private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//Spawning support functions
	void InitStartingPosition(InitPosition init);
	void InitData(const EnemyCompositeData& data);

	////////////
	/// Data ///
	////////////

	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;

	//KB data package for resolving KB effects from taking damage
	KBData m_KBData;

	//Directional vector for movement (wrap this into a module later)
	DirectX::SimpleMath::Vector2 m_Velocity = { 0.f, 0.f };

	//For tracking players position
	Entity_Player* m_Player = nullptr;

	//Instead of a complex string of switch statements, store the indexes of animations
	//related to this player when initialised as a character.
	//See AnimType for what the indexes should relate too
	int m_AnimIndexes[AnimIndexes::COUNT];

	//Tracking timer for if the entity can damage the player on contact (true if below 0.f)
	float m_CollisionTimer = COLLISION_CD;
};