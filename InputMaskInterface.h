#pragma once

#include "InputManager.h"

/*
	Masking interface designed to work with "InputManager - KeyboardAndMouse".
	Acts as go between for checking if mapped inputs have been registered in KBM.
*/
class InputMaskInterface
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	InputMaskInterface() {}
	virtual ~InputMaskInterface() {}

	////////////////
	/// Virtuals ///
	////////////////

	//Reset mappings to defaults (must be defined by child classes)
	virtual void ResetMapping() = 0;

	//////////////////
	/// Operations ///
	//////////////////

	//Bind this to KMB ONCE
	void BindToKBM(KeyboardAndMouse* const kbm);

	//Keyboard 
	bool IsPressed(unsigned short keyCode);
	bool IsPressed(unsigned short pKeyCode, unsigned short sKeyCode);
	bool IsPressedNoRepeat(unsigned short keyCode);
	bool IsPressedNoRepeat(unsigned short pKeyCode, unsigned short sKeyCode);
	bool IsKeyReleased(unsigned short keyCode);
	bool IsKeyReleased(unsigned short pKeyCode, unsigned short sKeyCode);

private:
	
	//Hold pointer to KBM Manager 
	KeyboardAndMouse* m_KBM = nullptr;

	//As container size can be variable to the game, define the container child class

};