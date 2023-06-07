#pragma once
#include "D3DUtils.h"

/*
	Any high level constants, definitions or enumerations that the game or entities may need.
*/

///////////////////
/// Definitions ///
///////////////////


/////////////////
/// Constants ///
/////////////////

//
//Game Details
//

const int MAP_HEIGHT = 128;
const int MAP_WIDTH = 128;

const unsigned MAX_WEAPON_COUNT = 6;
const unsigned MAX_ACCESSORY_COUNT = 6;

//General entity offsetting for spawning/placing them off the screen
const int ENTITY_SCREEN_OFFSET_MIN = 100;
const int ENTITY_SCREEN_OFFSET_MAX = 200;
const float ENTITY_SCALE_ADJ_X = 0.66f;
const float ENTITY_SCALE_ADJ_Y = 0.66f;

//Amount that potions + level overflow management heals for
const short HEAL_AMOUNT = 25;

//Defines the minimum amount of time required before enemies can re-collide with the player
const float ENTITY_COLLISION_INTERVAL = 1.0f;

//Defines max amount of options to get post level up
const unsigned MAX_LEVEL_OPTIONS = 4;

//Defines max hit list size (amount of things before the list hard resets)
const unsigned MAX_HITLIST_SIZE = 50;

/*
	indexes start at 0, so a max index of 1 is equal to 2 indexes total
*/
//Defines max amount of main menu entries
const unsigned MAX_MAIN_MENU_INDEXES = 4;
//Defines max amount of pause menu entries
const unsigned MAX_PAUSE_MENU_INDEXES = 4;
//Defines the number of stat texts
const unsigned MAX_STAT_TEXTS = 12;
//Defines max amount of game over menu entries
const unsigned MAX_GAMEOVER_MENU_INDEXES = 2;
//Defines max amount of map selection entries
const unsigned MAX_MAP_INDEXES = 2;
//Defines max amount of character selection entries
const unsigned MAX_CHARACTER_INDEXES = 3;
//Defines amount of icons for each character during character selection
const unsigned MAX_CHARACTER_ICONS = 2; //change to 3 later when character sprite added to info panel
//Defines max amount of option menu entries
const unsigned MAX_OPTION_MENU_ENTRIES = 3;
//
//Other
//

const unsigned FIREBALL_DELAY = 6;
const unsigned BULLET_HELL_DELAY = 4;
const unsigned HOH_DELAY = 4;
