#pragma once

#include "EM_EntityInterface.h"	//Parent
#include "GameTypes.h"			//Player Data
#include "GameConstants.h"		//Global Data

//EM Components
#include "EM_Sprite.h"				
#include "EM_Animator.h"		

//Weapon/Accessories/Ability
#include "Player_Weapon.h"
#include "Player_Accessory.h"
#include "Player_Ability.h"

/*
	The main player entity for the game. Acts has hub for several weapons, accessories and stats,
	and multi-configurable as different game character/classes.
*/

class Entity_Player : public EM_EntityInterface2D 
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	//Relates to how animations should be stored in accociated containers (loadable and local)
	enum AnimIndexes : unsigned
	{
		ATTACK_L,
		ATTACK_R,
		IDLE_L,
		IDLE_R,
		MOVE_L,
		MOVE_R,
		MISC_00,
		MISC_01,
		MISC_02,
		MISC_03,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Entity_Player() { Init(); }
	~Entity_Player() { Release(); }


	/////////////////
	/// Overrides ///
	/////////////////

	void Update(GameTimer& gt) override;
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//
	//Combat
	//

	bool ProcessDamageEvent(short damage, short knockback) override;
	bool ProcessCollisionEvent() override { return false; }

	//////////////////
	/// Operations ///
	//////////////////

	//Initialises the player as chosen character, prepping data, setting textures etc 
	bool InitAsCharacter(CharacterIDs charID);

	//Force update the players current relative positon
	void UpdateRelativePosition();

	//Initialises the first free slot as the given weapon
	bool AddWeaponToFreeSlot(unsigned weaponID);
	//Initalises the first free slot as the given accessory
	bool AddAccessoryToFreeSlot(unsigned accessoryID);

	//Applies a healing effect on the player
	void ApplyHealingEffect(short amount, bool useFCT);


	///////////
	/// Get ///
	///////////

	EM_Sprite& GetBaseSprite() { return m_BaseSprite; }
	EM_Animator& GetBaseAnimator() { return m_BaseAnimator; }

	GeneralEntityData& GetGeneralData() { return EM_EntityInterface2D::GetGeneralData(); }
	PlayerStats& GetStats() { return EM_EntityInterface2D::GetPlayerStats(); }

	Directional8Way GetDirection() { return m_Direction; }

	Player_Weapon* GetWeapons() { return m_Weapons; }
	Player_Accessory* GetAccessories() { return m_Accessories; }

	//Find weapon (if owned) by weapon type ID
	Player_Weapon* GetWeaponByTypeID(unsigned id);
	//Find accessory (if owned) by accessory type ID
	Player_Accessory* GetAccessoryByTypeID(unsigned id);

	Player_Ability& GetAbility() { return m_Ability; }

	//Check to see if there are any free slots
	bool AnyFreeWeaponSlots();
	//Check to see if there are any free slots
	bool AnyFreeAccessorySlots();

	//Returns the players position with the camera off-set considered (Updated as part of update loop)
	DirectX::XMFLOAT2 GetRelativePosition() { return m_RelativePosition; }
	DirectX::XMFLOAT2 GetVelocity() { return m_Velocity; }

	//As the player uses relative position, provide a slightly different collision
	//rect than what is provided by EM_Sprite's collision function
	DirectX::SimpleMath::Vector4 GetCollisionRect();

	///////////
	/// Set ///
	///////////

	void SetCollisionDirectionX(bool x) { m_CollDirX = x; }
	void SetCollisionDirectionY(bool y) { m_CollDirY = y; }

	///////////////
	/// Testing ///
	///////////////

	//Debug window to level up a weapon/accessory at index (if active)
	void LevelWeaponAtIndex(unsigned index);
	void LevelAccessoryAtIndex(unsigned index);
	//Edge case fix for if one of the players weapons is the aura, and a wave skip is called
	void WaveAdvanceFix();

	void SetPlayerStats(LevelUpData& data);

private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	//Applies regen effect to player, clamping hp if above max
	void ApplyRegenEffect();

	//Support function to cut down on direction animation checking
	void ApplyConditionalAnimation(bool isMove);

	//
	//Character Inits
	//

	//Shared behaviour between each init types
	bool InitSharedBehaviour(PlayerCompositeData* data);
	//Test character, ideal for changing things
	bool InitTestCharacter();
	//Inits character "Knight"
	bool InitAsKnight();
	//Inits character as "Gunner"
	bool InitAsGunner();
	//Inits character as "Mage"
	bool InitAsMage();

	////////////
	/// Data ///
	////////////

	//Player weapons & accessories
	Player_Weapon m_Weapons[MAX_WEAPON_COUNT];
	Player_Accessory m_Accessories[MAX_ACCESSORY_COUNT];
	Player_Ability m_Ability;

	//Instead of a complex string of switch statements, store the indexes of animations
	//related to this player when initialised as a character.
	//See AnimType for what the indexes should relate too
	int m_AnimIndexes[AnimIndexes::COUNT];

	//Sprite/Animator Combo
	EM_Sprite m_BaseSprite;
	EM_Animator m_BaseAnimator;

	//As the player is fixed to the middle of the screen, and actually "moving" just moves the scene,
	//we need to track the players position as if it also moved with the scene.
	DirectX::XMFLOAT2 m_RelativePosition = { 0.f, 0.f };
	//Directional Vector for movement (wrap this into a module later)
	DirectX::SimpleMath::Vector2 m_Velocity = { 0.f, 0.f };

	//Track what character this player is (initalised to invalid value, must be set on selection).
	CharacterIDs m_CharID = CharacterIDs::COUNT;
	//Tracks the players last faced direction (used with weapons)
	Directional8Way m_Direction = Directional8Way::RIGHT;

	//Elapsed timer for applying regen effect to player
	float m_RegenElapsed = 0.f;

	//Control flag for facing (till better solution)
	bool m_LookRight = false;

	bool m_CollDirX = false;
	bool m_CollDirY = false;

	// Current Velocity for internal check (On Collisions)
	DirectX::SimpleMath::Vector2 m_CurrentVelocity = { 0.f, 0.f };
};
