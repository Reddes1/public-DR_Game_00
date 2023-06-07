#pragma once
#include "D3DUtils.h"		//Types
class CameraMovement
{
public:
	CameraMovement() {}
	~CameraMovement() {}

	void InitCamera(DirectX::XMFLOAT2 playerPosition);
	void UpdateCamera(float DirX, float DirY);

	void MoveCamera(DirectX::XMFLOAT2 move, bool setFlag);

	///////////
	/// Get ///
	///////////
	
	DirectX::XMMATRIX& GetSceneTransform() { return m_SceneTransform; }
	DirectX::XMINT2& GetTileSize() { return m_TileSize; }
	DirectX::XMINT2& GetMapSize() { return m_MapSize; }
	DirectX::XMFLOAT2 GetSceneTranslation() { return m_SceneTranslation; }


	///////////
	/// Set ///
	///////////

	//Set the scene translation,  and set the individual components of the translation
	void SetSceneTranslation(DirectX::XMFLOAT2 translation) { m_SceneTranslation = translation; }
	void SetSceneTranslationX(float x) { m_SceneTranslation.x = x; }
	void SetSceneTranslationY(float y) { m_SceneTranslation.y = y; }

	//Set the scene scale, and the set the individual components of the scale
	void SetSceneScale(DirectX::XMFLOAT2 scale) { m_SceneScale = scale; }
	void SetSceneScaleX(float x) { m_SceneScale.x = x; }
	void SetSceneScaleY(float y) { m_SceneScale.y = y; }

	//Set update flag
	void SetUpdateFlag(bool set) { m_UpdateFlag = set; }
private:

	void Init() {}
	void Release() {}

	//Scene manipulation Matric
	DirectX::XMMATRIX m_SceneTransform = DirectX::XMMatrixIdentity();

	//Translation, scale and rotaiton for use in the matrix
	DirectX::XMFLOAT2 m_SceneTranslation = { 0.f,0.f };
	DirectX::XMFLOAT2 m_SceneScale = { 0.f,0.f };
	float m_SceneRotation = 0.f;

	bool m_UpdateFlag = false;

	DirectX::XMINT2 m_TileSize = { 32,32 };
	DirectX::XMINT2 m_MapSize = { 192,192 };
};

