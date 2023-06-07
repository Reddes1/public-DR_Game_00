#pragma once
#include "Tiled_Types.h"
#include "EM_Sprite.h"

/*
	The tile animation structure is different to how other sprites work (for now..?), 
	so this is a specialised animator is used to fix that.
*/

class MT_Animator
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	MT_Animator() {}
	~MT_Animator() {}

	//////////////////
	/// Operations ///
	//////////////////

	void UpdateAnimation(float dTime);

	///////////
	/// Get ///
	///////////


	///////////
	/// Set ///
	///////////

	void SetPlayState(bool play) { m_Play = play; }
	void SetSprite(EM_Sprite* spr) { m_Sprite = spr; }
	void SetAnimArray(std::vector<TileAnim>& arr) { m_Animations = arr; }

private:
	//Store Animation Data Locally
	std::vector<TileAnim> m_Animations;
	//Anim Control Data
	int m_CurrentFrameIndex = 0;
	float m_Elapsed = 0.f;
	//Associcated Sprite
	EM_Sprite* m_Sprite;
	bool m_Play = true;

};