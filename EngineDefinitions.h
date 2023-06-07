#pragma once

/*
	Generally helpful defintions for use in the engine at anypoint for easy adjustment and visability to the program.
*/

//Primary Loading Manifest (will serve as a replacement to most of the below when done)
#define MASTER_MANIFEST_FP "data/files/Master_Manifest_00.json"


/////////////////////////
/// Draw Layer Depths ///
/////////////////////////

#define DRAW_DEPTH_MAX 1.f
#define DRAW_DEPTH_MIN 0.f

#define DRAW_DEPTH_MISC_LOW_0 0.00f
#define DRAW_DEPTH_MISC_LOW_1 0.01f
#define DRAW_DEPTH_MISC_LOW_2 0.02f
#define DRAW_DEPTH_MISC_LOW_3 0.03f
#define DRAW_DEPTH_MISC_LOW_4 0.04f
#define DRAW_DEPTH_MISC_LOW_5 0.05f
#define DRAW_DEPTH_MISC_LOW_6 0.06f
#define DRAW_DEPTH_MISC_LOW_7 0.07f
#define DRAW_DEPTH_MISC_LOW_8 0.08f
#define DRAW_DEPTH_MISC_LOW_9 0.09f

#define DRAW_DEPTH_BACKGROUND_0 0.10f
#define DRAW_DEPTH_BACKGROUND_1 0.11f
#define DRAW_DEPTH_BACKGROUND_2 0.12f
#define DRAW_DEPTH_BACKGROUND_3 0.13f
#define DRAW_DEPTH_BACKGROUND_4 0.14f
#define DRAW_DEPTH_BACKGROUND_5 0.15f
#define DRAW_DEPTH_BACKGROUND_6 0.16f
#define DRAW_DEPTH_BACKGROUND_7 0.17f
#define DRAW_DEPTH_BACKGROUND_8 0.18f
#define DRAW_DEPTH_BACKGROUND_9 0.19f

#define DRAW_DEPTH_FOREGROUND_0 0.20f
#define DRAW_DEPTH_FOREGROUND_1 0.21f
#define DRAW_DEPTH_FOREGROUND_2 0.22f
#define DRAW_DEPTH_FOREGROUND_3 0.23f
#define DRAW_DEPTH_FOREGROUND_4 0.24f
#define DRAW_DEPTH_FOREGROUND_5 0.25f
#define DRAW_DEPTH_FOREGROUND_6 0.26f
#define DRAW_DEPTH_FOREGROUND_7 0.27f
#define DRAW_DEPTH_FOREGROUND_8 0.28f
#define DRAW_DEPTH_FOREGROUND_9 0.29f

#define DRAW_DEPTH_ENTITY_0 0.40f
#define DRAW_DEPTH_ENTITY_1 0.41f
#define DRAW_DEPTH_ENTITY_2 0.42f
#define DRAW_DEPTH_ENTITY_3 0.43f
#define DRAW_DEPTH_ENTITY_4 0.44f
#define DRAW_DEPTH_ENTITY_5 0.45f
#define DRAW_DEPTH_ENTITY_6 0.46f
#define DRAW_DEPTH_ENTITY_7 0.47f
#define DRAW_DEPTH_ENTITY_8 0.48f
#define DRAW_DEPTH_ENTITY_9 0.49f

#define DRAW_DEPTH_PROJECTILE_0 0.50f
#define DRAW_DEPTH_PROJECTILE_1 0.51f
#define DRAW_DEPTH_PROJECTILE_2 0.52f
#define DRAW_DEPTH_PROJECTILE_3 0.53f
#define DRAW_DEPTH_PROJECTILE_4 0.54f
#define DRAW_DEPTH_PROJECTILE_5 0.55f
#define DRAW_DEPTH_PROJECTILE_6 0.56f
#define DRAW_DEPTH_PROJECTILE_7 0.57f
#define DRAW_DEPTH_PROJECTILE_8 0.58f
#define DRAW_DEPTH_PROJECTILE_9 0.59f

#define DRAW_DEPTH_UI_0 0.60f
#define DRAW_DEPTH_UI_1 0.61f
#define DRAW_DEPTH_UI_2 0.62f
#define DRAW_DEPTH_UI_3 0.63f
#define DRAW_DEPTH_UI_4 0.64f
#define DRAW_DEPTH_UI_5 0.65f
#define DRAW_DEPTH_UI_6 0.66f
#define DRAW_DEPTH_UI_7 0.67f
#define DRAW_DEPTH_UI_8 0.68f
#define DRAW_DEPTH_UI_9 0.69f

#define DRAW_DEPTH_HI_TERRAIN_0 0.70f
#define DRAW_DEPTH_HI_TERRAIN_1 0.71f
#define DRAW_DEPTH_HI_TERRAIN_2 0.72f
#define DRAW_DEPTH_HI_TERRAIN_3 0.73f
#define DRAW_DEPTH_HI_TERRAIN_4 0.74f
#define DRAW_DEPTH_HI_TERRAIN_5 0.75f
#define DRAW_DEPTH_HI_TERRAIN_6 0.76f
#define DRAW_DEPTH_HI_TERRAIN_7 0.77f
#define DRAW_DEPTH_HI_TERRAIN_8 0.78f
#define DRAW_DEPTH_HI_TERRAIN_9 0.79f

#define DRAW_DEPTH_MISC_HIGH_0 0.90f
#define DRAW_DEPTH_MISC_HIGH_1 0.91f
#define DRAW_DEPTH_MISC_HIGH_2 0.92f
#define DRAW_DEPTH_MISC_HIGH_3 0.93f
#define DRAW_DEPTH_MISC_HIGH_4 0.94f
#define DRAW_DEPTH_MISC_HIGH_5 0.95f
#define DRAW_DEPTH_MISC_HIGH_6 0.96f
#define DRAW_DEPTH_MISC_HIGH_7 0.97f
#define DRAW_DEPTH_MISC_HIGH_8 0.98f
#define DRAW_DEPTH_MISC_HIGH_9 0.99f


//////////
/// UI ///
//////////

#define UI_RESERVE_SIZE 16
#define DEFAULT_UI_CONFIG_FP "data/files/UI_Config_Example_00.json"

#define BK_UI_00_TEXTURE_NAME "BK_UI_Atlas_00"
#define BK_UI_00_TEXTURE_FP L"data/textures/ui/BK_UI_Package_00.dds"
#define BK_UI_00_TEXTURE_JSON_FP "data/textures/ui/BK_UI_Package_00.json"


/////////////////
/// Texturing ///
/////////////////

#define DEFAULT_TESTING_TEXTURE_NAME "Testing_Texture_00"
#define DEFAULT_TESTING_TEXTURE_FP L"data/textures/misc/Testing_Texture_00.dds"
#define DEFAULT_TESTING_TEXTURE_JSON_FP "data/textures/misc/Testing_Texture_00.json"

#define DEFAULT_ANIM_TESTING_TEXTURE_NAME "Anim_Test_SS_00"
#define DEFAULT_ANIM_TESTING_TEXTURE_FP L"data/textures/sprites/Anim_Test_SS_00.dds"
#define DEFAULT_ANIM_TESTING_TEXTURE_JSON_FP "data/textures/sprites/Anim_Test_SS_00.json"

#define DEFAULT_TEST_BG_TEXTURE_NAME "Test_BG_00"
#define DEFAULT_TEST_BG_TEXTURE_FP L"data/textures/backgrounds/RB_WP_00.dds"


/////////////
/// Fonts ///
/////////////

#define DEFAULT_FONT_CALIBRI_12_FP L"data/fonts/Default_Calibri_12.spritefont"
#define DEFAULT_FONT_CALIBRI_14_FP L"data/fonts/Default_Calibri_14.spritefont"
#define DEFAULT_FONT_CALIBRI_16_FP L"data/fonts/Default_Calibri_16.spritefont"
#define DEFAULT_FONT_CALIBRI_18_FP L"data/fonts/Default_Calibri_18.spritefont"
#define DEFAULT_FONT_CALIBRI_20_FP L"data/fonts/Default_Calibri_20.spritefont"
#define DEFAULT_FONT_CALIBRI_22_FP L"data/fonts/Default_Calibri_22.spritefont"


//////////////////////////
/// Exceptions & Codes ///
//////////////////////////

#define BKE_OK 1
#define BKE_BASIC_ERROR 0
#define BKE_TEXTURE_LOAD_ERROR 101
#define BKE_TEXTURE_SET_ERROR 102
#define BKE_TEXTURE_FIND_ERROR 103
#define BKE_UI_LOAD_ERROR 201
#define BKE_UI_SET_ERROR 202
#define BKE_UI_FIND_ERROR 203
#define BKE_SFX_LOAD_ERROR 301
