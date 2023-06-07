#include "UIElementManager.h"		
#include "Game.h"					//Managers

#include <assert.h>

using namespace DirectX;
using namespace rapidjson;

bool UIElementManager::GetNonNavUIElementByTypeID(NonNavUI*& ui, int id)
{
	for (auto& u : m_NonNavUIElements)
	{
		//Search for free element
		if (u->GetUsageFlag() && u->GetID() == id)
		{
			//If this ptr already holds a menu, release it before changing the ptr
			if (ui)
				ReleaseUIElement(ui);
			//Flag that its in use and return true
			u->SetUsageFlag(false);
			ui = u.get();
			return true;
		}
	}

	//Nothing found, return false
	DBOUT("No UI Found! Review Usage/Creation Count!");
	return false;
}

bool UIElementManager::GetFixedIndexNavUIElementByTypeID(FixedIndexNavUI*& ui, int id)
{
	for (auto& u : m_FixedNavUIElements)
	{
		//Search for free element
		if (u->GetUsageFlag() && u->GetID() == id)
		{
			//If this ptr already holds a menu, release it before changing the ptr
			if (ui)
				ReleaseUIElement(ui);
			//Flag that its in use and return true
			u->SetUsageFlag(false);
			ui = u.get();
			return true;
		}
	}

	//Nothing found, return false
	DBOUT("No UI Found! Review Usage/Creation Count!");
	return false;
}

void UIElementManager::LoadUIElementsFromFile(std::string& fp)
{
	//Create and parse document
	Document doc;
	ParseNewJSONDocument(doc, fp);

	//Load into appropriate container if UI type array found
	if (doc.HasMember("Non-Navigational UI Elements"))
	{
		PopulateNonNavContainer(doc);
	}
	if (doc.HasMember("Fixed-Navigational UI Elements"))
	{
		PopulateFixedNavContainer(doc);
	}

	//Shrink containers for space optimization
	m_NonNavUIElements.shrink_to_fit();
	m_FixedNavUIElements.shrink_to_fit();
}

void UIElementManager::PopulateNonNavContainer(rapidjson::Document& doc)
{
	//Get Game for texture purposes
	Game* game = Game::GetGame();

	//Hold access to the main array here
	const Value& arr = doc["Non-Navigational UI Elements"].GetArray();
		
	//For each UI entry that needs creating
	for (unsigned int i(0); i < arr.GetArray().Size(); ++i)
	{
		//Assert Main Members
		assert(arr[i].HasMember("Unique Type ID"));
		assert(arr[i].HasMember("Creation Count"));
		assert(arr[i].HasMember("Name"));
		assert(arr[i].HasMember("Texture Filepath"));
		assert(arr[i].HasMember("Texture Access Name"));
		assert(arr[i].HasMember("Texture Frame ID"));
		assert(arr[i].HasMember("UI Pos X"));
		assert(arr[i].HasMember("UI Pos Y"));
		assert(arr[i].HasMember("Pos Adj X"));
		assert(arr[i].HasMember("Pos Adj Y"));
		assert(arr[i].HasMember("Scale Adj X"));
		assert(arr[i].HasMember("Scale Adj Y"));
		assert(arr[i].HasMember("UI Layer Depth"));
		assert(arr[i].HasMember("UI Strings"));
				
		//Create as many new entries in accordance with creation count
		for (int j(0); j < arr[i]["Creation Count"].GetInt(); ++j)
		{
			//Setup new UI entry
			auto newUI = std::make_shared<NonNavUI>();

			//Setup
			//Set ID & Name
			newUI->SetID(arr[i]["Unique Type ID"].GetInt());
			newUI->SetName(std::string(arr[i]["Name"].GetString()));

			//Texture & Frame
			newUI->GetSprite().SetTexture(game->GetTextureManager().GetSpriteTextureData(std::string(arr[i]["Texture Access Name"].GetString())));
			newUI->GetSprite().SetFrame(arr[i]["Texture Frame ID"].GetInt());

			//Main UI window position, position adjustment & layer depth
			newUI->GetSprite().SetPosition(arr[i]["UI Pos X"].GetFloat(), arr[i]["UI Pos Y"].GetFloat());
			newUI->SetPositionAdjustment(arr[i]["Pos Adj X"].GetFloat(), arr[i]["Pos Adj Y"].GetFloat());
			newUI->GetSprite().SetLayerDepth(arr[i]["UI Layer Depth"].GetFloat());

			//Set scale adjustment
			newUI->GetSprite().SetScale(arr[i]["Scale Adj X"].GetFloat(), arr[i]["Scale Adj Y"].GetFloat());

			//Hold 2nd array here for brevity in next section
			const Value& strArr = arr[i]["UI Strings"].GetArray();

			//For each entry in "UI Strings", fill out a new data container
			for (unsigned int k(0); k < strArr.GetArray().Size(); ++k)
			{
				//Assert string entry members
				assert(strArr[k].HasMember("String"));
				assert(strArr[k].HasMember("Font ID"));
				assert(strArr[k].HasMember("Text Justification ID"));
				assert(strArr[k].HasMember("Pos X"));
				assert(strArr[k].HasMember("Pos Y"));
				assert(strArr[k].HasMember("Rotation"));
				assert(strArr[k].HasMember("Scale"));
				assert(strArr[k].HasMember("Effect ID"));
				assert(strArr[k].HasMember("Layer Depth"));
				assert(strArr[k].HasMember("R"));
				assert(strArr[k].HasMember("G"));
				assert(strArr[k].HasMember("B"));
				assert(strArr[k].HasMember("A"));

				//Create new string data entry
				NonNavUI::Data newData;
				//Fill out details
				newData.m_OriginalString = strArr[k]["String"].GetString();
				newData.ResetString();
				newData.m_FontSF = game->GetPassData().m_Spritefonts[strArr[k]["Font ID"].GetInt()].get();
				newData.m_StringJustification = strArr[k]["Text Justification ID"].GetInt();
				newData.m_Position.x = strArr[k]["Pos X"].GetFloat();
				newData.m_Position.y = strArr[k]["Pos Y"].GetFloat();
				newData.m_Rotation = strArr[k]["Rotation"].GetFloat();
				newData.m_Scale = strArr[k]["Scale"].GetFloat();
				newData.m_Effect = static_cast<DirectX::SpriteEffects>(strArr[k]["Effect ID"].GetUint());
				newData.m_LayerDepth = strArr[k]["Layer Depth"].GetFloat();
				newData.m_Colour = XMVECTOR({
					strArr[k]["R"].GetFloat(),
					strArr[k]["G"].GetFloat(),
					strArr[k]["B"].GetFloat(),
					strArr[k]["A"].GetFloat()
					});

				//Store Data
				newUI->StoreNewData(newData);
			}

			//Finalise
			newUI->SetUsageFlag(true);
			newUI->SetEnabledState(true);

			//Store Entry
			m_NonNavUIElements.push_back(newUI);
		}
	}	
}

void UIElementManager::PopulateFixedNavContainer(rapidjson::Document& doc)
{
	//Get Game for texture purposes
	Game* game = Game::GetGame();

	//Hold access to the main array here
	const Value& arr = doc["Fixed-Navigational UI Elements"].GetArray();

	//For each UI entry that needs creating
	for (unsigned int i(0); i < arr.GetArray().Size(); ++i)
	{
		//Assert Main Members
		assert(arr[i].HasMember("Unique Type ID"));
		assert(arr[i].HasMember("Creation Count"));
		assert(arr[i].HasMember("Name"));
		assert(arr[i].HasMember("Texture Filepath"));
		assert(arr[i].HasMember("Texture Access Name"));
		assert(arr[i].HasMember("Texture Frame ID"));
		assert(arr[i].HasMember("Cursor Texture Filepath"));
		assert(arr[i].HasMember("Cursor Texture Access Name"));
		assert(arr[i].HasMember("Cursor Texture Frame ID"));
		assert(arr[i].HasMember("UI Pos X"));
		assert(arr[i].HasMember("UI Pos Y"));
		assert(arr[i].HasMember("Pos Adj X"));
		assert(arr[i].HasMember("Pos Adj Y"));
		assert(arr[i].HasMember("Scale Adj X"));
		assert(arr[i].HasMember("Scale Adj Y"));
		assert(arr[i].HasMember("UI Strings"));
		assert(arr[i].HasMember("Cursor Anchor Pos X"));
		assert(arr[i].HasMember("Cursor Anchor Pos Y"));
		assert(arr[i].HasMember("String Anchor Pos X"));
		assert(arr[i].HasMember("String Anchor Pos Y"));
		assert(arr[i].HasMember("Offset X"));
		assert(arr[i].HasMember("Offset Y"));
		assert(arr[i].HasMember("UI Layer Depth"));
		assert(arr[i].HasMember("Cursor Layer Depth"));
		assert(arr[i].HasMember("Index Max X"));
		assert(arr[i].HasMember("Index Max Y"));
		assert(arr[i].HasMember("Default Index X"));
		assert(arr[i].HasMember("Default Index Y"));

		//Create as many new entries in accordance with creation count
		for (int j(0); j < arr[i]["Creation Count"].GetInt(); ++j)
		{
			//Setup Entry
			auto newEntry = std::make_shared<FixedIndexNavUI>();

			//Setup
			//Set ID & Name
			newEntry->SetID(arr[i]["Unique Type ID"].GetInt());
			newEntry->SetName(std::string(arr[i]["Name"].GetString()));
			//Textures & Frames
			newEntry->GetMainSprite().SetTexture(game->GetTextureManager().GetSpriteTextureData(std::string(arr[i]["Texture Access Name"].GetString())));
			newEntry->GetCursorSprite().SetTexture(game->GetTextureManager().GetSpriteTextureData(std::string(arr[i]["Cursor Texture Access Name"].GetString())));
			newEntry->GetMainSprite().SetFrame(arr[i]["Texture Frame ID"].GetInt());
			newEntry->GetCursorSprite().SetFrame(arr[i]["Cursor Texture Frame ID"].GetInt());
			//Sprite positions
			newEntry->GetMainSprite().SetPosition(arr[i]["UI Pos X"].GetFloat(), arr[i]["UI Pos Y"].GetFloat());
			newEntry->GetCursorSprite().SetPosition(arr[i]["Cursor Anchor Pos X"].GetFloat(), arr[i]["Cursor Anchor Pos Y"].GetFloat());
			//Anchor positions
			newEntry->SetCursorAnchorPosition(arr[i]["Cursor Anchor Pos X"].GetFloat(), arr[i]["Cursor Anchor Pos Y"].GetFloat());
			newEntry->SetTextAnchorPosition(arr[i]["String Anchor Pos X"].GetFloat(), arr[i]["String Anchor Pos Y"].GetFloat());
			//Offsets
			newEntry->SetOffsets(arr[i]["Offset X"].GetFloat(), arr[i]["Offset Y"].GetFloat());
			//Indexes
			newEntry->SetMaxIndex(arr[i]["Index Max X"].GetInt(), arr[i]["Index Max Y"].GetInt());
			newEntry->SetDefaultIndex(arr[i]["Default Index X"].GetInt(), arr[i]["Default Index Y"].GetInt());
			//Layer depths
			newEntry->GetMainSprite().SetLayerDepth(arr[i]["UI Layer Depth"].GetFloat());
			newEntry->GetCursorSprite().SetLayerDepth(arr[i]["Cursor Layer Depth"].GetFloat());
			//Position adjustment
			newEntry->SetPositionAdjustment(arr[i]["Pos Adj X"].GetFloat(), arr[i]["Pos Adj Y"].GetFloat());
			//Scale adjustment
			newEntry->GetMainSprite().SetScale(arr[i]["Scale Adj X"].GetFloat(), arr[i]["Scale Adj Y"].GetFloat());

			//Hold 2nd array here for brevity in next section
			const Value& strArr = arr[i]["UI Strings"].GetArray();

			//For each entry in "UI Strings", fill out a new data container
			for (unsigned int k(0); k < strArr.GetArray().Size(); ++k)
			{
				//Assert string entry members
				assert(strArr[k].HasMember("String"));
				assert(strArr[k].HasMember("Font ID"));
				assert(strArr[k].HasMember("Text Justification ID"));
				assert(strArr[k].HasMember("Rotation"));
				assert(strArr[k].HasMember("Scale"));
				assert(strArr[k].HasMember("Effect ID"));
				assert(strArr[k].HasMember("Layer Depth"));
				assert(strArr[k].HasMember("R"));
				assert(strArr[k].HasMember("G"));
				assert(strArr[k].HasMember("B"));
				assert(strArr[k].HasMember("A"));

				FixedIndexNavUI::Data newData;

				//Fill out details
				newData.m_OriginalString = strArr[k]["String"].GetString();
				newData.ResetString();
				newData.m_FontSF = game->GetPassData().m_Spritefonts[strArr[k]["Font ID"].GetInt()].get();
				newData.m_StringJustification = strArr[k]["Text Justification ID"].GetInt();
				newData.m_Rotation = strArr[k]["Rotation"].GetFloat();
				newData.m_Scale = strArr[k]["Scale"].GetFloat();
				newData.m_Effect = static_cast<DirectX::SpriteEffects>(strArr[k]["Effect ID"].GetUint());
				newData.m_LayerDepth = strArr[k]["Layer Depth"].GetFloat();
				newData.m_Colour = XMVECTOR({
					strArr[k]["R"].GetFloat(),
					strArr[k]["G"].GetFloat(),
					strArr[k]["B"].GetFloat(),
					strArr[k]["A"].GetFloat()
					});

				//Store Data
				newEntry->StoreNewData(newData);
			}

			//Finalise
			newEntry->SetUsageFlag(true);
			newEntry->SetEnabledState(true);

			//Store Entry
			m_FixedNavUIElements.push_back(newEntry);
		}
	}
}

void UIElementManager::Init()
{
	//Reserve safe-ish about of space ahead of time
	m_NonNavUIElements.reserve(UI_RESERVE_SIZE);
	m_FixedNavUIElements.reserve(UI_RESERVE_SIZE);
}

void UIElementManager::Release()
{
	m_NonNavUIElements.clear();
	m_FixedNavUIElements.clear();
}


