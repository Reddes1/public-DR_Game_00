#pragma once

#include "InputMaskInterface.h"	//Parent

/*
	Child control scheme class. Designed for use with game "Death's Rebuke".
*/
class CS_Survivor_00 : public InputMaskInterface
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	//Defines input meanings
	enum class MappedActions : unsigned short
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		ACTION_00,
		ACTION_01,
		BACK,
		COUNT
	};
	//Default mappings
	enum class DefaultMappingPrimary : unsigned short
	{
		MOVE_UP = VK_W,
		MOVE_DOWN = VK_S,
		MOVE_LEFT = VK_A,
		MOVE_RIGHT = VK_D,
		ACTION_00 = VK_E,
		ACTION_01 = VK_Q,
		BACK = VK_ESCAPE,
		COUNT
	};
	enum class DefaultMappingSecondary : unsigned short
	{
		MOVE_UP = VK_UP,
		MOVE_DOWN = VK_DOWN,
		MOVE_LEFT = VK_LEFT,
		MOVE_RIGHT = VK_RIGHT,
		ACTION_00 = VK_RETURN,
		ACTION_01 = VK_SPACE,
		BACK = VK_BACK,
		COUNT
	};

	typedef DefaultMappingPrimary DMP;
	typedef DefaultMappingSecondary DMS;

	////////////////////
	/// Constructors ///
	////////////////////

	CS_Survivor_00() {}
	~CS_Survivor_00() {}

	////////////////
	/// Override ///
	////////////////

	void ResetMapping() override;

	///////////
	/// Get ///
	///////////

	//Get code in primary mapped keys at index
	unsigned short GetPrimaryKeyAtIndex(MappedActions index);
	//Get code in secondary mapped keys at index
	unsigned short GetSecondaryKeyAtIndex(MappedActions index);


private:

	////////////
	/// Data ///
	////////////

	unsigned short m_PrimaryMappedKeys[(unsigned short)DMP::COUNT];
	unsigned short m_SecondaryMappedKeys[(unsigned short)DMS::COUNT];

};