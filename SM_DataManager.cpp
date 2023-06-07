#include "SM_DataManager.h"
#include "SM_DataManager_Strings.h"	//File Strings

bool SM_DataManager::LoadCharacterData(std::string& fp, bool clearCurrentData)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_CharacterMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["Character Data"].GetArray();

	//For each data entry, create a pair, extract data and store in map
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		//Create pair (Unique ID, Data)
		std::pair<unsigned, PlayerCompositeData> nP;

		//Get Unique ID
		nP.first = arr[i]["Unique ID"].GetUint();

		//Get General Data
		std::string name = arr[i]["Name"].GetString();
		assert(name.length() < GeneralEntityData::NAME_MAX_LENGTH);
		strcpy(nP.second.m_GeneralData.m_Name, name.c_str());
		nP.second.m_GeneralData.m_ID = arr[i]["Unique ID"].GetUint();

		//Entity Stats
		nP.second.m_Stats.m_MaxHealth = static_cast<short>(arr[i]["Stats"]["Base Health"].GetInt());
		nP.second.m_Stats.SetHPToMax();
		nP.second.m_Stats.m_Movespeed = static_cast<short>(arr[i]["Stats"]["Base Movespeed"].GetInt());
		nP.second.m_Stats.m_Armour = static_cast<unsigned short>(arr[i]["Stats"]["Armour"].GetUint());
		nP.second.m_Stats.m_Regen = arr[i]["Stats"]["Regen Per/5"].GetFloat();
		nP.second.m_Stats.m_ExtraProjCount = arr[i]["Stats"]["Extra Projectile Count"].GetUint();

		//Stat Modification
		nP.second.m_Stats.m_WeaponDMGMod = arr[i]["Stats"]["Weapon Damage Mod"].GetFloat();
		nP.second.m_Stats.m_WeaponCDMod = arr[i]["Stats"]["Weapon Cooldown Mod"].GetFloat();
		nP.second.m_Stats.m_WeaponProjSpeedMod = arr[i]["Stats"]["Weapon Projectile Speed Mod"].GetFloat();
		nP.second.m_Stats.m_WeaponAoEMod = arr[i]["Stats"]["Weapon AoE Mod"].GetFloat();
		nP.second.m_Stats.m_WeaponDurationMod = arr[i]["Stats"]["Weapon Duration Mod"].GetFloat();
		nP.second.m_Stats.m_WeaponKnockbackMod = arr[i]["Stats"]["Weapon Knockback Mod"].GetFloat();
		nP.second.m_Stats.m_PickupRangeMod = arr[i]["Stats"]["Pickup Range Mod"].GetFloat();
		nP.second.m_Stats.m_MovespeedMod = arr[i]["Stats"]["Movespeed Mod"].GetFloat();
		
		//Flags
		//None Yet

		//Texture Information
		nP.second.m_TextureData.m_TextureName = arr[i]["Texture Info"]["Texture Name"].GetString();
		nP.second.m_TextureData.m_Animated = arr[i]["Texture Info"]["Animated"].GetBool();

		//Based on animation flag, store accociated animation indexes
		if (nP.second.m_TextureData.m_Animated)
		{
			//Reserve space
			nP.second.m_TextureData.m_Frames.reserve(arr[i]["Texture Info"]["Animated IDs"].GetArray().Size());

			//For each frame, store it
			for (unsigned j(0); j < arr[i]["Texture Info"]["Animated IDs"].GetArray().Size(); ++j)
			{
				nP.second.m_TextureData.m_Frames.push_back(arr[i]["Texture Info"]["Animated IDs"][j].GetUint());
			}
		}
		else
		{
			nP.second.m_TextureData.m_FrameIndex = arr[i]["Texture Info"]["Non-Animated ID"].GetUint();
		}

		//Misc Data
		nP.second.m_GeneralData.m_CollisionID = arr[i]["Misc"]["Collision Type ID"].GetUint();
		nP.second.m_GeneralData.m_CollisionRadius = arr[i]["Misc"]["Collision Radius"].GetFloat();
		nP.second.m_GeneralData.m_CollisionBox.x = arr[i]["Misc"]["Collision Box"][0].GetFloat();
		nP.second.m_GeneralData.m_CollisionBox.y = arr[i]["Misc"]["Collision Box"][1].GetFloat();
		nP.second.m_GeneralData.m_Scale.x = arr[i]["Misc"]["Scale Adjustments"][0].GetFloat();
		nP.second.m_GeneralData.m_Scale.y = arr[i]["Misc"]["Scale Adjustments"][1].GetFloat();

		//Gameplay
		nP.second.m_AbilityID = arr[i]["Gameplay"]["Ability ID"].GetUint();

		//Store Pair
		m_CharacterMap.insert(nP);
	}

	//Job done, return true
	return true;
}

bool SM_DataManager::LoadWeaponData(std::string& fp, bool clearCurrentData)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_WeaponMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["Weapon Data"].GetArray();

	//For each data entry, create a pair, extract data and store in map
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		//Create pair (Unique ID, Data)
		std::pair<unsigned, WeaponCompositeData> nP;
		
		//Get Unique ID
		nP.first = arr[i]["Unique ID"].GetUint();

		//Get General Data
		assert(arr[i]["Name"].GetStringLength() < GeneralEntityData::NAME_MAX_LENGTH);
		strcpy(nP.second.m_GeneralData.m_Name, std::string(arr[i]["Name"].GetString()).c_str());
		nP.second.m_GeneralData.m_ID = arr[i]["Unique ID"].GetUint();

		//Weapon Stats
		nP.second.m_Stats.m_BaseDamage = arr[i]["Stats"]["Base Damage"].GetInt();
		nP.second.m_Stats.m_ProjSpeed = arr[i]["Stats"]["Base Projectile Speed"].GetInt();
		nP.second.m_Stats.m_Knockback = arr[i]["Stats"]["Base Knockback"].GetInt();
		nP.second.m_Stats.m_InnateProjCount = static_cast<unsigned short>(arr[i]["Stats"]["Base Projectile Count"].GetInt());
		nP.second.m_Stats.m_MaxCollisions = static_cast<short>(arr[i]["Stats"]["Base Collision Count"].GetInt());

		nP.second.m_Stats.m_Lifetime = arr[i]["Stats"]["Base Lifetime"].GetFloat();
		nP.second.m_Stats.m_Cooldown = arr[i]["Stats"]["Base Cooldown"].GetFloat();
		nP.second.m_Stats.m_ProjInterval = arr[i]["Stats"]["Interval Cooldown"].GetFloat();
		nP.second.m_Stats.m_ReHitDelay = arr[i]["Stats"]["Re-Hit Delay"].GetFloat();

		//Weapon Description
		assert(arr[i]["Misc"]["Description"].GetStringLength() < LevelingData::MAX_LENGTH);
		strcpy(nP.second.m_LevelData.m_Description, std::string(arr[i]["Misc"]["Description"].GetString()).c_str());

		//Leveling System (if defined)
		if (arr[i].HasMember("Level Up Details"))
		{
			nP.second.m_LevelData.m_MaxLevel = arr[i]["Level Up Details"]["Max Level"].GetUint();
			//Array for brevity
			const rapidjson::Value& lvlArr = arr[i]["Level Up Details"]["Level Breakdown"].GetArray();
			//For each level described in the weapon
			for (unsigned j(0); j < lvlArr.Size(); ++j)
			{
				//Create Levelup entry
				LevelUpEntry lvlEntry;
				//Another array for brevity
				const rapidjson::Value& lvlEffArr = lvlArr[j]["Level Effects"].GetArray();
				//For each effect in each level
				for (unsigned k(0); k < lvlEffArr.Size(); ++k)
				{
					//Create data entry
					LevelUpData lvlData;

					//Description
					assert(lvlEffArr[k]["Description"].GetStringLength() < LevelUpData::MAX_LENGTH);
					strcpy(lvlData.m_Description, std::string(lvlEffArr[k]["Description"].GetString()).c_str());

					//Index and Value
					lvlData.m_StatIndex = lvlEffArr[k]["Stat Index"].GetUint();

					//Store value based on type
					if (lvlEffArr[k]["Value"].IsFloat())
						lvlData.StatModValues.m_F32 = lvlEffArr[k]["Value"].GetFloat();
					else if (lvlEffArr[k]["Value"].IsInt())
						lvlData.StatModValues.m_I16 = lvlEffArr[k]["Value"].GetInt();
					else
						lvlData.StatModValues.m_U16 = lvlEffArr[k]["Value"].GetUint();

					//Store data in lvlEntry
					lvlEntry.m_LevelData.push_back(lvlData);
				}
				//Store the entry
				nP.second.m_LevelData.m_Levels.push_back(lvlEntry);
			}
		}		

		//Flags
		//None Yet

		//Texture Info
		nP.second.m_IconData.m_TextureName = arr[i]["Icon Info"]["Texture Name"].GetString();
		nP.second.m_IconData.m_Frame = arr[i]["Icon Info"]["Frame ID"].GetInt();
		nP.second.m_IconData.m_ScaleX = arr[i]["Icon Info"]["Scale"][0].GetFloat();
		nP.second.m_IconData.m_ScaleY = arr[i]["Icon Info"]["Scale"][1].GetFloat();

		//Store Pair
		m_WeaponMap.insert(nP);
	}

	//Job done, return true
	return true;
}

bool SM_DataManager::LoadEnemyData(std::string& fp, bool clearCurrentData)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_EnemyMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["Enemy Data"].GetArray();

	//For each data entry, create a pair, extract data and store in map
	for (unsigned i(0); i < arr.Size(); ++i)
	{	
		//Create pair (Unique ID, Data)
		std::pair<unsigned, EnemyCompositeData> nP;

		//Get Unique ID
		nP.first = arr[i]["Unique ID"].GetUint();

		//Get General Data
		std::string name = arr[i]["Name"].GetString();
		assert(name.length() < GeneralEntityData::NAME_MAX_LENGTH);
		strcpy(nP.second.m_GeneralData.m_Name, name.c_str());
		nP.second.m_GeneralData.m_ID = arr[i]["Unique ID"].GetUint();

		//Entity Stats
		nP.second.m_Stats.m_MaxHealth = arr[i]["Stats"]["Base Health"].GetUint();
		nP.second.m_Stats.SetHPToMax();
		nP.second.m_Stats.m_Movespeed = arr[i]["Stats"]["Base Movespeed"].GetUint();
		nP.second.m_Stats.m_Damage = arr[i]["Stats"]["Base Damage"].GetUint();
		nP.second.m_Stats.m_XPValue = arr[i]["Stats"]["Base XP Value"].GetUint();
		nP.second.m_Stats.m_AttackInterval = arr[i]["Stats"]["Attack Interval"].GetFloat();

		//Modifications
		nP.second.m_Stats.m_KnockbackEffectMod = arr[i]["Stats"]["Knockback Res Mod"].GetFloat();
		nP.second.m_Stats.m_FreezeDurationMod = arr[i]["Stats"]["Freeze Res Mod"].GetFloat();

		//KB Data
		nP.second.m_KBData.m_Duration = arr[i]["Stats"]["Knockback Duration"].GetFloat();
		nP.second.m_KBData.m_ImmunityWindow = arr[i]["Stats"]["Knockback Immunity Window"].GetFloat();
		nP.second.m_KBData.ClampDuration();

		//Flags
		nP.second.m_Flags.m_SlowRes = arr[i]["Flags"]["Slow Res"].GetBool();
		nP.second.m_Flags.m_InstaDeathRes = arr[i]["Flags"]["Instant Death Res"].GetBool();

		//Texture Information
		nP.second.m_TextureData.m_TextureName = arr[i]["Texture Info"]["Texture Name"].GetString();
		nP.second.m_TextureData.m_Animated = arr[i]["Texture Info"]["Animated"].GetBool();

		//Based on animation flag, store accociated animation indexes
		if (nP.second.m_TextureData.m_Animated)
		{
			//Reserve space
			nP.second.m_TextureData.m_Frames.reserve(arr[i]["Texture Info"]["Animated IDs"].GetArray().Size());

			//For each frame, store it
			for (unsigned j(0); j < arr[i]["Texture Info"]["Animated IDs"].GetArray().Size(); ++j)
			{
				nP.second.m_TextureData.m_Frames.push_back(arr[i]["Texture Info"]["Animated IDs"][j].GetUint());
			}
		}
		else
		{
			nP.second.m_TextureData.m_FrameIndex = arr[i]["Texture Info"]["Non-Animated ID"].GetUint();
		}

		//Misc Data
		nP.second.m_GeneralData.m_CollisionID = arr[i]["Misc"]["Collision Type ID"].GetUint();
		nP.second.m_GeneralData.m_CollisionRadius = arr[i]["Misc"]["Collision Radius"].GetFloat();
		nP.second.m_GeneralData.m_CollisionBox.x = arr[i]["Misc"]["Collision Box"][0].GetFloat();
		nP.second.m_GeneralData.m_CollisionBox.y = arr[i]["Misc"]["Collision Box"][1].GetFloat();
		nP.second.m_GeneralData.m_Scale.x = arr[i]["Misc"]["Scale Adjustments"][0].GetFloat();
		nP.second.m_GeneralData.m_Scale.y = arr[i]["Misc"]["Scale Adjustments"][1].GetFloat();

		//Store Pair
		m_EnemyMap.insert(nP);
	}

	//Job done, return true
	return true;
}

bool SM_DataManager::LoadAccessoryData(std::string& fp, bool clearCurrentData)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_AccessoryMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["Accessory Data"].GetArray();

	//For each data entry, create a pair, extract data and store in map
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		//Create pair (Unique ID, Data)
		std::pair<unsigned, AccessoryCompositeData> nP;

		//Get Unique ID
		nP.first = arr[i]["Unique ID"].GetUint();

		//Get General Data
		std::string name = arr[i]["Name"].GetString();
		assert(name.length() < GeneralEntityData::NAME_MAX_LENGTH);
		strcpy(nP.second.m_GeneralData.m_Name, name.c_str());
		nP.second.m_GeneralData.m_ID = arr[i]["Unique ID"].GetUint();
		//Get the base description of the accessory
		assert(arr[i]["Misc"]["Description"].GetStringLength() < LevelingData::MAX_LENGTH);
		strcpy(nP.second.m_LevelData.m_Description, std::string(arr[i]["Misc"]["Description"].GetString()).c_str());
		//Accessory Level up data
		
		//Leveling System (if defined)
		if (arr[i].HasMember("Level Up Details"))
		{
			nP.second.m_LevelData.m_MaxLevel = arr[i]["Level Up Details"]["Max Level"].GetUint();
			//Array for brevity
			const rapidjson::Value& lvlArr = arr[i]["Level Up Details"]["Level Breakdown"].GetArray();
			//For each level described in the accessory
			for (unsigned j(0); j < lvlArr.Size(); ++j)
			{
				//Create Levelup entry
				LevelUpEntry lvlEntry;
				//Another array for brevity
				const rapidjson::Value& lvlEffArr = lvlArr[j]["Level Effects"].GetArray();
				//For each effect in each level
				for (unsigned k(0); k < lvlEffArr.Size(); ++k)
				{
					//Create data entry
					LevelUpData lvlData;

					//Description
					assert(lvlEffArr[k]["Description"].GetStringLength() < LevelUpData::MAX_LENGTH);
					strcpy(lvlData.m_Description, std::string(lvlEffArr[k]["Description"].GetString()).c_str());

					//Index and Value
					lvlData.m_StatIndex = lvlEffArr[k]["Stat Index"].GetUint();

					//Store value based on type
					if (lvlEffArr[k]["Value"].IsFloat())
						lvlData.StatModValues.m_F32 = lvlEffArr[k]["Value"].GetFloat();
					else if (lvlEffArr[k]["Value"].IsInt())
						lvlData.StatModValues.m_I16 = lvlEffArr[k]["Value"].GetInt();
					else
						lvlData.StatModValues.m_U16 = lvlEffArr[k]["Value"].GetUint();

					//Store data in lvlEntry
					lvlEntry.m_LevelData.push_back(lvlData);
				}
				//Store the entry
				nP.second.m_LevelData.m_Levels.push_back(lvlEntry);
			}
		}
		//Flags
		//None Yet

		//Texture Info
		nP.second.m_IconData.m_TextureName = arr[i]["Icon Info"]["Texture Name"].GetString();
		nP.second.m_IconData.m_Frame = arr[i]["Icon Info"]["Frame ID"].GetInt();
		nP.second.m_IconData.m_ScaleX = arr[i]["Icon Info"]["Scale"][0].GetFloat();
		nP.second.m_IconData.m_ScaleY = arr[i]["Icon Info"]["Scale"][1].GetFloat();

		//Store Pair
		m_AccessoryMap.insert(nP);
	}

	//Job done, return true
	return true;
}

bool SM_DataManager::LoadSpawnData(std::string& fp, bool clearCurrentData)
{
	//Create and parse document
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_SpawnMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["Wave Patterns"].GetArray();

	//For each wave pattern described
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		//Create pair (Unique ID, Data)
		std::pair<unsigned, SpawnerData> nP;

		//Get Unique ID
		nP.first = arr[i]["Unique ID"].GetUint();
		//Get General Pattern Data
		nP.second.m_PatternName = arr[i]["Pattern Name"].GetString();
		nP.second.m_WaveCount = arr[i]["Waves Count"].GetUint();
		nP.second.m_PatternID = arr[i]["Unique ID"].GetUint();

		//Reserve space based on wave count
		nP.second.m_Waves.reserve(nP.second.m_WaveCount);
		
		//For each wave
		for (unsigned j(0); j < arr[i]["Waves"].GetArray().Size(); ++j)
		{
			Wave wave;

			//Get current entry for brevity
			rapidjson::Value& eArr = arr[i]["Waves"][j]["Wave Entries"].GetArray();

			//For each enemy entry
			for (unsigned k(0); k < eArr.Size(); ++k)
			{
				WaveEntityData entry;

				//Enemy IDs and Pool Location
				entry.m_EnemyDataID = eArr[k]["Enemy Data ID"].GetUint();
				entry.m_BehaviourTypeID = eArr[k]["Behaviour Type ID"].GetUint();
				entry.m_PoolID = eArr[k]["Pool Location ID"].GetUint();

				//If any mods used in this entry, store all of them, else keep as defaults
				entry.m_ModsEnabled = eArr[k]["Mods Used"].GetBool();
				if (entry.m_ModsEnabled)
				{
					entry.m_HealthMod = eArr[k]["Mods"]["Health Mod"].GetFloat();
					entry.m_MovespeedMod = eArr[k]["Mods"]["Movespeed Mod"].GetFloat();
					entry.m_DamageMod = eArr[k]["Mods"]["Damage Mod"].GetFloat();
					entry.m_XPMod = eArr[k]["Mods"]["XP Mod"].GetFloat();
					entry.m_ScaleMod = eArr[k]["Mods"]["Scale Mod"].GetFloat();
				}

				//Spawning Data
				entry.m_SpawnInterval = eArr[k]["Spawning"]["Spawn Interval"].GetFloat();
				entry.m_SpawnCount = eArr[k]["Spawning"]["Spawn Count"].GetUint();
				entry.m_SpawnDelay = eArr[k]["Spawning"]["Spawn Delay"].GetFloat();
				entry.m_SpawnOnce = eArr[k]["Spawning"]["Spawn Once"].GetBool();

				//If instant spawning flagged, set elapsed to interval time
				entry.m_InstantSpawn = eArr[k]["Spawning"]["Instant Spawn"].GetBool();
				entry.CalculateInitialElapsed();

				//If override flag, store special ID, else the regular ID
				entry.m_SpecialSpawnOverride = eArr[k]["Spawning"]["Special Spawn Override"].GetBool();
				if (entry.m_SpecialSpawnOverride)
					entry.m_SpawnLogicID = eArr[k]["Spawning"]["Special Spawn Location ID"].GetUint();
				else
					entry.m_SpawnLogicID = eArr[k]["Spawning"]["Spawn Location ID"].GetUint();
			
				//Misc
				entry.m_DropsChest = eArr[k]["Misc"]["Drops Chest"].GetBool();
				entry.m_ForcedItemDrop = eArr[k]["Misc"]["Force Item Drop"].GetBool();
				entry.m_DespawnOnWaveEnd = eArr[k]["Misc"]["Despawn At Wave End"].GetBool();

				//Store entry
				wave.m_WaveEntries.push_back(entry);
			}
		
			//Store in pattern
			nP.second.m_Waves.push_back(wave);
		}

		//Store Pair
		m_SpawnMap.insert(nP);
	}

	//Job done, return true
	return true;
}

bool SM_DataManager::LoadObjectCollisionData(std::string& fp, bool clearCurrentData)
{
	//Create and parse JSON doc
	rapidjson::Document doc;
	ParseNewJSONDocument(doc, fp);

	//Check for clearing flag
	if (clearCurrentData)
		m_ObjectCollisionMap.clear();

	//For brevity
	rapidjson::Value& arr = doc["layers"][3]["objects"].GetArray();

	//for each data entry, create a pair, extract the data and store it
	for (unsigned i(0); i < arr.Size(); ++i)
	{
		std::pair<unsigned, ObjectCol> nP;

		//Get unique ID
		nP.first = arr[i]["id"].GetUint();

		//Calculate and store RECT data
		nP.second.m_CollisonBox.posX = arr[i]["x"].GetFloat() + (arr[i]["width"].GetFloat() / 2);
		nP.second.m_CollisonBox.posY = arr[i]["y"].GetFloat() + (arr[i]["height"].GetFloat() / 2);
		nP.second.m_CollisonBox.width = arr[i]["width"].GetFloat();
		nP.second.m_CollisonBox.height = arr[i]["height"].GetFloat();

		//store the pair
		m_ObjectCollisionMap.insert(nP);
	}

	m_CollisionMaps.push_back(m_ObjectCollisionMap);
	//job done
	return true;
}

PlayerCompositeData* const SM_DataManager::GetCharacterDataAtID(unsigned id)
{
	CharacterEntityMap::iterator it = m_CharacterMap.find(id);

	//If ID was valid, return ptr to data
	if (it != m_CharacterMap.end())
		return &it->second;

	//No data found, return nullptr
	return nullptr;
}

WeaponCompositeData* const SM_DataManager::GetWeaponDataAtID(unsigned id)
{
	WeaponEntityMap::iterator it = m_WeaponMap.find(id);

	//If ID was valid, return ptr to data
	if (it != m_WeaponMap.end())
		return &it->second;

	//No data found, return nullptr
	return nullptr;
}

EnemyCompositeData* const SM_DataManager::GetEnemyDataAtID(unsigned id)
{
	EnemyEntityMap::iterator it = m_EnemyMap.find(id);
	
	//If ID was valid, return ptr to data
	if (it != m_EnemyMap.end())
		return &it->second;
	
	//No data found, return nullptr
	return nullptr;
}

SpawnerData* const SM_DataManager::GetSpawnDataAtID(unsigned id)
{
	SpawnerPatternMap::iterator it = m_SpawnMap.find(id);

	//If ID was valid, return ptr to data
	if (it != m_SpawnMap.end())
		return &it->second;

	//No data found, return nullptr
	return nullptr;
}

ObjectCol* const SM_DataManager::GetObjectCollisionAtID(unsigned index, unsigned id)
{
	ObjectCollisionMap::iterator it = m_CollisionMaps[index].find(id);

	//if ID was valid return ptr to data
	if (it != m_CollisionMaps[index].end())
	{
		return &it->second;
	}

	//No data found, return nullptr
	return nullptr;
}

AccessoryCompositeData* const SM_DataManager::GetAccessoryDataAtID(unsigned id)
{
	AccessoryEntityMap::iterator it = m_AccessoryMap.find(id);

	//If ID was valid, return ptr to data
	if (it != m_AccessoryMap.end())
		return &it->second;

	//No data found, return nullptr
	return nullptr;
}

void SM_DataManager::Init()
{
	m_CollisionMaps.reserve(4);
}

void SM_DataManager::Release()
{
}

