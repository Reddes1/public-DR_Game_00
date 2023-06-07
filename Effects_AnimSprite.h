#pragma once

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"	

#include "GameTimer.h"
#include "D3DUtils.h"
#include "GameTypes.h"

/*
	Simple class for producing animated sprite effects in a game. Designed to work as part of a pool system.
*/

class Effects_AnimSprite
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	//Defines method of how the effect ends (by time, animation, alpha etc)
	enum class OperationStateID : unsigned
	{
		ANIM_END,
		ANIM_LOOPS,
		ALPHA_FADE_OUT,
		ALPHA_FADE_IN,
		DURATION,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Effects_AnimSprite() { Init(); }
	~Effects_AnimSprite() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//
	//Inits
	//

	//Initialises the effect with texture and animation
	void InitialiseTexAndAnim(std::string& name, int animIndex);
	
	//Initialises the operational state, sprite data + optional additional data
	//duration used in states: DURATION, ALPHA_FADE_OUT, ALPHA_FADE_IN
	//loopCount used in states:: ANIM_LOOPS
	void InitData(OperationStateID state, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, float duration = 1.0f, unsigned short loopCount = 1);

	//Main Update Window
	void Update(GameTimer& gt);

	//Main Render Window
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);


	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }

	///////////
	/// Set ///
	///////////

	void InitPoolData(unsigned short poolID, unsigned short poolIndex);

private:

	//////////////////
	/// Operations ///
	//////////////////
	
	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	void FlagFreeInPool();

	////////////
	/// Data ///
	////////////

	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;

	//Pool information about where this comes from
	PoolIDs m_PoolData;

	//Current operational state
	OperationStateID m_State = OperationStateID::ANIM_END;

	//Control values
	float m_Elapsed = 0.f;
	float m_ElapsedEnd = 1.f;
	float m_CurrentAlpha = 0.f;
	unsigned short m_LoopCount = 0;
	unsigned short m_LoopMax = 1;
};