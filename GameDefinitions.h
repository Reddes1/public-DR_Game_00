#pragma once

/*
	Definitions needed by game for mode names, batches and fonts.
*/

//////////////////
/// Mode Names ///
//////////////////

/*
    Put any game modes/states here. Don't add more than is intended for use,
    and don't use one more than once (messes with the Manager)
*/
enum ModeNames
{
    MODE_SPLASH_SCREEN,
    MODE_MAIN_MENU,
    MODE_GAME_SCENE,
    MODE_COUNT
};

/////////////////////
/// Spritebatches ///
/////////////////////

/*
    May want to use different spritebatches, so specify and describe the purpose of each index here.
    Be sure to load them in the correct order)
*/
enum SpritebatchDefinitions
{
    MAIN_SB,
    POINT_WRAP_SB,
    SB_COUNT
};

///////////////////
/// Spritefonts ///
///////////////////

/*
    May have need for multiple spritefonts, so specify what each index points to.
    Be sure to load them in the correct order.
*/
enum SpritefontDefinitions
{
    CALIBRI_12_SF,
    CALIBRI_14_SF,
    CALIBRI_16_SF,
    CALIBRI_18_SF,
    CALIBRI_20_SF,
    CALIBRI_22_SF,
    SF_COUNT
};






