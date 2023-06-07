#pragma once
#include "D3DUtils.h"

class EM_Sprite
{
public:

	////////////
	/// Data ///
	////////////

	//Data used in draw call
	struct DrawData
	{
		DrawData()
			:m_SRVIndex(0),
			m_TexSize(1920, 1080),
			m_Pos(0, 0),
			m_FrameRect({ 0, 0, 1920, 1080 }),
			M_Colour({ 1,1,1,1 }),
			m_Rotation(0),
			m_Origin(0, 0),
			m_Scale(1, 1),
			m_SprEffect(DirectX::SpriteEffects::SpriteEffects_None),
			m_LayerDepth(1)
		{ }

		//Reordered for packing purposes
		DirectX::XMVECTOR M_Colour;
		RECT m_FrameRect;
		DirectX::XMUINT2 m_TexSize;
		DirectX::XMFLOAT2 m_Origin;
		DirectX::XMFLOAT2 m_Scale;
		DirectX::XMFLOAT2 m_Pos;
		size_t m_SRVIndex;
		float m_Rotation;
		float m_LayerDepth;
		DirectX::SpriteEffects m_SprEffect;

		//Support Overload
		DrawData& operator=(const DrawData& rSide)
		{
			m_SRVIndex = rSide.m_SRVIndex;
			m_TexSize = rSide.m_TexSize;
			m_Pos = rSide.m_Pos;
			m_FrameRect = rSide.m_FrameRect;
			M_Colour = rSide.M_Colour;
			m_Rotation = rSide.m_Rotation;
			m_Origin = rSide.m_Origin;
			m_Scale = rSide.m_Scale;
			m_SprEffect = rSide.m_SprEffect;
			m_LayerDepth = rSide.m_LayerDepth;
			return *this;
		}
	};

	////////////////////
	/// Constructors ///
	////////////////////

	EM_Sprite() {}
	EM_Sprite(std::shared_ptr<SpriteTexture> texPtr);
	~EM_Sprite() {}

	//////////////////
	/// Operations ///
	//////////////////

	virtual void Draw(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap);
	/*
		Returns the sprites to a safe default state (middle of window, texture default size,
		no scaling or rotations etc). This excludes the SRV Index.
	*/
	void DefaultSprite();

	
	///////////
	/// Set ///
	///////////

	//
	//Sprite Data Setters
	//
	void SetSRVIndex(size_t newIndex)				 { m_DrawData.m_SRVIndex = newIndex; }
	void SetTexSize(DirectX::XMUINT2 newTexSize)	 { m_DrawData.m_TexSize = newTexSize; }
	void SetTexSize(uint32_t x, uint32_t y)			 { m_DrawData.m_TexSize.x = x; m_DrawData.m_TexSize.y = y; }
	void SetPosition(DirectX::XMFLOAT2 newPos)		 { m_DrawData.m_Pos = newPos; }
	void SetPosition(float x, float y)				 { m_DrawData.m_Pos.x = x; m_DrawData.m_Pos.y = y; }
	void SetPositionX(float x)						 { m_DrawData.m_Pos.x = x; }
	void SetPositionY(float y)						 { m_DrawData.m_Pos.y = y; }
	void SetRect(RECT& newRect)						 { m_DrawData.m_FrameRect = newRect; }
	void SetColour(DirectX::XMVECTOR newColour)		 { m_DrawData.M_Colour = newColour; }
	void SetRotation(float newRotation)				 { m_DrawData.m_Rotation = newRotation; }
	void SetOrigin(DirectX::XMFLOAT2 newOrigin)		 { m_DrawData.m_Origin = newOrigin; }
	void SetOrigin(float x, float y)				 { m_DrawData.m_Origin.x = x; m_DrawData.m_Origin.y = y; }
	void SetScale(DirectX::XMFLOAT2 newScale)		 { m_DrawData.m_Scale = newScale; }
	void SetScale(float x, float y)					 { m_DrawData.m_Scale.x = x; m_DrawData.m_Scale.y = y; }
	void SetEffect(DirectX::SpriteEffects newEffect) { m_DrawData.m_SprEffect = newEffect; }
	void SetLayerDepth(float newDepth)				 { m_DrawData.m_LayerDepth = newDepth; }

	//Set Texture via Pointer (OPTIONAL: Set flag to set new TextureSize & Rect from data)
	void SetTexture(std::shared_ptr<SpriteTexture> tex, bool setDrawValues = false);
	//Set Texture via name through Manager (OPTIONAL: Set flag to set new TextureSize & Rect from data)
	void SetTexture(std::string texName, bool setDrawValues = false);

	//Set frame by index using texture data pointer (can be called by animator module)
	void SetFrame(int index);

	void AddToRotation(float incValue)			     { m_DrawData.m_Rotation += incValue; }

	//Add to existing position value instead of replacing
	void AddToPosition(DirectX::XMFLOAT2& addPos);
	void AddToPosition(float addX, float addY);
	void AddToPositionX(float addX)					 { m_DrawData.m_Pos.x += addX; }
	void AddToPositionY(float addY)					 { m_DrawData.m_Pos.y += addY; }


	//Set all values using draw data
	void SetDrawData(DrawData& newData) { m_DrawData = newData; }
	//Set all draw values at once
	void SetAllDrawValues(
		int srvIndex, DirectX::XMUINT2& textureSize, DirectX::XMFLOAT2& pos,
		RECT& rect,	DirectX::XMVECTOR& colour, float rotation,
		DirectX::XMFLOAT2& origin, DirectX::XMFLOAT2& scale,
		DirectX::SpriteEffects effect, float layerDepth
	);

	//Set origin any corner, side or center
	void SetOriginToPoint(JustificationPosition posID);
	//Set origin to center
	void SetOriginToCenter();


	///////////
	/// Get ///
	///////////

	//
	//Sprite Data Getters
	//
	size_t GetSRVIndex()										{ return m_DrawData.m_SRVIndex; }
	const DirectX::XMUINT2& GetTexSize()						{ return m_DrawData.m_TexSize; }
	const DirectX::XMFLOAT2& GetPosition()						{ return m_DrawData.m_Pos; }
	const RECT& GetRect() const									{ return m_DrawData.m_FrameRect; }
	const DirectX::XMVECTOR& GetColour()						{ return m_DrawData.M_Colour; }
	float GetRotation()											{ return m_DrawData.m_Rotation; }
	const DirectX::XMFLOAT2& GetOrigin()						{ return m_DrawData.m_Origin; }
	const DirectX::XMFLOAT2& GetScale()							{ return m_DrawData.m_Scale; }
	DirectX::SpriteEffects& GetEffect()							{ return m_DrawData.m_SprEffect; }
	float GetLayerDepth()										{ return m_DrawData.m_LayerDepth; }
	const std::shared_ptr<SpriteTexture>& GetTexturePtr() const { return m_Texture; }

	//Returns actual rect size relative to the sprites frame, not the spritesheet
	float GetActualRectX()										{ return static_cast<float>(m_DrawData.m_FrameRect.right - m_DrawData.m_FrameRect.left); }
	float GetActualRectY()										{ return static_cast<float>(m_DrawData.m_FrameRect.bottom - m_DrawData.m_FrameRect.top); }

	//Get actual rect size, adjusted for scale
	float GetScaleAdjustedRectX()								{ return GetActualRectX() * m_DrawData.m_Scale.x; }
	float GetScaleAdjustedRectY()								{ return GetActualRectY() * m_DrawData.m_Scale.y; }

	//Returns vec4 representing the onscreen rect size and position (ideal for rect collision)
	DirectX::SimpleMath::Vector4 GetRectCollision();

protected:

	//Drawing Data
	DrawData m_DrawData;
	//Pointer to actively used pointer
	std::shared_ptr<SpriteTexture> m_Texture;

};