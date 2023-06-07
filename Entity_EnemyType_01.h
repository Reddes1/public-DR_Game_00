#pragma once

#include "EM_EntityInterface.h"	//Parent
#include "GameTypes.h"			//Enemy Data

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"

/*
	Enemy Type 01 - Big Badaboom
*/
class Entity_EnemyType_01 : public EM_EntityInterface2D
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	//Describes the different modes of enemy should be configured (texture, settings etc)
	enum class SubType
	{
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Entity_EnemyType_01() { Init(); }
	~Entity_EnemyType_01() { Release(); }


	/////////////////
	/// Overrides ///
	/////////////////

	void Update(GameTimer& gt) override;
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	void InitSpecialSpawn(DirectX::XMFLOAT2 pos, const EnemyCompositeData& data) override {}

	//////////////////
	/// Operations ///
	//////////////////



	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }

	GeneralEntityData& GetGeneralData() { return m_Data; }
	EnemyStats& GetStats() { return m_Stats; }


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


	////////////
	/// Data ///
	////////////

	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;

	//Unit Data Containers
	GeneralEntityData m_Data;
	EnemyStats m_Stats;

	//Directional Vector for movement (wrap this into a module later)
	DirectX::SimpleMath::Vector2 m_DirVel = { 0, 0 };

	//Data describing the explosion details
	float m_ExposionTimer = 3.f;
	float m_ExplosionRadius = 30.f;

	// Knockback values
	bool m_KnockbackActive = false;
	float m_KnockbackSpeed = 100.f;
	DirectX::SimpleMath::Vector2 m_Knockback = { 0.f, 0.f };
	DirectX::SimpleMath::Vector2 m_KnockbackLeft = { 0.f, 0.f };
};