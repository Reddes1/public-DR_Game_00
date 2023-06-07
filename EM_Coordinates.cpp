#include "EM_Coordinates.h"

void EM_Coordinates::IncrementXCoordinate(float incAmnt, float clampCap, bool clampToZero)
{
	//Increment
	m_MapCoordinates.x += incAmnt;

	//Check against top end
	if (clampCap > 0 && m_MapCoordinates.x > clampCap)
		m_MapCoordinates.x = clampCap;
	//Check against bottome end
	if (clampToZero && m_MapCoordinates.x < 0.f)
		m_MapCoordinates.x = 0.f;
}

void EM_Coordinates::IncrementYCoordinate(float incAmnt, float clampCap, bool clampToZero)
{
	//Increment
	m_MapCoordinates.y += incAmnt;

	//Check against top end
	if (clampCap > 0 && m_MapCoordinates.y > clampCap)
		m_MapCoordinates.y = clampCap;
	//Check against bottome end
	if (clampToZero && m_MapCoordinates.y < 0.f)
		m_MapCoordinates.y = 0.f;
}

void EM_Coordinates::IncrementZCoordinate(float incAmnt, float clampCap, bool clampToZero)
{
	//Increment
	m_MapCoordinates.z += incAmnt;

	//Check against top end
	if (clampCap > 0 && m_MapCoordinates.z > clampCap)
		m_MapCoordinates.z = clampCap;
	//Check against bottome end
	if (clampToZero && m_MapCoordinates.z < 0.f)
		m_MapCoordinates.z = 0.f;
}
