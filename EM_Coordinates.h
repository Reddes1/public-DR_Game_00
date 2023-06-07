#pragma once
#include "D3DUtils.h"

/*
	Coordinate module for tracking an entities place in world space.
*/
class EM_Coordinates
{
public:
	
	EM_Coordinates() {}
	~EM_Coordinates() {}


	//////////////////
	/// Operations ///
	//////////////////

	//Increments current coordinate by an 
	void IncrementXCoordinate(float incAmnt, float clampCap = -1.f, bool clampToZero = true);
	void IncrementYCoordinate(float incAmnt, float clampCap = -1.f, bool clampToZero = true);
	void IncrementZCoordinate(float incAmnt, float clampCap = -1.f, bool clampToZero = true);

	///////////
	/// Get ///
	///////////

	const DirectX::XMFLOAT3& GetMapCoordinates()		 { return m_MapCoordinates; }
	float GetMapCoordinatesX()							 { return m_MapCoordinates.x; }
	float GetMapCoordinatesY()							 { return m_MapCoordinates.y; }
	float GetMapCoordinatesZ()							 { return m_MapCoordinates.z; }
	int GetMapCoordinatesXi()							 { return static_cast<int>(m_MapCoordinates.x); }
	int GetMapCoordinatesYi()							 { return static_cast<int>(m_MapCoordinates.y); }
	int GetMapCoordinateZi()							 { return static_cast<int>(m_MapCoordinates.z); }
	DirectX::XMFLOAT2 GetMapCoordinatesXY()				 { return DirectX::XMFLOAT2(m_MapCoordinates.x, m_MapCoordinates.y); }
	DirectX::XMFLOAT2 GetMapCoordinatesXZ()				 { return DirectX::XMFLOAT2(m_MapCoordinates.x, m_MapCoordinates.z); }
	DirectX::XMFLOAT2 GetMapCoordinatesYZ()				 { return DirectX::XMFLOAT2(m_MapCoordinates.y, m_MapCoordinates.z); }



	///////////
	/// Set ///
	///////////

	void SetMapCoordinates(DirectX::XMFLOAT3& newCoords) { m_MapCoordinates = newCoords; }
	void SetMapCoordinatesX(float newX)					 { m_MapCoordinates.x = newX; }
	void SetMapCoordinatesY(float newY)					 { m_MapCoordinates.y = newY; }
	void SetMapCoordinatesZ(float newZ)					 { m_MapCoordinates.z = newZ; }
	void SetMapCoordinatesXY(float newX, float newY)	 { SetMapCoordinatesX(newX); SetMapCoordinatesY(newY); }
	void SetMapCoordinatesXZ(float newX, float newZ)	 { SetMapCoordinatesX(newX); SetMapCoordinatesZ(newZ); }
	void SetMapCoordinatesYZ(float newY, float newZ)	 { SetMapCoordinatesY(newY); SetMapCoordinatesZ(newZ); }


private:

	DirectX::XMFLOAT3 m_MapCoordinates = { 0.0f, 0.0f, 0.0f };

};