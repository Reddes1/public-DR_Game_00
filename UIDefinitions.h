#pragma once

/*
	Define any UI expected to be loaded via file (i.e "UI_Config_Example_00") here.
	UI Manage is defaulted to reserve in accordance to the size of -
		- NonNavUIDefinitions
		- FixedIndexNavDefinitions

	If loading just one file, you can use replace the contents as required to file. However if loading more than one file
	during the runtime of the program, you will need to create a new enum that describes what ID points to what.
*/

enum class NonNavUIDefinitions : int
{
	TILE_TOOLTIP_00,
	UNIT_FRAME_00,
	UNIT_FRAME_01,
	MESSAGE_BOX_00,
	MESSAGE_BOX_01,
	UNIT_INFO_WINDOW_00,
	COUNT
};

enum class FixedIndexNavDefintions : int
{
	UNIT_ACTION_MENU_00,
	GAME_MENU_MASTER_00,
	MAIN_MENU_00,
	SCENARIO_OVER_MENU_00,
	COUNT
};