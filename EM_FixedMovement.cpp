#include "EM_FixedMovement.h"

void EM_FixedMovement::Move8(Directional8Way dirID, DirectX::XMFLOAT2& container)
{
	switch (dirID)
	{
	case(Directional8Way::UP_LEFT):
		container.x = -m_MoveDistance.x;
		container.y = -m_MoveDistance.y;
		break;
	case(Directional8Way::UP):
		container.y = -m_MoveDistance.y;
		break;
	case(Directional8Way::UP_RIGHT):
		container.x = m_MoveDistance.x;
		container.y = -m_MoveDistance.y;
		break;
	case(Directional8Way::RIGHT):
		container.x = m_MoveDistance.x;
		break;
	case(Directional8Way::DOWN_RIGHT):
		container.x = m_MoveDistance.x;
		container.y = m_MoveDistance.y;
		break;
	case(Directional8Way::DOWN):
		container.y = m_MoveDistance.y;
		break;
	case(Directional8Way::DOWN_LEFT):
		container.x = -m_MoveDistance.x;
		container.y = m_MoveDistance.y;
		break;
	case(Directional8Way::LEFT):
		container.x = -m_MoveDistance.x;
		break;
	}
}

DirectX::XMFLOAT2 EM_FixedMovement::Move8(Directional8Way dirID)
{
	DirectX::XMFLOAT2 ret;
	Move8(dirID, ret);
	return ret;
}

void EM_FixedMovement::Move4(Directional4Way dirID, DirectX::XMFLOAT2& container)
{
	switch (dirID)
	{
	case(Directional4Way::UP):
		container.y = -m_MoveDistance.y;
		break;
	case(Directional4Way::RIGHT):
		container.x = m_MoveDistance.x;
		break;
	case(Directional4Way::DOWN):
		container.y = m_MoveDistance.y;
		break;
	case(Directional4Way::LEFT):
		container.x = -m_MoveDistance.x;
		break;
	}
}

DirectX::XMFLOAT2 EM_FixedMovement::Move4(Directional4Way dirID)
{
	DirectX::XMFLOAT2 ret;
	Move4(dirID, ret);
	return ret;
}
