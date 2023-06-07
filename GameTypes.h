#pragma once

#include "GameConstants.h"

/*
	Global/Shared structures + Enumerations, classes and types here (Like universal unit data).
*/

/////////////////
/// Unit Data ///
/////////////////

//General shared data amongst game entities
struct GeneralEntityData
{
	static constexpr unsigned NAME_MAX_LENGTH = 32;
	
	//Entity Name
	char m_Name[NAME_MAX_LENGTH] = { "Nameless" };
	//Collision Data
	DirectX::XMFLOAT2 m_CollisionBox = { 0.f, 0.f };
	//Original Scaling
	DirectX::XMFLOAT2 m_Scale = { 1.f, 1.f };
	float m_CollisionRadius = 0.f;
	//Collision ID (Refer to CollisionTypeID)
	unsigned m_CollisionID = 0;
	//ID (relative to entities' purpose)
	short unsigned m_ID = 0;
};

//Player specific data structure
struct PlayerStats
{
	~PlayerStats() {}

	//
	//Support 
	//

	void SetHPToMax() { m_Health = m_MaxHealth; }

	//Base Stats
	short m_Health = 100;			 
	short m_MaxHealth = 100;
    short m_Movespeed = 400;		
	unsigned short m_Armour = 10;
	unsigned short m_PickupRange = 80;
	unsigned short m_ExtraProjCount = 5;
	float m_Regen = 1.f;
	//Stat Modifications	
	float m_MovespeedMod = 1.f;
	float m_WeaponDMGMod = 1.f;		 
	float m_WeaponCDMod = 1.f;		 
	float m_WeaponProjSpeedMod = 1.f;	 
	float m_WeaponAoEMod = 1.f;		 
	float m_WeaponDurationMod = 1.f; 
	float m_WeaponKnockbackMod = 1.f;
	float m_PickupRangeMod = 1.f;
};

//Basic enemy data structure
struct EnemyStats
{
	~EnemyStats() {}

	//
	//Support 
	//

	void SetHPToMax() { m_Health = m_MaxHealth; }

	//Base Stats
	short m_Health = 100;			  //Current enemy life
	short m_MaxHealth = 100;		  //Enemy max life
	short m_Movespeed = 300;		  //Speed of the enemy in game
	unsigned short m_Damage = 5;      //Damage the enemy deals to the player
	unsigned short m_XPValue = 1;     //XP the enemy gives on death
	
	float m_AttackInterval = 1.f;	  //How frequently can player he hit after contact

    //Stat/Debuff Modifications	 (1.0 = no modification, +- effect depends on stat)	  
	float m_DamageMod = 1.f;		  
	float m_MovespeedMod = 1.f;		  
	float m_FreezeDurationMod = 1.f;  //Modifier on freeze debuff effects enemy (0.0 = no effect)
	float m_KnockbackEffectMod = 1.f; //Modifier on knockback effects enemy (0.0 = no effect)
};

//Enemy flags for resisances, drops etc
struct EnemyFlags
{
	EnemyFlags()
		:m_SlowRes(false), m_InstaDeathRes(false), m_DropsChest(false),
		m_ForcedItemDrop(false), m_KBReductionApplied(false)
	{}

	//Effects
	bool m_SlowRes : 1;
	bool m_InstaDeathRes : 1;
	bool m_KBReductionApplied : 1;
	
	//Other
	bool m_DropsChest : 1;
	bool m_ForcedItemDrop : 1;

	//Padding
	bool m_Junk00 : 1;
	bool m_Junk01 : 1;
	bool m_Junk02 : 1;
	bool m_Junk03 : 1;
};
//Supporting KB data package
struct KBData
{
	KBData()
		:m_Active(false), m_Immune(false)
	{}

	//Clamps max duration to immunity window if above it (to prevent strange behaviour)
	void ClampDuration() 
	{
		if (m_Duration > m_ImmunityWindow)
			m_Duration = m_ImmunityWindow;
	}

	float m_Speed = 0.f;			  //Speed/effect of the KB
	float m_Elapsed = 0.0f;			  //Current KB duration
	float m_Duration = 0.15f;		  //How long KB should last
	float m_ImmunityWindow = 0.25f;	  //How long till next KB can be applied 
	bool m_Active : 1;				  //Is the KB active
	bool m_Immune : 1;				  //Is the entity currently immune to additional KB effects
};

//Weapon stat structure
struct WeaponStats
{
	float m_AoEScale = 1.f;				//Visual/Hitbox size of the weapon
	float m_Lifetime = 2.f;				//How long the weapon exists in the world
	float m_Cooldown = 2.f;				//Interval on till the weapon next attacks
	float m_ProjInterval = 0.25f;		//Sub-interval for time between multiple projectiles
	float m_ProjSpeedMod = 1.f;			//An additional modifier on the base speed
	float m_CooldownMod = 1.f;			//Modifier for adjusting weapon cooldown
	float m_ReHitDelay = 1.f;			//How long till this weapon can hit another

	short m_BaseDamage = 20;			//Damage the weapon deals to enemies
	short m_ProjSpeed = 100;			//Speed that the weapon moves across the screen (if applicable)
	short m_Knockback = 1;				//Knockback effect applied to enemies hit
	short m_InnateProjCount = 1;		//Initial projectile count tied to the weapon
	short m_MaxCollisions = 5;			//Maximum amount of collisions allowed weapon before expiring
};

//Projectile Stats (Mirrors/Combines values from player and weapon structures during gameplay)
struct ProjectileStats
{
	//Base Stats
	short m_Damage = 20;
	short m_Speed = 100;
	short m_CollisionsRemaining = 5;
	short m_KnockbackEffect = 1;
	float m_Lifetime = 2.f;
	float m_ReHitDelay = 1.f;
};

//Ability Class Container
struct AbilityStats
{
	float m_MaxCD = 180.f;			//Time till ability available
	float m_ElapsedCD = 0.f;		//Current ability CD remaining

	unsigned m_AbilityID = 0;		//Ability
};


//Ability Projectile Container

//Associated support container for tracking an objects pool and position in pool
struct PoolIDs
{
	short unsigned m_PoolID = 0;
	short unsigned m_PoolIndex = 0;
};

//Lightweight structure for describing texture alias and frame ID associated with entity
struct TextureData
{
	//Safe reservation of space
	TextureData() { m_Frames.reserve(8); }

	std::string m_TextureName = "NULL";
	std::vector<unsigned> m_Frames;
	unsigned m_FrameIndex = 0;
	bool m_Animated = false;
};

//Weapon/Accessory upgrade Data
struct LevelUpData
{
	//Description of the weapons effect
	static constexpr unsigned MAX_LENGTH = 48;
	char m_Description[MAX_LENGTH] = { "NO DESC" };
	//Relates the relative ID of a given stat/mod (See Enum "PlayerStatIDs")
	unsigned m_StatIndex = 0;
	//Describes the different value representations for stats
	union
	{
		float m_F32;
		short m_I16;
		unsigned short m_U16;
	} StatModValues;
};

//Wrapped up vector for LevelUpEntry(as a single level up may have more than one stat effect)
struct LevelUpEntry
{
	//Safe reserve for now
	LevelUpEntry() { m_LevelData.reserve(4); }
	std::vector<LevelUpData> m_LevelData;

};

//Wrapped up data container
struct LevelingData
{
	//Safe reserve for now
	LevelingData() { m_Levels.reserve(8); }

	//Track current level (acts as a sort of index with some adjustment required)
	unsigned m_WeaponLevel = 0;
	//Max weapon level (0 = 1, 1 = 2 etc..)
	unsigned m_MaxLevel = 7;
	//Base description of item
	static constexpr unsigned MAX_LENGTH = 96;
	char m_Description[MAX_LENGTH] = { "Trevor Belmont" };

	//Data storing all level up effects for this weapon
	std::vector<LevelUpEntry> m_Levels;
};

//Icon data that can be provided by entities/weapon etc, to UIs to display
struct IconData
{
	std::string m_TextureName = "N/A";
	int m_Frame = 0;
	float m_ScaleX = 1.f;
	float m_ScaleY = 1.f;
	bool m_PreserveOrigin = true;
};

//Composite type for storage with "SM_DataManager"
struct EnemyCompositeData
{
	GeneralEntityData m_GeneralData;
	EnemyStats m_Stats;
	EnemyFlags m_Flags;
	KBData m_KBData;
	TextureData m_TextureData;
};

//Composite type for character data storage with "SM_DataManager"
struct PlayerCompositeData
{
	GeneralEntityData m_GeneralData;
	PlayerStats m_Stats;
	TextureData m_TextureData;

	unsigned m_AbilityID = 0;
};

//Composite type for weapon data storage with "SM_DataManager"
struct WeaponCompositeData
{
	GeneralEntityData m_GeneralData;
	WeaponStats m_Stats;
	LevelingData m_LevelData;
	IconData m_IconData;
};

struct AccessoryCompositeData
{
	GeneralEntityData m_GeneralData;
	LevelingData m_LevelData;
	IconData m_IconData;
};

//Describes the data used in spawning
struct WaveEntityData
{
	WaveEntityData()
		:m_SpecialSpawnOverride(false), m_ModsEnabled(false), m_DropsChest(false),
		m_ForcedItemDrop(false), m_InstantSpawn(false), m_DespawnOnWaveEnd(false),
		m_SpawnOnce(false)
	{}

	//Calculates where the elapsed timer should start
	void CalculateInitialElapsed()
	{
		if (m_InstantSpawn)
			m_Elapsed = -m_SpawnDelay + m_SpawnInterval;
		else
			m_Elapsed = -m_SpawnDelay;
	}

	//Call after spawning has occured once.
	//Checks for spawning status, and sets elapsed to usable or unusable state (won't be able to spawn)
	void SetElapsedWithSpawnCheck()
	{
		if (!m_SpawnOnce)
			m_Elapsed = 0.f;
		else
			m_Elapsed = -120.f;
	}
	
	//Stat Modifiers (if enabled)
	float m_HealthMod = 1.0f;
	float m_MovespeedMod = 1.0f;
	float m_DamageMod = 1.0f;
	float m_XPMod = 1.0f;
	float m_ScaleMod =  1.0f;

	float m_SpawnInterval = 0.f;			 //How frequent spawns are
	float m_Elapsed = 0.f;					 //How much time till next spawn
	//Dictates how long till the first spawn timer is delayed for (if instant spawn flag active, will be -delay + interval)
	//E.g., Where interval is 3.0 and delay is 15.0, first spawn would be in 15 secs (with elapsed starting at -12.0)
	float m_SpawnDelay = 0.f;				  
		
	unsigned m_SpawnLogicID = 0;			 //What spawning logic to use relative to enemy behaviour or spawner

	unsigned short m_SpawnCount = 0;		 //Number of entities to spawn
	unsigned short m_EnemyDataID = 0;		 //Relates to enemies in Enemy_Data
	unsigned short m_BehaviourTypeID = 0;    //Relates to behaviour type(i.e., Enemy_Type_00, 01 etc)
	unsigned short m_PoolID = 0;			 //Pool where type resides
	
	bool m_SpecialSpawnOverride : 1;		 //Control flag for enemy or spawner level spawning logic
	bool m_ModsEnabled : 1;					 //Should modifiers be applied to the wave
	bool m_DropsChest : 1;					 //Should the spawned enemy drop a chest
	bool m_ForcedItemDrop : 1;				 //Should the enemy drop any special item (potion, magnet etc)
	bool m_InstantSpawn : 1;				 //Should the enemy spawn immediately
	bool m_DespawnOnWaveEnd : 1;			 //Should the enemy despawn on wave end
	bool m_SpawnOnce : 1;					 //Should the enemy spawn only once
};

//Sub-container for holding each entry for a wave
struct Wave
{
	std::vector<WaveEntityData> m_WaveEntries;
};


//Surrounding container to hold WaveEntries + extra data about the spawn pattern
struct SpawnerData
{
	SpawnerData() { m_Waves.reserve(m_WaveCount); }
	SpawnerData(unsigned reserveCount) { m_Waves.reserve(reserveCount); }

	//Each individual wave described in this pattern
	std::vector<Wave> m_Waves;
	std::string m_PatternName = "N/A";
	unsigned m_WaveCount = 30;
	unsigned m_PatternID = 0;
};




////////////////////
/// Enumerations ///
////////////////////

//Defines the player character IDs
enum class CharacterIDs : unsigned
{
	KNIGHT_00,
	GUNNER_00,
	MAGE_00,
	COUNT
};

//Defines the relative ID of stats (for use with weapon/accessory leveling system later)
enum class StatIDs : unsigned
{
	//
	//Player Base Stats
	//
	PLAYER_CURRENT_HP,
	PLAYER_MAX_HP,
	PLAYER_MOVESPEED,
	PLAYER_AROMOR,
	PLAYER_PICKUP_RANGE,
	PLAYER_EXTRA_PROJ,
	PLAYER_REGEN,
	//
	//Player Mods
	//
	PLAYER_WEAPON_MOVESPEED_MOD,
	PLAYER_WEAPON_DMG_MOD,
	PLAYER_WEAPON_CD_MOD,
	PLAYER_WEAPON_PROJ_SPEED_MOD,
	PLAYER_WEAPON_AOE_MOD,
	PLAYER_WEAPON_DURATION_MOD,
	PLAYER_WEAPON_KNOCKBACK_MOD,
	PLAYER_WEAPON_PICKUP_RANGE_MOD,
	//
	//Weapon Base Stats
	//
	WEAPON_BASE_DAMAGE,
	WEAPON_PROJ_SPEED,
	WEAPON_KNOCKBACK,
	WEAPON_BASE_PROJ_COUNT,
	WEAPON_MAX_COLLISIONS,
	WEAPON_AOE_SCALE,
	WEAPON_LIFETIME,
	WEAPON_COOLDOWN_MOD,
	WEAPON_PROJ_CD_INTERVAL,
	WEAPON_PROJ_SPEED_MOD,
	WEAPON_REHIT_DELAY,
	WEAPON_FLAT_COOLDOWN
};

//Type defintions for the accociated classes (might be used for searches to avoid type_id condition checks)
enum class EnemyTypeID
{
	//Locked In, No Changing
	CHASER_00,
	EXPLODER_00,
	SHOOTER_00,
	SWEEPER_00,
	//New/Testing/NYI After this point
	COUNT
};

enum class EquipmentTypeID
{
	WEAPON,
	ACCESSORY,
	COUNT
};

//Weapon definitions
enum class WeaponTypeID
{
	//Basic Weapons
	CROSS = 0,
	WHIP = 1,
	DAGGER = 2,
	BOMB = 3,
	AURA = 4,
	BOOK = 5,
	AXE = 6,
	HEX = 7,
	UNDEFINED, // KEEP THIS IN THE SAME RELATIVE POSITION, IS USED FOR COUNTING ELSEWHERE
	LIGHTNING,
	//Special Items
	SUPER = 100,
	//Abilities
	HEAVEN_ON_HIGH_PROJECTILE = 200,
	BULLET_HELL_GATE,
	BULLET_HELL_PROJECTILE,
	WATER_DRAGON,
	HEAVEN_ON_HIGH_ANGEL
};

//Accessory definitions
enum class AccessoryTypeID
{
	//Locked In, No Changing
	FORT_NECK = 0,
	BOOTS = 1,
	CHESTPLATE = 2,
	MAGNET = 3,
	FOCUS = 4,
	REJUVENATING_WATER = 5,
	WIZARD_HAT = 6,
	TIME_ACCEL = 7,
	TIME_DECEL = 8,
	THROWING_GUARD = 9,
	MAGNIFIER = 10,
	//New/Testing/NYI After this point
	UNDEFINED, // KEEP THIS IN THE SAME RELATIVE POSITION, IS USED FOR COUNTING ELSEWHERE
	COUNT
};

//Loot definitions
enum class LootTypeID
{
	//Locked In, No Changing
	EXP = 0,
	FOOD = 1,
	MAGNET = 2,
	CHEST = 3,
	SUPER = 4,
	FREEZE = 5,
	WIPE = 6,
	UNDEFINED,
	COUNT
};

struct CollisionArea {
	float posX = 0.f;
	float posY = 0.f;
	float width = 0.f;
	float height = 0.f;
};

enum class CollisionLayers : unsigned short
{
	FULL_COLLISION,
	PLAYER_ONLY_COLLISION,
	ENEMY_ONLY_COLLISION,
	COUNT
};

struct ObjectCol
{
	CollisionArea m_CollisonBox;
	CollisionLayers m_CollisonLayer = CollisionLayers::FULL_COLLISION;
};

enum class MapIDs
{
	GRASSLAND,
	CAVE,
	COUNT
};

// Wavebank ID definitions
enum class WavebankIDs {
	MUSIC,
	SFX,
	COUNT
};

/*
* Definitions for the music stored in the SoundEffectsWavebank
* For now use these when selecting which loop to play, as the only sounds that should loop
* at the moment is the music and the have been loaded into Soundeffect instances in this same order
*/
enum class MusicIDs {
	MAIN_MENU,
	GRASSLAND,
	CAVE,
	COUNT
};

// Defininitions for the SFX stored in the SoundEffectsWavebank
enum class SfxIDs {
	AURA,
	BOMB,
	BOOK,
	CROSS,
	DAGGER,
	WHIP,
	ENEMY_DAMAGE,
	PLAYER_DAMAGE,
	EXP,
	LEVEL_UP,
	HEALTH_POTION,
	GAMEOVER,
	MENU_SELECT,
	ANGEL_SPAWN,
	AXE,
	FIREBALL_POWER,
	SMITE,
	WATER_DRAGON,
	BULLET_HELL_ACTIVE,
	BULLET_HELL_DOWN,
	BULLET_HELL_SHOT,
	HEX,
	CHEST,
	COUNT
};

//////////////////////
/// Collision Data ///
//////////////////////

//Describes what type of collision should be used
enum class CollisionTypeID
{
	CIRCLE,
	RECT,
	NON_ALIGNED_RECT,
	OVAL,
	COUNT
};

//Describes player hitbox values (set these in json file)
static constexpr DirectX::XMFLOAT2 PLAYER_CHAR_HITBOXES[] = 
{
	{64.f, 104.f},		//Knight
	{44.f, 96.f},		//Gunner
	{42.f, 104.f}		//Mage
};

//Describes player radiuses (set these in json file)
static constexpr float PLAYER_CHAR_RADIUSES[] =
{
	{PLAYER_CHAR_HITBOXES[0].x * 0.5f},		//Knight
	{PLAYER_CHAR_HITBOXES[1].x * 0.5f},		//Gunner
	{PLAYER_CHAR_HITBOXES[2].x * 0.5f}		//Mage
};

//Describes enemy hitbox values (set these in json file)
static constexpr DirectX::XMFLOAT2 ENEMY_HITBOXES[] =
{
	{116.f, 142.f},		//Robot
	{69.f, 113.f},		//Skeleton Archer
	{87.f, 107.f},		//Skeleton Warrior
	{78.f, 106.f},		//Skeleton Chief
	{106.f, 77.f}		//Wolf
};

static constexpr float ENEMY_RADIUSES[] =
{
	{ENEMY_HITBOXES[0].x * 0.5f},		//Robot
	{ENEMY_HITBOXES[1].x * 0.5f},		//Skeleton Archer
	{ENEMY_HITBOXES[2].x * 0.5f},		//Skeleton Warrior
	{ENEMY_HITBOXES[3].x * 0.5f},		//Skeleton Chief
	{ENEMY_HITBOXES[4].y * 0.6f},		//Wolf
};

//Describes weapon hitbox values (set these in json file) - This also accounts for TexPacker Scaling
static constexpr DirectX::XMFLOAT2 WEAPON_HITBOXES[] =
{
	{38.4f, 38.4f},		//Cross
	{210.f, 68.f},		//Whip
	{44.f, 22.f},		//Dagger
	{150.f, 150.f},		//Bomb Exp
	{48.f, 48.f}		//Book
};

static constexpr float WEAPON_RADIUSES[] =
{
	{19.2f},  	//Cross
	{75.f},		//Bomb
	{82.f},		//Aura
	{16.f}		//Bullets (00-02)
};