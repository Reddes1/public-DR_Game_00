#include "SM_TilemapManager.h"
#include "Tiled_TilemapUtils.h"
#include "Game.h"		//Manager Access

using namespace rapidjson;

SM_TilemapManager::SM_TilemapManager()
{
	Init();
}

SM_TilemapManager::~SM_TilemapManager()
{
	Release();
}

void SM_TilemapManager::LoadMap(LoaderTypes loader, std::string& levelFP, std::string& tileFP, std::string& tilesetName, bool clearCurrent)
{
	//Load using specified loader, passing fp and name
	switch (loader)
	{
	case LoaderTypes::MAP_LOADER_00:
		break;
	case LoaderTypes::MAP_LOADER_01:
		MapLoader(levelFP, tileFP, tilesetName, clearCurrent);
		break;
	}

}

void SM_TilemapManager::Init()
{
	m_Maps.reserve(4);
}

void SM_TilemapManager::CalculateCullingRect()
{
	m_Cull = {
		-150.f - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().x,
		-150.f - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().y,
		(1920.f + 150.f) - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().x,
		(1080.f + 150.f) - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().y
	};
}

void SM_TilemapManager::Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap)
{
	//Update culling rect
	CalculateCullingRect();

	for (auto& a : m_Maps[m_ActiveMap]) 
	{
		//Skip/Cull tiles based on distance from the player
		auto& pos = a->GetSprite().GetPosition();
		if(!(pos.x < m_Cull.x || pos.x > m_Cull.z || pos.y < m_Cull.y || pos.y > m_Cull.w))
			a->Render(sb, heap);
	}		
}

void SM_TilemapManager::Update(float dTime)
{
	for (auto& a : m_Maps[m_ActiveMap])
	{
		a->Update(dTime);
	}
}

void SM_TilemapManager::Release()
{
	for (unsigned i(0); i < m_Maps.size(); ++i)
	{
		for (unsigned j(0); j < m_Maps[i].size(); ++j)
		{
			delete m_Maps[i][j];
			m_Maps[i][j] = nullptr;
		}
	}
}

void SM_TilemapManager::MapLoader(std::string& levelFP, std::string& tilesetFP,std::string& tilesetName, bool clearCurrent)
{
	//Get Game
	Game* game = Game::GetGame();
	
	//Parse the json doc
	rapidjson::Document levelDoc;
	rapidjson::Document tileDoc;
	ParseNewJSONDocument(levelDoc, levelFP);
	ParseNewJSONDocument(tileDoc, tilesetFP);

	//Store some important map information
	int mapRows = levelDoc["height"].GetInt();
	int mapCols = levelDoc["width"].GetInt();
	int tileWidth = levelDoc["tilewidth"].GetInt();
	int tileHeight = levelDoc["tileheight"].GetInt();

	if (clearCurrent)
		m_TileMap.clear();

	//Reserve vector space
	m_TileMap.reserve(mapRows * mapCols);

	//Get the texture for the tiles
	auto tilesetTexture = game->GetTextureManager().GetSpriteTextureData(
		std::string(levelDoc["tilesets"][0]["name"].GetString()));

	//A vector to store the RECT data for each of the tiles
	std::vector<RECT> tileFrames;

	//Generate the RECT frame data
	GenerateFrameData(tileDoc,0,tileFrames);

	//Counter for the data sub-arrays
	int count = 0;
	//store the number of Tile Layers
	unsigned layerCount = levelDoc["layers"].Size() - 1;

	//start building the map, (layer by layer, L->R, T->B)
	for (unsigned i(0); i < layerCount; ++i)
	{
		//Reset count to equal 0
		count = 0;

		for (int y(0); y < mapRows; ++y)
		{
			for (int x(0); x < mapCols; ++x)
			{
				//Get the ID of the tile that makes up this spot in the map
				unsigned int layerID = levelDoc["layers"][i]["data"][count].GetUint();

				if (layerID != 0)
				{
					--layerID;
					//Create new tile
					MT_Type2* newTile = new MT_Type2();

					//Set all of the necessary properties for the tile (Texture, RECT, Position, Origin and LayerDepth)
					newTile->GetSprite().SetTexture(tilesetTexture);
					newTile->GetSprite().SetRect(tileFrames[layerID]);
					newTile->GetSprite().SetPosition(
					static_cast<float>((tileWidth * x) + tileWidth * 0.5),
					static_cast<float>((tileWidth * y) + tileHeight * 0.5));

					newTile->GetSprite().SetOriginToCenter();
					newTile->GetSprite().SetLayerDepth(DRAW_DEPTH_FOREGROUND_0);

					//push the new tile into the map
					m_TileMap.push_back(newTile);

				}
				//Increment the loop
				++count;
			}
		}
	}

	m_Maps.push_back(m_TileMap);
}
