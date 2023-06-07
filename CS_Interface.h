#pragma once
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>

class CS_Interface
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	CS_Interface();
	CS_Interface(unsigned reserveCount);
	~CS_Interface() { }


	////////////////
	/// Virtuals ///
	////////////////

	//Dictates how to use the information flagged in process key
	virtual void UpdateInputs() = 0;
	//Process the key again all indexed keycodes
	virtual void UpdateProcessKey(char key) = 0;
	//Resets keycodes to defined defaults
	virtual void ResetKeysToDefault() = 0;
	//Define a loader for loading a scheme from file
	virtual void LoadControlSchemeFromFile(std::string& fp) {}


	//////////////////
	/// Operations ///
	//////////////////

	//Clears all indexes to false (should be called each frame, if required)
	void ClearActionsFlags() { std::fill(m_ActionPressed.begin(), m_ActionPressed.end(), 0); }
	//Maps a new keycode to a given index
	void MapKeyToIndex(unsigned short newKey, unsigned index);
	//Checks if a specific indexed keycode has been flagged this frame
	bool GetPressedStateAtIndex(unsigned index);
	//Returns a given keycode at index
	unsigned short GetMappedKeyAtIndex(unsigned index);
	//Gets the index that the given key matches in the mapped key container
	unsigned GetIndexOfMatchedKey(char key);

protected:

	//////////////////
	/// Operations ///
	//////////////////

	void Release();


	////////////
	/// Data ///
	////////////

	std::vector<unsigned short> m_MappedActionKeys;
	std::vector<bool> m_ActionPressed;


};