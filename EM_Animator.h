#pragma once
#include "EM_Sprite.h"			//For setting frame when needed

/*
	Specialised animator module. Paired to work with "EM_Sprite"
*/
class EM_Animator
{
public:


	////////////
	/// Data ///
	////////////

	struct AnimatorData
	{
		AnimatorData()
			:m_Elapsed(0), m_Speed(1), m_CurrentFrame(0), m_Play(false), m_Loop(false), m_Reverse(false)
		{}
		float m_Elapsed;
		float m_Speed;
		short m_CurrentFrame;
		bool m_Play : 1;
		bool m_Loop : 1;
		bool m_Reverse : 1;
		
	};


	////////////////////
	/// Constructors ///
	////////////////////

	
	EM_Animator() {}
	EM_Animator(EM_Sprite* sprPtr)
		:m_Sprite(sprPtr)
	{}
	~EM_Animator() { }


	//////////////////
	/// Operations ///
	//////////////////

	//Main update window
	void Update(float dTime);

	//Restarts the current animation
	void RestartAnimation(bool play);

	///////////
	/// Get ///
	///////////

	float GetSpeed()											   { return m_Data.m_Speed; }
	bool GetPlayState()											   { return m_Data.m_Play; }
	bool GetLoopState()											   { return m_Data.m_Loop; }
	bool GetReverse()											   { return m_Data.m_Reverse; }
	short GetCurrentFrame()										   { return m_Data.m_CurrentFrame; }
	short GetFrame()											   { return m_CurrentAnim->m_FrameIndexes[m_Data.m_CurrentFrame]; }
	short GetAnimationLength();
	//Gets the frame relative to the current animation (
	short GetRelativeCurrentFrame();

	const AnimationData* GetCurrentAnimation() const			   { return m_CurrentAnim; }

	///////////
	/// Set ///
	///////////

	//Set sprite pointer (IMPORTANT THAT THIS IS DONE!)
	void SetSpritePointer(EM_Sprite* ptr)						   { m_Sprite = ptr; }
																   
	void SetSpeed(float newSpeed)								   { m_Data.m_Speed = newSpeed; }
	void SetPlay(bool play)										   { m_Data.m_Play = play; }
	void SetLoop(bool loop)										   { m_Data.m_Loop = loop; }
	void SetReverse(bool reverse)								   { m_Data.m_Reverse = reverse; }
	//Sets frame relative to the number in the index container
	void SetFrame(int newFrame, bool resetElapsed = true);

	//Set Animation by index through sprite texture data with optional control settings.
	void SetAnimation(int index, bool play = true, bool loop = true, bool reverse = false, bool restartAnim = true);


private:

	void LinearAnimation(float dTime);
	void NonLinearAnimation(float dTime);

	void SetFrameAndOrigin();

	//Animation Data
	AnimatorData m_Data;
	//Currently active animation
	const AnimationData* m_CurrentAnim = nullptr;
	//Associated sprite that this animator is bound with
	EM_Sprite* m_Sprite = nullptr;

};