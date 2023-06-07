#pragma once
#include "D3DUtils.h"
#include "RapidJSONLoaderUtils.h"	//File loading

//UI Types
#include "UITypes.h"
//UI Definitions
#include "UIDefinitions.h"

/*
	Specialised manager for holding both navigational and non-navigational UI elements.
	Stores common elements that would be used in more than one mode.
	Modes will get a hold of a pointer to the UI element (if its available), and will be responsible for managing its
	usage and then releasing it back to the manager when done.
	Any behaviours to operate the menu (i.e move the cursor in response to an input) should be wrapped up in the UI types.
	External behaviors to give menu indexes context should be defined externally.
*/
class UIElementManager
{
public:

	UIElementManager() { Init(); }
	~UIElementManager() { Release(); }

	///////////
	/// Get ///
	///////////

	//Get a free, non-nav UI element from appropriate container
	bool GetNonNavUIElementByTypeID(NonNavUI*& ui, int id);
	//Get a free, Fixed-Index Nav UI element from appropriate container
	bool GetFixedIndexNavUIElementByTypeID(FixedIndexNavUI*& ui, int id);

	//////////////////
	/// Operations ///
	//////////////////

	//Load UI Elements from file
	void LoadUIElementsFromFile(std::string& fp);
	//Clear all containers (only manually call if intending to reload containers for loading purposes)
	void Release();

private:

	//Load into NonNavUI container
	void PopulateNonNavContainer(rapidjson::Document& doc);
	//Load into FixedNavUI container
	void PopulateFixedNavContainer(rapidjson::Document& doc);

	//Init Containers
	void Init();

	//Containers for each UI type
	std::vector<std::shared_ptr<NonNavUI>> m_NonNavUIElements;
	std::vector<std::shared_ptr<FixedIndexNavUI>> m_FixedNavUIElements;

};

//Utilty function to release an element so that other mode can use (Only call with Known UI type with (SetUsageFlag call)
template<class T>
static inline void ReleaseUIElement(T& element)
{
	//safety check
	if (element)
	{
		element->SetUsageFlag(true);
		element = nullptr;
	}
}

