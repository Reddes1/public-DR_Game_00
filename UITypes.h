#pragma once
#include "EM_Sprite.h"

//Definitions for UI types (This relates the the JSON loading information)

enum UIElementTypes
{
	NON_NAVIGATION_UI_00,
	FIXED_NAVIGATION_UI_00,
	UI_TYPE_COUNT
};

/*
	Non-Navigatiable UI element, where the user has no direct interaction with the element.
	Good for simple tooltips, pop-ups, providing information etc.
*/
class NonNavUI
{
public:

	////////////
	/// Data ///
	////////////

	struct Data
	{
		~Data() { m_FontSF = nullptr; }

		std::string m_OriginalString;
		std::string m_DrawString;
		DirectX::XMVECTOR m_Colour;
		DirectX::XMFLOAT2 m_Position;
		DirectX::SpriteFont* m_FontSF;
		float m_Rotation = 0.f;
		float m_Scale = 1.0f;
		DirectX::SpriteEffects m_Effect = DirectX::SpriteEffects::SpriteEffects_None;
		float m_LayerDepth = 1.0f;
		short m_StringJustification = 0;

		void ResetString() { m_DrawString = m_OriginalString; }
	};

	////////////////////
	/// Constructors ///
	////////////////////

	NonNavUI() {}
	~NonNavUI() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Draw UI and strings
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void RenderUIOnly(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void RenderTextOnly(DirectX::SpriteBatch& sb);

	//Reserve container space
	void ReserveSpace(int count)											 { m_Data.reserve(count); }

	//Only call in initialisation phase (during JSON load)
	void StoreNewData(Data& newData)										 { m_Data.push_back(newData); }

	///////////
	/// Get ///
	///////////

	EM_Sprite& GetSprite()													 { return m_Sprite; }
	int GetID()																 { return m_ID; }
	bool GetUsageFlag()														 { return m_FreeForUse; }
	bool GetEnabledState()													 { return m_Enabled; }
	const std::string& GetName()											 { return m_Name; }

	const std::string& GetDefaultStringAtIndex(unsigned int index)			 { return m_Data[index].m_OriginalString; }
	const std::string& GetDrawStringAtIndex(unsigned int index)				 { return m_Data[index].m_DrawString; }
	const DirectX::XMVECTOR& GetColourAtIndex(unsigned int index)			 { return m_Data[index].m_Colour; }
	const DirectX::SpriteFont* GetFontAtIndex(unsigned int index)			 { return m_Data[index].m_FontSF; }
	const short GetJustificationAtIndex(unsigned int index)					 { return m_Data[index].m_StringJustification; }
	float GetLayerDepthAtIndex(unsigned int index)							 { return m_Data[index].m_LayerDepth; }
	float GetRotationAtIndex(unsigned int index)							 { return m_Data[index].m_Rotation; }
	float GetScaleAtIndex(unsigned int index)								 { return m_Data[index].m_Scale; }
	DirectX::SpriteEffects GetEffectAtIndex(unsigned int index)				 { return m_Data[index].m_Effect; }
	size_t GetStringCount()													 { return m_Data.size(); }
																			 
	///////////																 
	/// Set ///																 
	///////////																 
																			 
	void SetID(int id)														 { m_ID = id; }
	void SetUsageFlag(bool IsFree)											 { m_FreeForUse = IsFree; }
	void SetEnabledState(bool state)										 { m_Enabled = state; };
	void SetName(std::string& name)											 { m_Name = name; }
																			 
	void SetColourAtIndex(DirectX::XMVECTOR& col, unsigned int index)		 { m_Data[index].m_Colour = col; }
	void SetStringAtIndex(std::string& str, unsigned int index)				 { m_Data[index].m_DrawString = str; }
	void SetFontAtIndex(DirectX::SpriteFont* font, unsigned int index)		 { m_Data[index].m_FontSF = font; }
	void SetJustificationAtIndex(short justi, unsigned int index)			 { m_Data[index].m_StringJustification = justi; }
	void SetLayerDepthAtIndex(short depth, unsigned int index)				 { m_Data[index].m_LayerDepth = depth; }
	void SetPositionAtIndex(DirectX::XMFLOAT2& pos, unsigned int index)		 { m_Data[index].m_Position = pos; }
	void SetRotationAtIndex(float rot, unsigned int index)					 { m_Data[index].m_Rotation = rot; }
	void SetScaleAtIndex(float scale, unsigned int index)					 { m_Data[index].m_Scale; }
	void SetEffectAtIndex(DirectX::SpriteEffects effect, unsigned int index) { m_Data[index].m_Effect = effect; }

	//Set string to its original form
	void ResetStringAtIndex(unsigned int index)								 { m_Data[index].ResetString(); }
	//Reset all strings to default
	void ResetAllStrings();

	//Appends string to existing string at index
	void AppendStringAtIndex(std::string& str, unsigned int index);
	void AppendStringAtIndex(std::stringstream& str, unsigned int index);

	void SetPositionAdjustment(DirectX::XMFLOAT2& adj)						 { m_PositionAdj = adj; }
	void SetPositionAdjustment(float x, float y)							 { m_PositionAdj.x = x; m_PositionAdj.y = y; }


private:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();


	/////////////////
	/// Variables ///
	/////////////////

	//String Entries
	std::vector<Data> m_Data;

	//Main Sprite
	EM_Sprite m_Sprite;
	
	//UI Name
	std::string m_Name;
	//Global adjustment value to reposition the whole UI
	DirectX::XMFLOAT2 m_PositionAdj = {0.f, 0.f};
	//ID Describing UI
	int m_ID = -1;
	//Flag if this UI is owned by mode already
	bool m_FreeForUse = true;
	//Flag if the UI enabled (visable/interactable)
	bool m_Enabled = true;
};

/*
	Uses mathematics to position text and cursor at fixed spacing.
	Very good for X by X grid based UIs like menus, inventories etc
	Will draw out X strings over X/Y rows and columns as required.
*/
class FixedIndexNavUI
{
public:

	////////////
	/// Data ///
	////////////

	struct Data
	{
		~Data() { m_FontSF = nullptr; }

		std::string m_OriginalString;
		std::string m_DrawString;
		DirectX::XMVECTOR m_Colour;
		DirectX::SpriteFont* m_FontSF;
		float m_Rotation = 0.f;
		float m_Scale = 1.0f;
		DirectX::SpriteEffects m_Effect = DirectX::SpriteEffects::SpriteEffects_None;
		float m_LayerDepth = 1.0f;
		short m_StringJustification = 0;

		void ResetString() { m_DrawString = m_OriginalString; }
	};

	////////////////////
	/// Constructors ///
	////////////////////

	FixedIndexNavUI() {}
	~FixedIndexNavUI() { Release(); }

	//////////////////
	/// Operations ///
	//////////////////

	//Draw UI and strings
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void RenderUIOnly(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);
	void RenderTextOnly(DirectX::SpriteBatch& sb);

	//Reserve container space
	void ReserveSpace(int count)										     { m_Data.reserve(count); }

	//Only call in initialisation phase (during JSON load)	
	void StoreNewData(Data& newData)										 { m_Data.push_back(newData); }

	//Cursor Operations
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorLeft();
	void MoveCursorRight();


	///////////
	/// Get ///
	///////////

	EM_Sprite& GetMainSprite()												 { return m_MainSprite; }
	int GetID()																 { return m_ID; }
	bool GetUsageFlag()														 { return m_FreeForUse; }
	bool GetEnabledState()													 { return m_Enabled; }
	const std::string& GetName()											 { return m_Name; }

	EM_Sprite& GetCursorSprite()											 { return m_CursorSprite; }
	const DirectX::XMINT2& GetMenuIndex()								     { return m_ActiveIndex; }
	const DirectX::XMINT2& GetMenuIndexMax()							     { return m_MaxIndex; }
	const DirectX::XMFLOAT2& GetOffsets()								     { return m_Offsets; }
	const DirectX::XMFLOAT2& GetStartPosition()							     { return m_CursorAnchorPos; }
																		     
	const std::string& GetDefaultStringAtIndex(unsigned int index)		     { return m_Data[index].m_OriginalString; }
	const std::string& GetDrawStringAtIndex(unsigned int index)			     { return m_Data[index].m_DrawString; }
	const DirectX::XMVECTOR& GetColourAtIndex(unsigned int index)		     { return m_Data[index].m_Colour; }
	const DirectX::SpriteFont* GetFontAtIndex(unsigned int index)		     { return m_Data[index].m_FontSF; }
	short GetJustificationAtIndex(unsigned int index)						 { return m_Data[index].m_StringJustification; }
	float GetLayerDepthAtIndex(unsigned int index)							 { return m_Data[index].m_LayerDepth; }
	float GetRotationAtIndex(unsigned int index)							 { return m_Data[index].m_Rotation; }
	float GetScaleAtIndex(unsigned int index)								 { return m_Data[index].m_Scale; }
	DirectX::SpriteEffects GetEffectAtIndex(unsigned int index)				 { return m_Data[index].m_Effect; }
	size_t GetStringCount()													 { return m_Data.size(); }
																		     
	///////////															     
	/// Set ///															     
	///////////															     
		
	void SetID(int id)														 { m_ID = id; }
	void SetUsageFlag(bool IsFree)											 { m_FreeForUse = IsFree; }
	void SetEnabledState(bool state)										 { m_Enabled = state; };
	void SetName(std::string& name)											 { m_Name = name; }

	void SetCursorAnchorPosition(DirectX::XMFLOAT2& initPos)				 { m_CursorAnchorPos = initPos; }
	void SetCursorAnchorPosition(float xAnchor, float yAnchor)				 { m_CursorAnchorPos.x = xAnchor; m_CursorAnchorPos.y = yAnchor; }
	void SetIndex(DirectX::XMINT2& indexes)								     { m_ActiveIndex = indexes; }
	void SetIndex(int x, int y)											     { m_ActiveIndex.x = x; m_ActiveIndex.y = y; }
	void SetMaxIndex(DirectX::XMINT2& maxIndex)							     { m_MaxIndex = maxIndex; }
	void SetMaxIndex(int xMax, int yMax)								     { m_MaxIndex.x = xMax; m_MaxIndex.y = yMax; }
	void SetDefaultIndex(DirectX::XMINT2& defIndex)						     { m_DefaultIndex = defIndex; }
	void SetDefaultIndex(int xDef, int yDef)							     { m_DefaultIndex.x = xDef; m_DefaultIndex.y = yDef; }
	void SetOffsets(DirectX::XMFLOAT2& offsets)								 { m_Offsets = offsets; }
	void SetOffsets(float xOffset, float yOffset)							 { m_Offsets.x = xOffset; m_Offsets.y = yOffset; }
	void SetTextAnchorPosition(DirectX::XMFLOAT2& initPos)				     { m_TextAnchorPos = initPos; }
	void SetTextAnchorPosition(float xAnchor, float yAnchor)			     { m_TextAnchorPos.x = xAnchor; m_TextAnchorPos.y = yAnchor; }

	void SetColourAtIndex(DirectX::XMVECTOR& col, unsigned int index)        { m_Data[index].m_Colour = col; }
	void SetStringAtIndex(std::string& str, unsigned int index)			     { m_Data[index].m_DrawString = str; }
	void SetFontAtIndex(DirectX::SpriteFont* font, unsigned int index)	     { m_Data[index].m_FontSF = font; }
	void SetJustificationAtIndex(short justi, unsigned int index)		     { m_Data[index].m_StringJustification = justi; }
	void SetLayerDepthAtIndex(short depth, unsigned int index)				 { m_Data[index].m_LayerDepth = depth; }
	void SetRotationAtIndex(float rot, unsigned int index)					 { m_Data[index].m_Rotation = rot; }
	void SetScaleAtIndex(float scale, unsigned int index)					 { m_Data[index].m_Scale; }
	void SetEffectAtIndex(DirectX::SpriteEffects effect, unsigned int index) { m_Data[index].m_Effect = effect; }

	//Set cursor to default index, and position
	void SetCursorToDefault();
	//Set string to its original form
	void ResetStringAtIndex(unsigned int index)								 { m_Data[index].ResetString(); }
	//Reset all strings to default
	void ResetAllStrings();

	//Appends string to existing string at index
	void AppendStringAtIndex(std::string& str, unsigned int index);
	void AppendStringAtIndex(std::stringstream& str, unsigned int index);


	void SetPositionAdjustment(DirectX::XMFLOAT2& adj)						 { m_PositionAdj = adj; }
	void SetPositionAdjustment(float x, float y)							 { m_PositionAdj.x = x; m_PositionAdj.y = y; }



private:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();


	/////////////////
	/// Variables ///
	/////////////////
	
	//String Entries
	std::vector<Data> m_Data;

	//Main and Cursor Sprites
	EM_Sprite m_MainSprite;
	EM_Sprite m_CursorSprite;

	//Active UI Index
	DirectX::XMINT2 m_ActiveIndex = { 0, 0 };
	//Default Index Value
	DirectX::XMINT2 m_DefaultIndex = { 0, 0 };
	//Max index value
	DirectX::XMINT2 m_MaxIndex = { 0, 0 };

	//Cursors anchoring position (its position at index [0,0])
	DirectX::XMFLOAT2 m_CursorAnchorPos = { 0, 0 };
	//Starting position for text
	DirectX::XMFLOAT2 m_TextAnchorPos = { 0, 0 };
	//Offset distance between entries
	DirectX::XMFLOAT2 m_Offsets = { 0, 0 };

	//Global adjustment value to reposition the whole UI
	DirectX::XMFLOAT2 m_PositionAdj = { 0.f, 0.f };

	//UI Name
	std::string m_Name;
	//ID Describing UI
	int m_ID = -1;
	//Flag if this UI is owned by mode already
	bool m_FreeForUse = true;
	//Flag if the UI enabled (visable/interactable)
	bool m_Enabled = true;

};