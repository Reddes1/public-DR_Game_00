#include "TestingEntity.h"

#include "Game.h"					//Manager Access
#include "AnimationDefinitions.h"	//Animation Info

TestingEntity::TestingEntity()
{
	Game* game = Game::GetGame();

	//Setup Sprite
	m_Sprite.SetTexture(DEFAULT_ANIM_TESTING_TEXTURE_NAME, true);
	m_Sprite.SetPosition(
		game->GetWindowWidthF() * 0.5f,
		game->GetWindowHeightF() * 0.5f
	);
	
	//Setup Animator
	m_Animator.SetSpritePointer(&m_Sprite);
	m_Animator.SetAnimation(0);
	m_Sprite.SetOriginToPoint(JustificationPosition::CENTER);
	m_Sprite.SetOriginToCenter();

	//Setup Controller
	m_Controller.ResetKeysToDefault();

	//Setup Mover
	m_FixedMove.SetMovementDistanceX(256.f);
	m_FixedMove.SetMovementDistanceY(256.f);
}

void TestingEntity::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Draw Main Sprite
	m_Sprite.Draw(batch, heap);
}

void TestingEntity::Update(GameTimer& gt)
{
	Game* game = Game::GetGame();
	
	//Update Flags
	m_Controller.UpdateInputs();

	//Position Change for frame
	DirectX::XMFLOAT2 newPos = { 0, 0 };

	////Check for flags and perform actions
	//if (m_Controller.GetPressedStateAtIndex(0))
	//	m_FixedMove.Move4(Directional4Way::LEFT, newPos);
	//else if (m_Controller.GetPressedStateAtIndex(1))
	//	m_FixedMove.Move4(Directional4Way::RIGHT, newPos);
	//if (m_Controller.GetPressedStateAtIndex(2))
	//	m_FixedMove.Move4(Directional4Way::UP, newPos);

	//"Edge check here"
	
	//Adjust for delta time
	newPos.x *= gt.DeltaTime();
	newPos.y *= gt.DeltaTime();
	//Set Position
	m_Sprite.AddToPosition(newPos);

	//Update Animator
	m_Animator.Update(gt.DeltaTime());
	//Clear action flags
	m_Controller.ClearActionsFlags();
}

void TestingEntity::MoveEntity(Directional4Way dir)
{

}
