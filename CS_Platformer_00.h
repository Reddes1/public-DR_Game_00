#pragma once

#include "CS_Interface.h" //Parent

#include "InputManager.h"	//VK Definition

class CS_Platformer_00 : public CS_Interface
{
public:

	////////////
	/// Data ///
	////////////

	enum class MappedKeyDefintions : unsigned short
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		JUMP,
		ACTION_COUNT
	};
	enum class DefaultInputs : unsigned short
	{
		MOVE_LEFT = VK_A,
		MOVE_RIGHT = VK_D,
		JUMP = VK_SPACE
	};

	////////////////////
	/// Constructors ///
	////////////////////

	CS_Platformer_00() :CS_Interface(static_cast<int>(MappedKeyDefintions::ACTION_COUNT)) {};
	~CS_Platformer_00() { CS_Interface::Release(); }


	/////////////////
	/// Overrides ///
	/////////////////

	void UpdateInputs() override;
	void UpdateProcessKey(char key);
	void ResetKeysToDefault() override;


private:

};