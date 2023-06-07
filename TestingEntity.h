#pragma once

#include "EM_EntityInterface.h" //Parent

//Managers
#include "InputManager.h"

//Behavior Modules
#include "EM_Sprite.h"
#include "EM_Animator.h"
#include "EM_FixedMovement.h"

//Control Scheme Test
#include "CS_Platformer_00.h"

class TestingEntity : public EM_EntityInterface2D
{
public:

	///////////////////
	/// Constructor ///
	///////////////////

	TestingEntity();
	~TestingEntity() {}

	/////////////////
	/// Overrides ///
	/////////////////

	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;
	void Update(GameTimer& gt) override;
	void ProcessKeyUpdate(char key)				{ m_Controller.UpdateProcessKey(key); }

	//////////////////
	/// Operations ///
	//////////////////

	void MoveEntity(Directional4Way dir);

	///////////
	/// Set ///
	///////////

	///////////
	/// Get ///
	///////////

	EM_Sprite& GetSpriteModule()				{ return m_Sprite; }
	EM_Animator& GetAnimatorModule()			{ return m_Animator; }
	EM_FixedMovement& GetFixedMoverModule()		{ return m_FixedMove; }
	CS_Platformer_00& GetController()			{ return m_Controller; }

private:

	EM_Sprite m_Sprite;
	EM_Animator m_Animator;
	EM_FixedMovement m_FixedMove;

	CS_Platformer_00 m_Controller;
};