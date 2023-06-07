#pragma once

#include <unordered_map>
#include "GameTypes.h"				//Data Types
#include "RapidJSONLoaderUtils.h"	//File type loading

/*
	Central storage class for entity + weapon data types
*/
class SM_DataManager
{
public:

	//////////////////
	/// Defintions ///
	//////////////////

	typedef std::unordered_map<unsigned, PlayerCompositeData> CharacterEntityMap;
	typedef std::unordered_map<unsigned, WeaponCompositeData> WeaponEntityMap;
	typedef std::unordered_map<unsigned, EnemyCompositeData> EnemyEntityMap;
	typedef std::unordered_map<unsigned, AccessoryCompositeData> AccessoryEntityMap;
	typedef std::unordered_map<unsigned, SpawnerData> SpawnerPatternMap;
	typedef std::unordered_map<unsigned, ObjectCol> ObjectCollisionMap;

	////////////
	/// Data ///
	////////////



	////////////////////
	/// Constructors ///
	////////////////////

	SM_DataManager() { Init(); }
	~SM_DataManager() { Release(); }


	//////////////////
	/// Operations ///
	//////////////////

	//Loads character data from JSON file, with optional bool to clear current container (false by default)
	bool LoadCharacterData(std::string& fp, bool clearCurrentData = false);
	//Loads weapon data from JSON file, with optional bool to clear current container (false by default)
	bool LoadWeaponData(std::string& fp, bool clearCurrentData = false);
	//Loads enemy entity data from JSON file, with optional bool to clear current container (false by default)
	bool LoadEnemyData(std::string& fp, bool clearCurrentData = false);
	//Loads accessory data from JSON file, with optional bool to clear current container (false by default)
	bool LoadAccessoryData(std::string& fp, bool clearCurrentData = false);
	//Loads spawning patterns from JSON file, with optional bool to clear current container (false by default)
	bool LoadSpawnData(std::string& fp, bool clearCurrentData = false);
	//Load object collision data from JSON file
	bool LoadObjectCollisionData(std::string& fp, bool clearCurrentData = false);

	///////////
	/// Get ///
	///////////

	PlayerCompositeData* const GetCharacterDataAtID(unsigned id);
	WeaponCompositeData* const GetWeaponDataAtID(unsigned id);
	EnemyCompositeData* const GetEnemyDataAtID(unsigned id);
	AccessoryCompositeData* const GetAccessoryDataAtID(unsigned id);
	SpawnerData* const GetSpawnDataAtID(unsigned id);

	//index 0 = collsion for map 0, and so on
	ObjectCol* const GetObjectCollisionAtID(unsigned index, unsigned id);
	ObjectCollisionMap* const GetCollisionMap(unsigned index) { return &m_CollisionMaps[index]; }
	///////////
	/// Set ///
	///////////



private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//
	//Data Loading
	//

	//
	//Debug
	//

	//Assertion checks for each document and its data members

	////////////
	/// Data ///
	////////////

	//Storage containers	
	CharacterEntityMap m_CharacterMap;
	WeaponEntityMap m_WeaponMap;
	EnemyEntityMap m_EnemyMap;
	SpawnerPatternMap m_SpawnMap;
	AccessoryEntityMap m_AccessoryMap;
	ObjectCollisionMap m_ObjectCollisionMap;

	/*
	* index 0 -> map 0
	* index 1 - map 1
	* etc
	*/
	std::vector<ObjectCollisionMap> m_CollisionMaps;
};