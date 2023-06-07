#include "CollisionManager.h"

bool CollisionManager::Rect2RectCollision(Vector4 lRect, Vector4 rRect)
{

    //Alter the rect positions for calculations (This moves them from origin @ center to Left/Top)
    lRect.x -= lRect.z * 0.5f;
    lRect.y -= lRect.w * 0.5f;

    rRect.x -= rRect.z * 0.5f;
    rRect.y -= rRect.w * 0.5f;

    //Check collision
    if (lRect.x + lRect.z >= rRect.x &&     //Rect1 Right edge past Rect2 Left
        lRect.x <= rRect.x + rRect.z &&     //Rect1 Left edge past Rect2 right
        lRect.y + lRect.w >= rRect.y &&     //Rect1 Top edge past Rect2 bottom
        lRect.y <= rRect.y + rRect.w)       //Rect1 bottom edge past Rect2 top
        return true;
    return false;

}

bool CollisionManager::Rect2RectCollision(Vector4 lRect, Vector4 rRect, float lRectXScale, float lRectYScale, float rRectXScale, float rRectYScale)
{

    lRect.z *= lRectXScale;
    lRect.w *= lRectYScale;
    rRect.z *= rRectXScale;
    rRect.w *= rRectYScale;

    return Rect2RectCollision(lRect, rRect);

}

bool CollisionManager::Cir2CirCollision(const Vector4 lObject, const Vector4 rObject)
{


    float radiusL = (Vector2(lObject.z, lObject.w).Length() * 0.5f);  //Adjust to make the hitbox a little more generous(Move to interface later)
    float radiusR = (Vector2(rObject.z, rObject.w).Length() * 0.5f);

    float distance = Vector2{ lObject.x - rObject.x, lObject.y - rObject.y }.Length();

    //Check collision
    if (distance <= radiusL + radiusR)
        return true;
    return false;

}

bool CollisionManager::Cir2CirCollision(Vector4 lObject, Vector4 rObject, float lObjScale, float rObjScale)
{

    //Rescale left circle
    lObject.z *= lObjScale;
    lObject.w *= lObjScale;
    //Rescale right circle
    rObject.z *= rObjScale;
    rObject.w *= rObjScale;

    //Pass new values through collision function and return
    return (Cir2CirCollision(lObject, rObject));

}

bool CollisionManager::Cir2RectCollision(Vector4 lObjectCir, Vector4 rObjectRect)
{
    //Alter the rect position for calculations (This moves it from origin @ center to Left/Top)
    rObjectRect.x -= rObjectRect.z * 0.5f;
    rObjectRect.y -= rObjectRect.w * 0.5f;
    //Get raidus of cicle obj & make the hitbox a little generous (Move this into an interface option later).
    float radius = (Vector2{ lObjectCir.z, lObjectCir.w }.Length() / 2.f) * 0.7f;

    //Comparion containers
    float compX = lObjectCir.x;
    float compY = lObjectCir.y;

    //Check what edges we are closest to
    if (lObjectCir.x < rObjectRect.x)                 //Left Edge
        compX = rObjectRect.x;
    else if (lObjectCir.x > rObjectRect.x + rObjectRect.z)   //Right Edge
        compX = rObjectRect.x + rObjectRect.z;
    if (lObjectCir.y < rObjectRect.y)                 //Top Edge
        compY = rObjectRect.y;
    else if (lObjectCir.y > rObjectRect.y + rObjectRect.w)   //Bottom Edge
        compY = rObjectRect.y + rObjectRect.w;

    //Calculate distance
    float distance = Vector2{ lObjectCir.x - compX, lObjectCir.y - compY }.Length();

    //Check collision
    if (distance <= radius)
        return true;
    return false;
}

bool CollisionManager::Cir2RectCollision(Vector4 lObjectCir, Vector4 rObjectRect, float lCirScaling, float rRectScaleX, float rRectScaleY)
{
    //Rescale circle
    lObjectCir.z *= lCirScaling;
    lObjectCir.w *= lCirScaling;
    //Rescale rect
    rObjectRect.z *= rRectScaleX;
    rObjectRect.w *= rRectScaleY;

    //Pass new values through collision function and return
    return Cir2RectCollision(lObjectCir, rObjectRect);
}


bool CollisionManager::Polygon2PointCollision(std::vector<Vector2> vertices, Vector2 lPolyPos, Vector4 rObjectRect)
{
    //Add current position given to shift the frame onto the sprite
    for (size_t i(0); i < vertices.size(); ++i)
    {
        vertices[i] += lPolyPos;
    }

    //Flags and container for collision check
    bool collision = false;
    int nextVertex = 0;

    //Run collision Check
    for (size_t currentVertex(0); currentVertex < vertices.size(); ++currentVertex)
    {
        nextVertex = currentVertex + 1;
        if (nextVertex == vertices.size())
            nextVertex = 0;

        //Capture current and next vertex positions
        Vector2 vectCurrent = vertices[currentVertex];
        Vector2 vectNext = vertices[nextVertex];

        //Use mathematics (Jordan Curve Theorem) to check for collision
        if (((vectCurrent.y >= rObjectRect.y && vectNext.y < rObjectRect.y)
            || (vectCurrent.y < rObjectRect.y && vectNext.y >= rObjectRect.y)) &&
            (rObjectRect.x < (vectNext.x - vectCurrent.x) * 
                (rObjectRect.y - vectCurrent.y) / (vectNext.y - vectCurrent.y) + vectCurrent.x))
                {
                    //Flip collision value
                    collision = !collision;
                }
    }

    return collision;
     
}

bool CollisionManager::Line2LineCollision(Vector2 line1End1, Vector2 line1End2, Vector2 line2End1, Vector2 line2End2)
{
    //Calulate direction of line1
    float uA = ((line2End2.x -line2End1.x) * (line1End1.y - line2End1.y) - (line2End2.y - line2End1.y) * (line1End1.x - line2End1.x)) /
        ((line2End2.y - line2End1.y) * (line1End2.x - line1End1.x) - (line2End2.x - line2End1.x) * (line1End2.y - line1End1.y));
    //Calulate direction of line2
    float uB = ((line1End2.x - line1End1.x) * (line1End1.y - line2End1.y) - (line1End2.y - line1End1.y) * (line1End1.x - line2End1.x))
        / ((line2End2.y - line2End1.y) * (line1End2.x - line1End1.x) - (line2End2.x - line2End1.x) * (line1End2.y - line1End1.y));
    
    //if uA and uB are between 0-1, they are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) 
        return true;
    return false;
}

bool CollisionManager::Line2RectCollision(Vector2 line1, Vector2 line2, Vector4 rRect)
{

    //Check if the line has hit any side of the rect using Line2Line

    bool left = Line2LineCollision(line1, line2, Vector2(rRect.x, rRect.y), Vector2(rRect.x, rRect.y + rRect.w));
    bool right = Line2LineCollision(line1, line2, Vector2(rRect.x + rRect.z, rRect.y), Vector2(rRect.x + rRect.z, rRect.y + rRect.w));
    bool top = Line2LineCollision(line1, line2, Vector2(rRect.x, rRect.y), Vector2(rRect.x + rRect.z, rRect.y));
    bool bottom = Line2LineCollision(line1, line2, Vector2(rRect.x, rRect.y + rRect.w), Vector2(rRect.x + rRect.z, rRect.y + rRect.w));

    // if ANY of the above are true,
    // the line has hit the rectangle
    if (left || right || top || bottom) 
        return true;
    return false;
}

bool CollisionManager::Polygon2RectCollision(std::vector<Vector2> vertices, Vector2 lPolyPos, Vector4 rObjectRect, bool checkIsInside)
{

    //Alter the rect positions for calculations (This moves them from origin @ center to Left/Top)
    rObjectRect.x -= rObjectRect.z * 0.5f;
    rObjectRect.y -= rObjectRect.w * 0.5f;

    //Add current position given to shift the frame onto the sprite
    for (size_t i(0); i < vertices.size(); ++i)
    {
        vertices[i] += lPolyPos;
    }

    //Flags and container for collision check
    bool collision = false;
    int nextVertex = 0;

    //Run collision Check
    for (int currentVertex(0); currentVertex < vertices.size(); ++currentVertex)
    {
        nextVertex = currentVertex + 1;
        if (nextVertex == vertices.size())
            nextVertex = 0;

        //Capture current and next vertex positions
        Vector2 vectCurrent = vertices[currentVertex];
        Vector2 vectNext = vertices[nextVertex];
        // check against all four sides of the rectangle
        collision = Line2RectCollision(vectCurrent, vectNext, rObjectRect);
        if (collision)
            return true;

        
        //option check to see if the rect is inside the polygon for collision purposes
        if (checkIsInside)
        {
            bool inside = Polygon2PointCollision(vertices, lPolyPos, rObjectRect);
            if (inside)
                return true;
        }
    }

    return false;
}

