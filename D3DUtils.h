#pragma once
#include "All_Includes.h"
#include "D3DUtils_Debug.h" //Debug tools

#include "MathHelper.h"		//FL

using Microsoft::WRL::ComPtr;

//
// Suite of DX & Game related utilities, support types functions etc. for use.
// "All_Includes" or equiv should be placed here, and just include this file where required.
//

//-------------------------------------------------------------------------------
// Type Defininitions
//-------------------------------------------------------------------------------

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;


//-------------------------------------------------------------------------------
// Program Constants
//-------------------------------------------------------------------------------

//PI is always helpful (and tasty!)
const static float PI = 3.1415926535f;
//Gravity Constant
const static float GRAVITY = 9.81f;
//Frame Count
const int NUM_FRAME_RESOURCES = 3;

//Common rotations
const float ROTATION_90 = PI / 2.f;
const float ROTATION_180 = PI;
const float ROTATION_270 = PI * 1.5f;


//-------------------------------------------------------------------------------
// Geometry, Textures & Materials Structures + Functions
//-------------------------------------------------------------------------------


//Testing structure for non-linear animation testing (using string of frame indexes instead of inc/dec animation)
/*
	Animation structure for storing information about frames relating to an associated texture
*/
struct AnimationData
{
	enum class AnimID : unsigned short
	{
		LINEAR_FRAMES,
		NON_LINEAR_FRAMES
	};

	AnimationData()
		:m_Name("N/A"), m_Speed(0), m_StartFrame(0), m_EndFrame(0), m_TypeID(AnimID::LINEAR_FRAMES)
	{
		m_FrameIndexes.reserve(8);
	}
	//Setup for Linear
	AnimationData(std::string& name, short startFrame, short endFrame, float animSpeed)
		:m_Name(name), m_StartFrame(startFrame), m_EndFrame(endFrame),
		m_Speed(animSpeed), m_TypeID(AnimID::LINEAR_FRAMES)
	{

	}
	~AnimationData()
	{
		m_FrameIndexes.clear();
	}

	std::vector<short> m_FrameIndexes;
	std::string m_Name;
	float m_Speed;
	short m_StartFrame;
	short m_EndFrame;
	AnimID m_TypeID;

	//Gets the number of frames for linear frames
	short int GetAnimFrameCount() { return m_EndFrame - m_StartFrame; }
};

//More specialised Sprite Texture Container. Holds more information that we might typically
//need in sprite work
struct SpriteTexture
{
	std::vector<RECT> m_Frames;
	std::vector<AnimationData> m_Animations;
	std::vector<DirectX::XMFLOAT2> m_Origins;
	std::wstring m_Filename;
	size_t m_SrvIndex;
	ComPtr<ID3D12Resource> m_Resource = nullptr;

	~SpriteTexture()
	{
		m_Frames.clear();
		m_Animations.clear();
	}
};

//Generates a vector of face normals for an object
void GenerateFaceNormals(std::vector<DirectX::SimpleMath::Vector3>& vertices, std::vector<DirectX::SimpleMath::Vector3>& normals, int numOfHalfQuads);


//-------------------------------------------------------------------------------
// Lighting & Camera Structures + Functions
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// Supporting Types/Structures
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// Shader Constant Buffer Containers
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// Mathematical Helpers/Functions/Types
//-------------------------------------------------------------------------------

//Get random value from range
float GetRandFromRange(float min, float max);

//Get random seed
void SeedRandom(int seed);

//Convert Degrees into Radian
inline float DegreesToRadian(float x) { return (x * PI) / 180.0f; }

//Convert Radian into Degrees
inline float RadianToDegrees(float x) {	return (x * 180.0f) / PI; }

//Linear Interpolation
float Lerp(float start, float end, float time);
//update the aspect ratio and recompute the projection matrix.
void CreateProjectionMatrix(DirectX::SimpleMath::Matrix& projM, float fieldOfView, float aspectRatio, float nearZ, float farZ);

//the view matrix represents the camera
void CreateViewMatrix(DirectX::SimpleMath::Matrix& viewM,
	const DirectX::SimpleMath::Vector3& camPos,
	const DirectX::SimpleMath::Vector3& camTgt,
	const DirectX::SimpleMath::Vector3& camUp);

//Invert a matrix
DirectX::SimpleMath::Matrix InverseTranspose(const DirectX::SimpleMath::Matrix& m);

//Returns an index value in an vector array is being suedo used as a 2d array
//A tile map is a good example of this (where x=0, y=0 represents the first index)
//Only use if coordinates are positive only (or pre-adjust negatives before calling this function)
inline size_t GetVectorIndexByCoordinate(size_t x, size_t y, size_t maxX)
{
	return (x + y) + (y * maxX);
}


//-------------------------------------------------------------------------------
// Timer Types/Functions
//-------------------------------------------------------------------------------

////////////////////////////
/// Game Timer Version 1 ///
////////////////////////////

//Get the current game run time
float GetClock();
//Get the elapsed frame time
float GetDeltaTime();
//Add time to clock (add dTime).
void AddSecToClock(float sec);
//If we want to reset or manually alter the game time.
void SetClockTime(float value);

////////////////////////////
/// Game Timer Version 2 ///
////////////////////////////



//-------------------------------------------------------------------------------
// D3D Support Types/Functions
//-------------------------------------------------------------------------------

//Simplified structure for 2D Usage (No need to manage Shader Constants for now)
struct FrameResource2D
{
public:

	FrameResource2D(ID3D12Device* device);
	FrameResource2D(const FrameResource2D& rhs) = delete;
	FrameResource2D& operator=(const FrameResource2D& rhs) = delete;
	~FrameResource2D();
	// We cannot reset the allocator until the GPU is done processing the commands.
	// So each frame needs their own allocator.
	ComPtr<ID3D12CommandAllocator> CmdListAlloc;

	// Fence value to mark commands up to this fence point.  This lets us
	// check if these frame resources are still in use by the GPU.
	UINT64 Fence = 0;

};

//Forward Dec
class Game;

//Wraps up common data for passing to modes or states (expand as required)
struct PassData
{
	PassData()
	{
		m_Spritebatches.reserve(8);
		m_Spritefonts.reserve(8);
	}

	std::vector<std::unique_ptr<DirectX::SpriteBatch>> m_Spritebatches;
	std::vector<std::unique_ptr<DirectX::SpriteFont>> m_Spritefonts;
	DirectX::DescriptorHeap* m_Heap = nullptr;
};

/*
	Macro for releasing COM Objects (aka Resources).
	If !null, then release and null. Func/Data that is created by
	another system and will not go till released properly.
	Saves typing and silly bugs.
*/
#define ReleaseCOM(x) \
{	if(x){	x->Release();	x = 0;	} }	


//Definitions for describing string justification positions when drawing with Spritefont
enum class JustificationPosition : unsigned short
{
	LEFT_TOP,
	LEFT,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT,
	RIGHT_BOTTOM,
	TOP,
	CENTER,
	BOTTOM
};

//enum directions as values for use in classes/functions
enum class Directional4Way : unsigned short
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	COUNT
};
enum class Directional8Way : unsigned short
{
	NULL_DIR,
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8,
	UP_LEFT = UP + LEFT,
	UP_RIGHT = UP + RIGHT,
	DOWN_LEFT = DOWN + LEFT,
	DOWN_RIGHT = DOWN + RIGHT
};

//Pass justification index (STRING_JUSTIFY_POS) and calculate origin point for string
void CalculateStringJustifyPosition(DirectX::SpriteFont* sf, short index, std::string& message, DirectX::SimpleMath::Vector2& origin);


//-------------------------------------------------------------------------------
// Miscellaneous / Undefined Categories
//-------------------------------------------------------------------------------

/*
	We like colour definitions for things, so here are some predefined colours
	for use here (Expand as needed, most are based on webcolours).
*/
namespace Colours
{
	const DirectX::SimpleMath::Vector4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 DimGray = { 0.412f, 0.412f, 0.412f, 1.0f };
	const DirectX::SimpleMath::Vector4 AliceBlue = { 0.941f, 0.972f, 1.0f, 1.0f };
	const DirectX::SimpleMath::Vector4 AntiqueWhite = { 0.98f, 0.922f, 0.843f, 1.0f };
	const DirectX::SimpleMath::Vector4 Aquamarine = { 0.5f, 1.0f, 0.831f, 1.0f };
	const DirectX::SimpleMath::Vector4 Crimson = { 0.863f, 0.078f, 0.235f, 1.0f };
	const DirectX::SimpleMath::Vector4 SaddleBrown = { 0.545f, 0.271f, 0.074f, 1.0f };
}

//Convert from Vec3 to Vec4 (DX Types)
inline DirectX::SimpleMath::Vector4 Vec3To4(const DirectX::SimpleMath::Vector3& src, float w)
{
	return DirectX::SimpleMath::Vector4(src.x, src.y, src.z, w);
}

//Convert from Vec4 to Vec3 (DX Types)
inline DirectX::SimpleMath::Vector3 Vec4To3(const DirectX::SimpleMath::Vector4& src)
{
	return DirectX::SimpleMath::Vector3(src.x, src.y, src.z);
}

/*
	Comparing floating point numbers is tricky. A number is rarely zero or one, it's more common for it
	to be 0.001 or 0.998. So we need a way of comparing numbers that takes this into account,
	this simple approach won't work for everything, but it's useful most of the time.
*/
const float VERY_SMALL = 0.0001f;
inline bool Equals(float a, float b)
{
	return (fabs(a - b) < VERY_SMALL) ? true : false;
}
