#include "CameraMovement.h"

void CameraMovement::InitCamera(DirectX::XMFLOAT2 playerPosition)
{
	SetSceneTranslation(playerPosition);
}

void CameraMovement::UpdateCamera(float DirX, float DirY)
{
	//Add to the scence translation based on the directions passed in
	m_SceneTranslation.x += DirX;
	m_SceneTranslation.y += DirY;

	//Set the scene transform
	m_SceneTransform = DirectX::XMMatrixTranslation(
		m_SceneTranslation.x, m_SceneTranslation.y, 0.0f
	);
}

void CameraMovement::MoveCamera(DirectX::XMFLOAT2 move, bool setFlag)
{
	SetSceneTranslation(move);
	SetUpdateFlag(setFlag);
}
