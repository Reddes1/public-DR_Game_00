#pragma once

#include "CS_Interface.h"	//Parent

#include "InputManager.h"	//VK Definitions

class CS_Main_00 : public CS_Interface
{
public:
	////////////
	/// Data ///
	////////////

	enum class MappedKeyDefintions : unsigned short
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		CONFIRM,
		BACK,
		ACTION_COUNT
	};
	enum class DefaultInputs : unsigned short
	{
		MOVE_UP = VK_W,
		MOVE_DOWN = VK_S,
		MOVE_LEFT = VK_A,
		MOVE_RIGHT = VK_D,
		CONFIRM = VK_F,
		BACK = VK_ESCAPE,
	};

	////////////////////
	/// Constructors ///
	////////////////////

	CS_Main_00();
	~CS_Main_00() { CS_Interface::Release(); }


	/////////////////
	/// Overrides ///
	/////////////////

	void UpdateInputs() override;
	void UpdateProcessKey(char key);
	void ResetKeysToDefault() override;

	//////////////////
	/// Operations ///
	//////////////////


private:

	////////////
	/// Data ///
	////////////

	MappedKeyDefintions m_MappedKeys;

};