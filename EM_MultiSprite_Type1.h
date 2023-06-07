#pragma once

#include "EM_Sprite.h"

/*
	Specialised class handing multiple draws using the set of data available to the sprite.
	Uses shared data from the main sprite to access the same texture and draw seperate frames at the same position.
	Useful for overlaying tiles with opaque and transparent overlays, and serves as a lightweight solution 
	to vertical, multi-sprite structures.
	This type uses the same texture and frame data. Use "EM_MultiSprite_Type2" for seperate texture usage,
	or "EM_MultiSprite_Type3" for full sprite + data usage.
*/

class EM_MultiSprite_Type1 : public EM_Sprite
{
public:

	////////////
	/// Data ///
	////////////

	struct SubSpriteData
	{
		RECT m_FrameRect = { 0, 0, 0, 0 };
		bool m_DrawEnabled = true;
	};

	////////////////////
	/// Constructors ///
	////////////////////
	
	EM_MultiSprite_Type1() {}
	EM_MultiSprite_Type1(std::shared_ptr<SpriteTexture> sprTex);
	EM_MultiSprite_Type1(std::shared_ptr<SpriteTexture> sprTex, unsigned int subSprCount);

	/////////////////
	/// Overrides ///
	/////////////////

	void Draw(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap) override;

	//////////////////
	/// Operations ///
	//////////////////

	//Call this ONLY ONCE!! Create a number of sub-sprites for use
	void CreateSubSprites(unsigned count);

	///////////
	/// Get ///
	///////////



	///////////
	/// Set ///
	///////////

	void SetSubSpriteDrawStateAtIndex(int index, bool enabledDraw) { m_SubSprites[index].m_DrawEnabled = enabledDraw; }
	void SetSubSpriteFrameAtIndex(int index, int frameIndex);
	void SetSubSpriteRectAtIndex(int index, RECT& newRect);
	//Defaults the subsprite the main sprites frame rect
	void DefaultSubSpriteAtIndex(int index);

private:
	//Store subsprite data here
	std::vector<SubSpriteData> m_SubSprites;
};