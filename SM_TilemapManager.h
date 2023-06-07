#pragma once
#include "D3DUtils.h"
#include "RapidJSONLoaderUtils.h" //File Loading

#include "MT_Type2.h"
/*
	Sub-Manager that handles map loading, management and access for modes
*/

class SM_TilemapManager
{
public:

	////////////
	/// Data ///
	////////////

	//Store current relevant map details
	struct MapData
	{
		std::string m_MapName = "";
		DirectX::XMINT2 m_MapSize = { 0,0 };
		DirectX::XMINT2 m_TileSize = { 0,0 };
	};

	//////////////////
	/// Defintions ///
	//////////////////

	enum class LoaderTypes : unsigned short
	{
		MAP_LOADER_00,
		MAP_LOADER_01
	};

	////////////////////
	/// Constructors ///
	////////////////////

	SM_TilemapManager();
	~SM_TilemapManager();

	//////////////////
	/// Operations ///
	//////////////////

	//Loads a map using a configuration file, and the texture name of a already loaded tileset.
	//Define what type of loader to use (specific to the map being loaded).
	void LoadMap(LoaderTypes loader, std::string& levelFP, std::string& tileFP, std::string& tilesetName, bool clearCurrent = false);
	//Renders each individual tile
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	//Updates each individual tile
	void Update(float dTime);
	//Releases current map array
	void Release();


	///////////
	/// Get ///
	///////////

	//Get the data on the currently loaded map
	const MapData& GetMapData() { return m_MapData; }

	//Get direct look at map
	std::vector<MT_Type2*>& GetTileMap() { return m_Maps[m_ActiveMap]; }

	int GetActiveMapIndex() { return m_ActiveMap; }

	///////////
	/// Set ///
	///////////

	void SetActiveMap(unsigned index) { m_ActiveMap = index; }

private:

	//////////////////
	/// Operations ///
	//////////////////
	
	void Init();

	//Calculates new culling rect for frame
	void CalculateCullingRect();

	//
	//Map Loading
	//

	//Type One Functions
	void MapLoader(std::string& levelFP, std::string& tilesetFP, std::string& tilesetName, bool clearCurrent = false);

	////////////
	/// Data ///
	////////////
	
	//Map Container
	std::vector<MT_Type2*> m_TileMap;

	std::vector<std::vector<MT_Type2*>> m_Maps;
	//Currently active map data
	MapData m_MapData;

	int m_ActiveMap = 0;

	//Culling rect for saving on rendering tiles
	DirectX::SimpleMath::Vector4 m_Cull = { 0.f, 0.f, 0.f,0.f };
};
