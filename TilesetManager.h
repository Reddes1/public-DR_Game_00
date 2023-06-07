#pragma once

#include "MT_Type1.h"
#include "MT_Type2.h"
/*
	A specialised asset manager that takes a Tiled formatted json file to load and store tile properties data in relation 
	to a specific tile spritesheet/tileset. This information is stored in a multi-dimentional map to be able to access
	specific tilesets as needed. Designed to be used in tandem with "SM_Tilemap", which can access this information to 
	build functional game maps.
*/
class TilesetManager
{
public:

	////////////
	/// Data ///
	////////////

	////////////////////
	/// Constructors ///
	////////////////////

	TilesetManager() {}
	~TilesetManager() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Loads tile data from file and stores it under the name give in the file
	void LoadNewTileMap(std::string& tilesFP);

	///////////
	/// Get ///
	///////////

	std::unordered_map<int, TMM_Properties_Type1>* GetTileMapByName(std::string& keyName);

	///////////
	/// Set ///
	///////////


private:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();

	////////////
	/// Data ///
	////////////

	typedef std::unordered_map<std::string, std::unordered_map<int, TMM_Properties_Type1>> TileMapMap;
	//Store loaded tilemaps and their data here
	TileMapMap m_TileMaps;
};