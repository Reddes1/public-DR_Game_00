#include "Entity_EnemyType_01.h"

#include "Game.h"		  //Manager Access
#include "GeneralUtils.h" //Utils

void Entity_EnemyType_01::Init()
{

}

void Entity_EnemyType_01::Release()
{
	//Release Code Here
}

void Entity_EnemyType_01::Update(GameTimer& gt)
{
	//If updating enabled (off by default)
	if (GetActiveState())
	{
		//Update explosion timer
		m_ExposionTimer -= gt.DeltaTime();
		//If timer is done, time to explode
		if (m_ExposionTimer <= 0.f)
		{
			//Boom
		}

		//Store old vector to Lerp between it and new vector (to produce slight accel/decel effect)
		DirectX::SimpleMath::Vector2 oldV = m_DirVel;
		//Generate new direction by getting distance between this entity and target (currently mouse: REPLACE WITH PLAYER LATER)
		m_DirVel = {
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().x, Game::GetGame()->GetKBMManager().GetCursorAbsolutePosition().x, false),
			GetDistanceBetweenPoints(m_BaseSprite.GetPosition().y, Game::GetGame()->GetKBMManager().GetCursorAbsolutePosition().y, false)
		};
		m_DirVel.Normalize();
		DirectX::SimpleMath::Vector2 newV = DirectX::SimpleMath::Vector2::Lerp(oldV, m_DirVel, 20);
		newV.Normalize();

		//Add new position to the entity

		if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
			m_BaseSprite.AddToPosition((m_Stats.m_Movespeed * newV.x) * gt.DeltaTime(), (m_Stats.m_Movespeed * newV.y) * gt.DeltaTime());
	}

	//Update animation regardless (to keep global sync)
	if (!Game::GetGame()->GetGameplayManager().GetEntityManager().GetSleep())
		//Update animation
		m_BaseAnimator.Update(gt.DeltaTime());
}

void Entity_EnemyType_01::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//If rendering enabled (off by default)
	if (GetRenderState())
	{
		m_BaseSprite.Draw(batch, heap);
	}
}


