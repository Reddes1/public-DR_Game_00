#include "TilesetManager.h"

#include "RapidJSONLoaderUtils.h"
#include "Tiled_TilemapUtils.h"
#include "GeneralUtils.h"

using namespace rapidjson;

void TilesetManager::LoadNewTileMap(std::string& tilesFP)
{
	//Setup doc for tilemap file
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, tilesFP);

	//Asserts here

	//Assuming only one tileset here, so load the tiles in that one array (Expand later)
	const Value& tilesArray = doc["tilesets"][0]["tiles"];

	//We need to divide the map up into a number of different frames, and then relate them
	//to a TileID later down the line
	std::vector<RECT> frames;
	GenerateFrameData(doc, 0, frames);

	//Create new map
	std::unordered_map<int, TMM_Properties_Type1> newMap;
	
	//Loop through entire tiles array and store all the relevant data
	for (unsigned i(0); i < tilesArray.GetArray().Size(); ++i)
	{
		//Check to see if flagged to ignore (deadspace tiles, purposefully omited tiles etc)
		if (!tilesArray[i]["properties"][TPIndexes::TPI_IGNORE]["value"].GetBool())
		{
			//No skip flag, so fill out a map entry
			std::pair<int, TMM_Properties_Type1> newTile;

			//Load ID
			newTile.first = tilesArray[i]["id"].GetInt();
			
			//Load Data
			newTile.second.m_LoadedProperties.m_Name = tilesArray[i]["properties"][TPIndexes::TPI_NAME]["value"].GetString();
			newTile.second.m_LoadedProperties.m_Collideable = tilesArray[i]["properties"][TPIndexes::TPI_COLLIDEABLE]["value"].GetBool();
			newTile.second.m_LoadedProperties.m_CollisionLayerID = tilesArray[i]["properties"][TPIndexes::TPI_COLLISION_LAYER_ID]["value"].GetInt();

			//Check to see if there is an animator component to the tile
			if (tilesArray[i].HasMember("animation"))
			{
				newTile.second.m_Animations.reserve(tilesArray[i]["animation"].Size());
				for (unsigned j(0); j < tilesArray[i]["animation"].Size(); ++j)
				{
					TileAnim newAnim;
					newAnim.m_FrameID = tilesArray[i]["animation"][j]["tileid"].GetUint();
					//Need to convert the anim duration from ms to fps
					newAnim.m_Speed = 1000.f / tilesArray[i]["animation"][j]["duration"].GetFloat();
					//Store frame
					newAnim.m_FrameRect = frames[newAnim.m_FrameID];
					newTile.second.m_Animations.push_back(newAnim);
				}
			}
			//We still need to store at least one frame of data, so just create one frame
			else
			{
				newTile.second.m_Animations.reserve(1);
				TileAnim newAnim;
				newAnim.m_FrameID = tilesArray[i]["id"].GetUint();
				newAnim.m_FrameRect = frames[i];
				newTile.second.m_Animations.push_back(newAnim);
			}

			//Insert
			newMap.insert(newTile);
		}
	}
	//Clear frames, dont need them anymore
	frames.clear();

	//Store the map
	m_TileMaps[doc["tilesets"][0]["name"].GetString()] = std::move(newMap);
}

std::unordered_map<int, TMM_Properties_Type1>* TilesetManager::GetTileMapByName(std::string& keyName)
{
	TileMapMap::iterator it = m_TileMaps.find(keyName);
	if (it != m_TileMaps.end())
		return &it->second;
	return nullptr;
}

void TilesetManager::Release()
{
	FreeSTLContainer(m_TileMaps);
}
