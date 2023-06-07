#pragma once
/*
    Credit to http://www.jeffreythompson.org/collision-detection/ - Methods derived from this source.
*/
#include "D3DUtil.h"
/*
    Collection of collision methods. All methods assume origin is centered.
*/

using namespace DirectX::SimpleMath;

class CollisionManager
{
public:
    CollisionManager()
    {}
    ~CollisionManager()
    {}

    //Rect/Rect collision, Takes Vector4 (posX, posY, Width, Height). Assumes position is centered origin.
    bool Rect2RectCollision(Vector4 lRect, Vector4 rRect);
    //Rect/Rect collision, Takes Vector4 (posX, posY, Width, Height). Assumes position is centered origin. Takes scaling arguments.
    bool Rect2RectCollision(Vector4 lRect, Vector4 rRect, float lRectXScale, float lRectYScale, float rRectXScale, float rRectYScale);
    
    //Cir/Cir Collision, Takes Vector4 (posX, posY, Width, Height) Assumes position is centered origin.
    bool Cir2CirCollision(const Vector4 lObject, const Vector4 rObject);
    //Cir/Cir Collision, Takes Vector4 (posX, posY, Width, Height) Assumes position is centered origin. Takes scaling arguments.
    bool Cir2CirCollision(Vector4 lObject, Vector4 rObject, float lObjScale, float rObjScale);

    //Cir/Rect Collision, Takes Vector4 (posX, posY, Width, Height). Assumes position is centered origin.
    bool Cir2RectCollision(Vector4 lObjectCir, Vector4 rObjectRect);
    //Cir/Rect Collision, Takes Vector4 (posX, posY, Width, Height). Assumes position is centered origin. Takes scaling arguments.
    bool Cir2RectCollision(Vector4 lObjectCir, Vector4 rObjectRect, float lCirScaling, float rRectScaleX, float rRectScaleY);

    //Polygon/Point collision, give vectex data + position and point position.
    bool Polygon2PointCollision(std::vector<Vector2> vertices, Vector2 lPolyPos, Vector4 rObjectRect);

    bool Line2LineCollision(Vector2 value1, Vector2 value2, Vector2 value3, Vector2 value4);

    bool Line2RectCollision(Vector2 line1, Vector2 line2, Vector4 rObjectRect);

    bool Polygon2RectCollision(std::vector<Vector2> vertices, Vector2 lPolyPos, Vector4 rObjectRect, bool checkIsInside = false);


private:

};