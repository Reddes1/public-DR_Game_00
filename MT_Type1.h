#pragma once
#include "D3DUtils.h"	//Types & Utils
#include "GameTimer.h"	//Game time

//Types
#include "EM_MultiSprite_Type1.h"
#include "EM_Coordinates.h"
#include "Tiled_Types.h"
#include "MT_Animator.h"

/*
	Primary game tile.
*/

class MT_Type1
{
public:

	////////////
	/// Data ///
	////////////

	//Support data for A*
	struct AStarData
	{
		//Sets F cost to sum of G+H
		void UpdateFCost() { m_FCost = m_GCost + m_HCost; }

		//Distance from origin
		float m_GCost = 0;
		//Distance to target
		float m_HCost = 0;
		//Sum of G and H
		float m_FCost = 0;
		//Parent tile for linking start and end point of path
		MT_Type1* m_ParentTile = nullptr;
	};


	////////////////////
	/// Constructors ///
	////////////////////

	MT_Type1() { Init(); }
	~MT_Type1() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////
	
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void Update(float dTime);

	//Enables the highlight/grid effect for dedicated sprite
	void EnableHighlight();
	//Enables the highlight/grid effect for dedicated sprite using passed index as basis
	void EnableHighlight(unsigned index);
	//Disables the effect sprite
	void DisableHighlight();


	///////////
	/// Get ///
	///////////

	EM_Coordinates& GetCoordinates() { return m_WorldCoordinates; }
	MT_Properties_Type1& GetProperties() { return m_Properties; }
	MT_Animator& GetAnimatorAtIndex(unsigned index);
	EM_Sprite& GetSpriteAtIndex(unsigned index);
	MT_Type1* GetNeighbourTileAtIndex(unsigned int index);
	AStarData& GetAStarData() { return m_AStarData; }

	bool GetEnabledStateAtIndex(unsigned int index);

	///////////
	/// Set ///
	///////////

	void SetNeighbourTileAtIndex(unsigned int index, MT_Type1* tile);
	void SetTileProperties(MT_Properties_Type1& props) { m_Properties = props; }
	void ExtractPropertiesFromMapTileProperties(TMM_Properties_Type1& mapProps);
	void SetEnabledAtIndex(unsigned index, bool enable);

private:

	//////////////////
	/// Operations ///
	//////////////////
	
	void Init();
	void Release();


	////////////
	/// Data ///
	////////////

	//We know how many sprite we may need to accomidate for, so use fixed arrays
	EM_Sprite m_Sprites[TileSpriteIndexes::TS_COUNT];
	//Tile Properties
	MT_Properties_Type1 m_Properties;
	//Specialised animator as the data for tile anims is different to loaded textures
	MT_Animator m_Animator[TileSpriteIndexes::TS_COUNT];
	//World Space Coordinates
	EM_Coordinates m_WorldCoordinates;
	//A* Data
	AStarData m_AStarData;
	//Keeps track of neighbouring tiles
	MT_Type1* m_NeighbourTiles[TNIndexes::TNI_COUNT];
	//Track what to draw/update
	bool m_Enabled[TileSpriteIndexes::TS_COUNT] = { false, false, false, false };

};