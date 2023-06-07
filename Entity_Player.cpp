#include "Entity_Player.h"

#include "GameConstants.h"			//Game info
#include "AnimationDefinitions.h"	//Animator info
#include "GeneralUtils.h"

#include "Game.h"					//Manager Access

void Entity_Player::Update(GameTimer& gt)
{
	//Only update if enabled
	if (GetActiveState())
	{
		//Get game for brevity
		Game* game = Game::GetGame();

		//
		//Ability Check
		//

		if (game->GetControlScheme().IsPressed((unsigned)CS_Survivor_00::DMP::ACTION_01, (unsigned)CS_Survivor_00::DMS::ACTION_01))
		{
			if (m_Ability.IsAbilityUsable())
				m_Ability.ActivateAbility();
		}
		m_Ability.Update(gt);

		//
		//Movement Cycle
		//

		//Flags for checking checking certain factors for this frame (direction + movement state)
		Directional8Way hDir = Directional8Way::NULL_DIR;
		Directional8Way vDir = Directional8Way::NULL_DIR;	
		bool moving = false;

		//Reset velocity for frame
		m_Velocity = { 0.f, 0.f };

		//Based on inputs, set velocity, animation, and flags as required
		//Note: As "camera" produces the movement effect for this game, velocities are inverted as what you might expect normally
		if (game->GetControlScheme().IsPressed((unsigned)CS_Survivor_00::DMP::MOVE_LEFT, (unsigned)CS_Survivor_00::DMS::MOVE_LEFT))
		{
			m_Velocity.x = 1.f;
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);
			hDir = Directional8Way::LEFT;
			m_LookRight = false;
			moving = true;
		}			
		else if (game->GetControlScheme().IsPressed((unsigned)CS_Survivor_00::DMP::MOVE_RIGHT, (unsigned)CS_Survivor_00::DMS::MOVE_RIGHT))
		{
			m_Velocity.x = -1.f;
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
			hDir = Directional8Way::RIGHT;
			m_LookRight = true;
			moving = true;
		}
		if (game->GetControlScheme().IsPressed((unsigned)CS_Survivor_00::DMP::MOVE_UP, (unsigned)CS_Survivor_00::DMS::MOVE_UP))
		{
			m_Velocity.y = 1.f;
			ApplyConditionalAnimation(true);
			vDir = Directional8Way::UP;
			moving = true;
		}
		else if (game->GetControlScheme().IsPressed((unsigned)CS_Survivor_00::DMP::MOVE_DOWN, (unsigned)CS_Survivor_00::DMS::MOVE_DOWN))
		{
			m_Velocity.y = -1.f;
			ApplyConditionalAnimation(true);
			vDir = Directional8Way::DOWN;
			moving = true;
		}
		//Normalise velocity
		m_Velocity.Normalize();

		//Based on if the player moved or not, execute certain actions
		if (!moving)
		{
			ApplyConditionalAnimation(false);
		}
		else
		{
			unsigned short d = (unsigned short)hDir + (unsigned short)vDir;
			m_Direction = (Directional8Way)d;
		}
		//Change velocity based on direction
		m_CurrentVelocity = m_Velocity;
		if (m_CollDirX == true)		
			m_CurrentVelocity.x = m_Velocity.x - m_Velocity.x;
		if (m_CollDirY == true)
			m_CurrentVelocity.y = m_Velocity.y - m_Velocity.y;
		if (m_CollDirX == true && m_CollDirY == true)
			m_CurrentVelocity = m_Velocity - m_Velocity;

		m_CollDirX = false;
		m_CollDirY = false;

		//Update the camera (or the players "position"), and relative position (adjusted for camera/sprite position)
		float ms = GetStats().m_Movespeed * GetStats().m_MovespeedMod;
		game->GetGameplayManager().GetCamera().UpdateCamera((ms * m_CurrentVelocity.x) * gt.DeltaTime(), (ms * m_CurrentVelocity.y) * gt.DeltaTime());
		m_RelativePosition = 
		{
			m_BaseSprite.GetPosition().x - game->GetGameplayManager().GetCamera().GetSceneTranslation().x,
			m_BaseSprite.GetPosition().y - game->GetGameplayManager().GetCamera().GetSceneTranslation().y
		};

		//
		//Weapon Update
		//

		for (auto& a : m_Weapons)
			a.Update(gt);

		//
		//Post
		//

		//Upticks timer and applys regen if 5secs elapsed
		m_RegenElapsed += gt.DeltaTime();
		if (m_RegenElapsed >= 5.f)
			ApplyRegenEffect();

		//Update collision box position
		SetCollisionData(CollisionArea({ m_RelativePosition.x, m_RelativePosition.y, GetGeneralData().m_CollisionRadius, GetGeneralData().m_CollisionRadius }));
	
		//Update animation
		m_BaseAnimator.Update(gt.DeltaTime());
	}
}

void Entity_Player::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Only render if enabled (this is set to default because this entity is likely created without a texture being set)
	if (GetRenderState())
	{
		m_BaseSprite.Draw(batch, heap);
	}
}

bool Entity_Player::ProcessDamageEvent(short damage, short knockback)
{
	//Check if the damage value ends up below zero from the effect of the armour (prevent healing bug)
	if (damage - GetStats().m_Armour < 1)
		damage = 1;
	
	//Play the player damage sound
	Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::PLAYER_DAMAGE, GameVolumes::SFX);
	//Apply damage to this entity
	GetStats().m_Health -= damage;

	Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
		GetRelativePosition(),
		damage,
		(-GetBaseSprite().GetScaleAdjustedRectY() * 0.5f) - 10.f,
		static_cast<DirectX::SimpleMath::Vector4>(DirectX::Colors::Crimson)
	);

	//Check for death
	if (GetStats().m_Health <= 0)
	{
		GetStats().m_Health = 0;

		//Disable unit
		SetActiveState(false);
		SetRenderState(false);
		//Stop playing the main game music
		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::GRASSLAND)->Stop();
		Game::GetGame()->GetAudioManager().GetSoundEffectInstanceAtIndex((unsigned)MusicIDs::CAVE)->Stop();
		//Play the gameover sound
		Game::GetGame()->GetAudioManager().PlayOneShotFromWaveBank((unsigned)WavebankIDs::SFX, (unsigned)SfxIDs::GAMEOVER, GameVolumes::SFX);
		//Set the state to game over
		Game::GetGame()->GetGameplayManager().GetStateManager().SetActiveGameState(SM_GameStateManager::AGS::LOST_STATE_PLAYER_KILLED);

		//Death event, return true
		return true;
	}

	//No death event, return false
	return false;
}

bool Entity_Player::InitAsCharacter(CharacterIDs charID)
{
	//Setup the character differently based on ID
	switch (charID)
	{
	case CharacterIDs::KNIGHT_00:
		return InitAsKnight();
		break;

	case CharacterIDs::GUNNER_00:
		return InitAsGunner();
		break;

	case CharacterIDs::MAGE_00:
		return InitAsMage();
		break;

	default:
		msg_assert(false, "InitAsCharacter(): Invalid Character Init.");
	}

	return false;
}

void Entity_Player::UpdateRelativePosition()
{
	m_RelativePosition = { m_BaseSprite.GetPosition().x - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().x,
		m_BaseSprite.GetPosition().y - Game::GetGame()->GetGameplayManager().GetCamera().GetSceneTranslation().y };
}

bool Entity_Player::AddWeaponToFreeSlot(unsigned weaponID)
{
	//Check for invalid weapon
	msg_assert(weaponID < (unsigned)WeaponTypeID::UNDEFINED, "AddWeaponToFreeSlot(): Attempting to add invalid weapon!");

	//Cycle through array, looking first free slot
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
	{
		//Check for duplicates
		msg_assert(weaponID != (unsigned)m_Weapons[i].GetWeaponID(), "AddWeaponToFreeSlot(): Attempting to add duplicate weapon!");

		//If the slot is free, set weapon and initialise as type
		if (m_Weapons[i].GetWeaponID() == WeaponTypeID::UNDEFINED)
		{
			m_Weapons[i].InitialiseWeapon((WeaponTypeID)weaponID, *Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID(weaponID));
			return true;
		}
	}

	//Failed to find a free slot
	msg_assert(false, "AddWeaponToFreeSlot(): Unable to find free weapon slot, function called incorrectly?");
	return false;
}

bool Entity_Player::AddAccessoryToFreeSlot(unsigned accessoryID)
{
	//Check for invalid accessory
	msg_assert(accessoryID < (unsigned)AccessoryTypeID::UNDEFINED, "AddAccessoryToFreeSlot(): Attempting to add invalid accessory!");

	//Cycle through array, looking first free slot
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
	{
		//Check for duplicates
		msg_assert(accessoryID != (unsigned)m_Accessories[i].GetAccessoryID(), "AddAccessoryToFreeSlot(): Attempting to add duplicate accessory!");

		//If the slot is free, set accessory and initialise as type
		if (m_Accessories[i].GetAccessoryID() == AccessoryTypeID::UNDEFINED)
		{
			m_Accessories[i].InitialiseAccessory((AccessoryTypeID)accessoryID, *Game::GetGame()->GetGameplayManager().GetDataManager().GetAccessoryDataAtID(accessoryID));
			return true;
		}
	}

	//Failed to find a free slot
	msg_assert(false, "AddAccessoryToFreeSlot(): Unable to find free accessory slot, function called incorrectly?");
	return false;
}

void Entity_Player::ApplyHealingEffect(short amount, bool useFCT)
{
	//Apply heal
	GetStats().m_Health += amount;
	if (GetStats().m_Health > GetStats().m_MaxHealth)
		GetStats().SetHPToMax();

	//Call FCT if called for
	if (useFCT)
	{
		Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
			GetRelativePosition(),
			amount,
			(-GetBaseSprite().GetScaleAdjustedRectY() * 0.5f) - 10.f,
			static_cast<DirectX::SimpleMath::Vector4>(DirectX::Colors::LawnGreen)
		);
	}
}

Player_Weapon* Entity_Player::GetWeaponByTypeID(unsigned id)
{
	//Look for weapon by type
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
	{
		if (m_Weapons[i].GetWeaponID() == (WeaponTypeID)id)
			return &m_Weapons[i];
	}
	//No weapon found
	return nullptr;
}

Player_Accessory* Entity_Player::GetAccessoryByTypeID(unsigned id)
{
	//Look for accessory by type
	for (unsigned i(0); i < MAX_ACCESSORY_COUNT; ++i)
	{
		if (m_Accessories[i].GetAccessoryID() == (AccessoryTypeID)id)
			return &m_Accessories[i];
	}
	//No accessory found
	return nullptr;
}

bool Entity_Player::AnyFreeWeaponSlots()
{
	//Cycle through array, returning true as soon as a free slot is found
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
		if (m_Weapons[i].GetWeaponID() == WeaponTypeID::UNDEFINED)
			return true;

	//No free slots, so return false
	return false;
}

bool Entity_Player::AnyFreeAccessorySlots()
{
	//Cycle through array, returning true as soon as a free slot is found
	for (unsigned i(0); i < MAX_ACCESSORY_COUNT; ++i)
		if (m_Accessories[i].GetAccessoryID() == AccessoryTypeID::UNDEFINED)
			return true;

	//No free slots, so return false
	return false;
}

DirectX::SimpleMath::Vector4 Entity_Player::GetCollisionRect()
{
	return DirectX::SimpleMath::Vector4(
	m_RelativePosition.x, m_RelativePosition.y,
		m_BaseSprite.GetScaleAdjustedRectX(),
		m_BaseSprite.GetScaleAdjustedRectY()
	);
}

void Entity_Player::LevelWeaponAtIndex(unsigned index)
{
	assert(index <= MAX_WEAPON_COUNT);

	if (m_Weapons[index].GetWeaponID() != WeaponTypeID::UNDEFINED)
		m_Weapons[index].LevelUpWeapon();
}

void Entity_Player::LevelAccessoryAtIndex(unsigned index)
{
	assert(index <= MAX_ACCESSORY_COUNT);

	if (m_Accessories[index].GetAccessoryID() != AccessoryTypeID::UNDEFINED)
		m_Accessories[index].LevelUpAccessory();
}

void Entity_Player::WaveAdvanceFix()
{
	for (auto& a : m_Weapons)
		if (a.GetWeaponID() == WeaponTypeID::AURA)
			a.ResetTrackedProjectile();
}

void Entity_Player::SetPlayerStats(LevelUpData& data)
{
	/*
	This adjusts the players stats when the accessories level up
	Using a stat index to know which stat to modify
	*/
	switch (static_cast<StatIDs>(data.m_StatIndex))
	{
	case StatIDs::PLAYER_MAX_HP:
		GetStats().m_MaxHealth += data.StatModValues.m_I16;
		GetStats().m_Health += data.StatModValues.m_I16;
		break;
	case StatIDs::PLAYER_MOVESPEED:
		GetStats().m_Movespeed += data.StatModValues.m_I16;
		break;
	case StatIDs::PLAYER_AROMOR:
		GetStats().m_Armour += data.StatModValues.m_I16;
		break;
	case StatIDs::PLAYER_PICKUP_RANGE:
		GetStats().m_PickupRange += data.StatModValues.m_I16;
		break;
	case StatIDs::PLAYER_EXTRA_PROJ:
		GetStats().m_ExtraProjCount += data.StatModValues.m_I16;
		break;
	case StatIDs::PLAYER_REGEN:
		GetStats().m_Regen += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_MOVESPEED_MOD:
		GetStats().m_MovespeedMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_DMG_MOD:
		GetStats().m_WeaponDMGMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_CD_MOD:
		GetStats().m_WeaponCDMod -= data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_PROJ_SPEED_MOD:
		GetStats().m_WeaponProjSpeedMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_AOE_MOD:
		GetStats().m_WeaponAoEMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_DURATION_MOD:
		GetStats().m_WeaponDurationMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_KNOCKBACK_MOD:
		GetStats().m_WeaponKnockbackMod += data.StatModValues.m_F32;
		break;
	case StatIDs::PLAYER_WEAPON_PICKUP_RANGE_MOD:
		GetStats().m_PickupRangeMod += data.StatModValues.m_F32;
		break;
	default:
		msg_assert(false, "SetPlayerStats(): Invalid Stat Index in level data! No stat increase applied!");
	}
}

void Entity_Player::Init()
{
	//Bind base sprite with animator
	m_BaseAnimator.SetSpritePointer(&m_BaseSprite);
	m_BaseAnimator.SetPlay(false);
	//Bind with progress manager (for leveling)
	Game::GetGame()->GetGameplayManager().GetProgressManager().SetPlayerPointer(this);

	//Set control flags
	SetActiveState(false);
	SetRenderState(false);

	//Sync weapon slots to player
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
		m_Weapons[i].SetPlayerPointer(this);
	//Sync accessory slots to player
	for (unsigned i(0); i < MAX_ACCESSORY_COUNT; ++i)
		m_Accessories[i].SetPlayerPointer(this);

	//Sync ability to player
	m_Ability.SetPlayerPointer(this);

	UpdateRelativePosition();

	//Post
	m_BaseSprite.SetLayerDepth(DRAW_DEPTH_UI_9);
}

void Entity_Player::Release()
{

}

void Entity_Player::ApplyRegenEffect()
{
	GetStats().m_Health += (short)GetStats().m_Regen;

	if (GetStats().m_Health >= GetStats().m_MaxHealth)
		GetStats().SetHPToMax();

	m_RegenElapsed = 0.f;

	Game::GetGame()->GetGameplayManager().GetFCTManager().SetupFreeFCTEntry(
		GetRelativePosition(),
		(short)GetStats().m_Regen,
		(-GetBaseSprite().GetScaleAdjustedRectY() * 0.5f) - 10.f,
		static_cast<DirectX::SimpleMath::Vector4>(DirectX::Colors::LawnGreen)
	);
}

void Entity_Player::ApplyConditionalAnimation(bool isMove)
{
	if (isMove)
		if (m_LookRight)
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_R], true, true, false, false);
		else
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::MOVE_L], true, true, false, false);
	else
		if (m_LookRight)
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::IDLE_R], true, true, false, false);
		else
			m_BaseAnimator.SetAnimation(m_AnimIndexes[AnimIndexes::IDLE_L], true, true, false, false);
}

bool Entity_Player::InitSharedBehaviour(PlayerCompositeData* data)
{
	assert(data);

	//
	//Texture + Animation
	//

	//Set texture by name
	m_BaseSprite.SetTexture(data->m_TextureData.m_TextureName);
	//If animated, store indexes of animations related to this entity
	if (data->m_TextureData.m_Animated)
	{
		//Load animation indexes into array
		for (unsigned i(0); i < data->m_TextureData.m_Frames.size(); ++i)
			m_AnimIndexes[i] = data->m_TextureData.m_Frames[i];

		m_BaseAnimator.SetAnimation(data->m_TextureData.m_Frames[0], true, true, false);
	}
	//Just set the base frame given
	else
		m_BaseSprite.SetFrame(data->m_TextureData.m_FrameIndex);

	//
	//General Data & Weapons
	//

	//Set data, stats, flags
	GetStats() = data->m_Stats;
	GetGeneralData() = data->m_GeneralData;

	//Set general details
	m_BaseSprite.SetScale(GetGeneralData().m_Scale);
	m_BaseSprite.SetPosition(Game::GetGame()->GetWindowWidthF() * 0.5f, Game::GetGame()->GetWindowHeightF() * 0.5f);
	m_BaseSprite.SetLayerDepth(DRAW_DEPTH_ENTITY_9);
	SetRenderState(true);
	SetActiveState(true);

	//Disable any existing weapons/accessories
	for (unsigned i(0); i < MAX_WEAPON_COUNT; ++i)
	{
		m_Weapons[i].DisableWeapon();
		m_Accessories[i].DisableAccessory();
	}

	//
	//Ability Setup
	//

	//Use ability ID to setup ability class
	m_Ability.InitialiseAbility(data->m_AbilityID);
	m_Ability.SetCDToHalfMax();

	return true;
}

bool Entity_Player::InitTestCharacter()
{
	//Load Character Data
	auto data = Game::GetGame()->GetGameplayManager().GetDataManager().GetCharacterDataAtID(0);

	//Run shared setup
	InitSharedBehaviour(data);

	//Set starting weapon (Variable by ID)
	m_Weapons[0].InitialiseWeapon(WeaponTypeID::BOOK,
		*Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID((unsigned)WeaponTypeID::BOOK));

	//Force update weapons to sync stats
	for (auto& a : m_Weapons)
		a.ResyncWeapon();

	return true;
}

bool Entity_Player::InitAsKnight()
{
	//Load Character Data
	auto data = Game::GetGame()->GetGameplayManager().GetDataManager().GetCharacterDataAtID((unsigned)CharacterIDs::KNIGHT_00);

	//Run shared setup
	InitSharedBehaviour(data);

	//Set starting weapon (Variable by ID)
	m_Weapons[0].InitialiseWeapon(WeaponTypeID::WHIP,
		*Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID((unsigned)WeaponTypeID::WHIP));

	//Force update weapons to sync stats
	for (auto& a : m_Weapons)
		a.ResyncWeapon();

	return true;
}

bool Entity_Player::InitAsGunner()
{
	//Load Character Data
	auto data = Game::GetGame()->GetGameplayManager().GetDataManager().GetCharacterDataAtID((unsigned)CharacterIDs::GUNNER_00);

	//Run shared setup
	InitSharedBehaviour(data);

	//Set starting weapon (Variable by ID)
	m_Weapons[0].InitialiseWeapon(WeaponTypeID::DAGGER,
		*Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID((unsigned)WeaponTypeID::DAGGER));

	//Force update weapons to sync stats
	for (auto& a : m_Weapons)
		a.ResyncWeapon();

	return true;
}

bool Entity_Player::InitAsMage()
{
	//Load Character Data
	auto data = Game::GetGame()->GetGameplayManager().GetDataManager().GetCharacterDataAtID((unsigned)CharacterIDs::MAGE_00);

	//Run shared setup
	InitSharedBehaviour(data);

	//Set starting weapon (Variable by ID)
	m_Weapons[0].InitialiseWeapon(WeaponTypeID::AURA,
		*Game::GetGame()->GetGameplayManager().GetDataManager().GetWeaponDataAtID((unsigned)WeaponTypeID::AURA));

	//Force update weapons to sync stats
	for (auto& a : m_Weapons)
		a.ResyncWeapon();

	return true;
}
