#pragma once

#include "EM_Sprite.h"
#include "EM_Animator.h"
#include "D3DUtils.h"

/*
	Specialised class to hold a number of indexes to slide through logos.
	Designed in mind to take one texture file compiled of multiple logos and goes through them at a set pace, with effect (NYI) 
*/
class LogoSlider
{
public:


	////////////
	/// Data ///
	////////////


	///////////////////
	/// Definitions ///
	///////////////////


	////////////////////
	/// Constructors ///
	////////////////////

	LogoSlider() { }
	~LogoSlider() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	void Initialise(std::string texFP);

	///////////
	/// Get ///
	///////////

	EM_Sprite& GetSprite() { return m_Sprite; }

	///////////
	/// Set ///
	///////////


private:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();
	
	////////////
	/// Data ///
	////////////

	EM_Sprite m_Sprite;
	EM_Animator m_Animator;

};