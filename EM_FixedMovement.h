#pragma once
#include "D3DUtils.h"

class EM_FixedMovement
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	EM_FixedMovement() {}
	~EM_FixedMovement() {}


	//////////////////
	/// Operations ///
	//////////////////

	void Move8(Directional8Way dirID, DirectX::XMFLOAT2& container);
	DirectX::XMFLOAT2 Move8(Directional8Way dirID);

	void Move4(Directional4Way dirID, DirectX::XMFLOAT2& container);
	DirectX::XMFLOAT2 Move4(Directional4Way dirID);

	///////////
	/// Set ///
	///////////

	void SetMovementDistance(DirectX::XMFLOAT2& newDist) { m_MoveDistance = newDist; }
	void SetMovementDistanceX(float newX) { m_MoveDistance.x = newX; }
	void SetMovementDistanceY(float newY) { m_MoveDistance.y = newY; }

	///////////
	/// Get ///
	///////////

	const DirectX::XMFLOAT2& GetMovementDistance() { return m_MoveDistance; }
	float GetMovementDistanceX() { return m_MoveDistance.x; }
	float GetMovementDistanceY() { return m_MoveDistance.y; }


private:

	DirectX::XMFLOAT2 m_MoveDistance = { 0, 0 };

};