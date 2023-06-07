#pragma once
#include "D3DUtils.h"
#include "GameTypes.h"	//Collision DataType
/*
	Collection of different collision methods.
*/

namespace BK_Collision
{

	//////////////////////
	/// Radius Methods ///
	//////////////////////

	bool IsCircleInCircle(const CollisionArea& ob1, const CollisionArea& ob2);

	bool IsOvalInCircle(const CollisionArea& ob1, const CollisionArea& ob2);

	bool IsRectInCircle(const CollisionArea& ob1, const CollisionArea& ob2);

	bool IsNonAxisRectInCircle(const CollisionArea& ob1, const float rotation, const CollisionArea& ob2);
	
	////////////////////
	/// AABB Methods ///
	////////////////////

	//Rect / Rect Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	bool Rect2Rect(Vec4 lRect, Vec4 rRect);
	//Rect / Rect Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	//Takes addtional scaling arguments to uniformly shrink/enlarge collision space
	bool Rect2Rect_Scaled(Vec4 lRect, Vec4 rRect, float lRectXScale, float lRectYScale, float rRectXScale, float rRectYScale);
	

	//////////////////////
	/// Radius Methods ///
	//////////////////////

	//Circle / Circle Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	bool Cir2Cir(const Vec4& lCircle, const Vec4& rCircle);
	//Circle / Circle Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	//Takes addtional scaling arguments to uniformly shrink/enlarge collision space
	bool Cir2Cir_Scaled(Vec4 lCircle, Vec4 rCircle, float lCirjScale, float rCirScale);





	/////////////////////
	/// Mixed Methods ///
	/////////////////////

	//Circle / Rect Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	bool Cir2Rect(const Vec4& lCircle, Vec4 rRect);
	//Circle / Rect Collision: PosX, PosY, Width, Height. Position is assumed to be center origin.
	//Takes addtional scaling arguments to uniformly shrink/enlarge collision space
	bool Cir2Rect_Scaled(Vec4 lCircle, Vec4 rRect, float lCirScaling, float rRectScaleX, float rRectScaleY);


	/////////////////////
	/// Other Methods ///
	/////////////////////


	//Polygon (Uniquely Defined Shape) / Point Collision: Vertex Positions, Object Position / PosX, PosY, Width, Height.
	//Position is assumed to be center origin. 
	bool Polygon2Point(std::vector<Vec2> vertices, Vec2 lPolyPos, Vec4 rRect);
	//Line / Line Collision: StartX, StartY, EndX, EndY.
	bool Line2Line(const Vec2& lLineStart, const Vec2& lLineEnd, const Vec2& rLineStart, const Vec2 rLineEnd);
	//Line / Rect Collision: StartX, StartY, EndX, EndY / PosX, PosY, Width, Height. Position is assumed to center origin.
	bool Line2Rect(const Vec2& lLineStart, Vec2& lLineEnd, Vec4 rRect);

	//Polygon / Rect Collision: Vertex Positions, Object Position / PosX, PosY, Width, Height.
	//Position is assumed to be centre origin.
	//Additional flag to check to see if rect is inside of polygon shape (set for auto true).
	bool Polygon2Rect(std::vector<Vec2> vertices, Vec2 lPolyPos, Vec4 rRect, bool checkIfInside = true);

}



