#pragma once

/*
	Define any audio defintions here.
*/

///////////////////////////
/// WaveBank Defintions ///
///////////////////////////

//Define what indexes point to what in the map

enum WBIndexes
{
	WB_MENU_TEST_00,
	WB_COUNT
};

///////////////////////////////////
/// WaveBank Content Defintions ///
///////////////////////////////////

//Define what the indexes are within a wavebank

enum MENU_TEST_SOUNDS_00 : unsigned int
{
    MENU_TEST_SOUNDS_00_DFS_EARTH_01 = 0,
    MENU_TEST_SOUNDS_00_DFS_EARTH_03 = 1,
    MENU_TEST_SOUNDS_00_DFS_EARTH_05 = 2,
    MENU_TEST_SOUNDS_00_DFS_EARTH_07 = 3,
    MENU_TEST_SOUNDS_00_DFS_EARTH_09 = 4,
    MENU_TEST_SOUNDS_00_DFS_EARTH_11 = 5,
    MENU_TEST_SOUNDS_00_DFS_EARTH_13 = 6,
    MENU_TEST_SOUNDS_00_DFS_EARTH_15 = 7,
    MENU_TEST_SOUNDS_01_DFS_THE_MYST_SEAMLESS = 8,
};

#define XACT_WAVEBANK_MENU_TEST_SOUNDS_00_ENTRY_COUNT 8
