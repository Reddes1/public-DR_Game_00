#pragma once

#include "D3DUtils.h"

//Reflects what the main properties of a tile are, and are used in the loading process
//Must match the order that they appear in the json file 

//-------------------------------------------------------------------------------
// Enumerations & Definitions
//-------------------------------------------------------------------------------

enum TPIndexes
{
	TPI_COLLIDEABLE,
	TPI_COLLISION_LAYER_ID,
	TPI_DRAW_LAYER_ID,
	TPI_FOREGROUND_DRAW,
	TPI_IGNORE,
	TPI_SPAWNABLE,
	TPI_NAME
};

enum TileSpriteIndexes
{
	TS_TERRAIN_TILE_00,
	TS_TERRAIN_TILE_01,
	TS_DECORATION_TILE_00,
	TS_HIGHLIGHT_TILE_00,
	TS_COUNT
};

enum class TileOccupationStatus : short
{
	NONE,
	TEAM_01,
	TEAM_02
};

//Tile neighbour indexes
enum TNIndexes
{
	TNI_NORTH,
	TNI_NORTH_EAST,
	TNI_EAST,
	TNI_SOUTH_EAST,
	TNI_SOUTH,
	TNI_SOUTH_WEST,
	TNI_WEST,
	TNI_NORTH_WEST,
	TNI_COUNT
};


//-------------------------------------------------------------------------------
// Structures
//-------------------------------------------------------------------------------

//Structure mirroring the animation data from Tiled
//Additionally store RECT information locally to avoid map navigation requirement (SPEED TESTING REQUIRED)
struct TileAnim
{
	RECT m_FrameRect = { 0, 0, 0, 0 };
	unsigned short m_FrameID = 0;
	float m_Speed = 3;
};

///////////////////////////////
/// Tiled Loaded Properties ///
///////////////////////////////

struct TL_Properties_Type1
{
	//General Tile Details
	std::string m_Name = "PH";
	bool m_Collideable = false;
	int m_CollisionLayerID = 0;
};


/////////////////////////////
/// Map Stored Properties ///
/////////////////////////////

struct TMM_Properties_Type1
{
	TL_Properties_Type1 m_LoadedProperties;
	//Animation Data (If tile has the data), this is loaded into "MT_Animator"
	std::vector<TileAnim> m_Animations;
};


///////////////////////////
/// Map Tile Properties ///
///////////////////////////

struct MT_Properties_Type1
{
	std::string m_Name = "PH";
	bool m_Collideable = false;
	int m_ColliosnLayerID = 0;
	short m_OccupiedTeamID = -1;
	bool m_IsOccupied = false;
};



