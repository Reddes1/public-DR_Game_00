#include "Loot_Experience.h"
#include "Game.h"			//Manager Access
#include "GeneralUtils.h" //Utils
void Loot_Experience::Update(GameTimer& gt)
{
	m_ExpBalance = GetPassingValue();

	//Update collision data
	SetCollisionData(CollisionArea({
		GetBaseSprite().GetPosition().x, GetBaseSprite().GetPosition().y,
		GetBaseSprite().GetScaleAdjustedRectX(), GetBaseSprite().GetScaleAdjustedRectY()
		}));

	if (GetFollowPlayer()) // Sets the loot objects velocity to follow the player untill pick up
	{
		DirectX::SimpleMath::Vector2 direction(
			GetDistanceBetweenPoints(GetBaseSprite().GetPosition().x, Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition().x, false),
			GetDistanceBetweenPoints(GetBaseSprite().GetPosition().y, Game::GetGame()->GetGameplayManager().GetEntityManager().GetPlayer()->GetRelativePosition().y, false)
		);
		direction.Normalize();
		GetBaseSprite().AddToPosition((direction.x * m_Speed) * gt.DeltaTime(), (direction.y * m_Speed) * gt.DeltaTime());
		m_Speed++;
		SetCollisionLayer(CollisionLayers::PLAYER_ONLY_COLLISION);
	}

	if (m_ExpBalance >= m_ExpLevels[0]) { // Upgrade Gem Tier 1
		m_ExpLevel++;
		GetBaseSprite().SetFrame((int)GameUI00Desc::EXPERIENCE_GEM_00);
	}
	if (m_ExpBalance >= m_ExpLevels[1]) { // Upgrade Gem Tier 2
		m_ExpLevel++;
		GetBaseSprite().SetFrame((int)GameUI00Desc::EXPERIENCE_GEM_01);
	}
	if (m_ExpBalance >= m_ExpLevels[2]) { // Upgrade Gem Tier 3
		m_ExpLevel++;
		GetBaseSprite().SetFrame((int)GameUI00Desc::EXPERIENCE_GEM_02);
	}


	//Update animation
	GetBaseAnimator().Update(gt.DeltaTime());
}

void Loot_Experience::Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
{
	//Just call default for now
	Loot_Interface::Render(batch, heap);
}

bool Loot_Experience::Reset()
{
	m_ExpBalance = 0;
	m_Speed = 300.f;
	m_ExpLevel = 0;
	SetPassingValue(0);
	SetFollowPlayer(false);
	SetCollisionLayer(CollisionLayers::FULL_COLLISION);
	return true;
}
void Loot_Experience::ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00)
{
	Game::GetGame()->GetGameplayManager().GetProgressManager().AddExp((unsigned)GetPassingValue());
	loot00->SetObjectStatus(GetPoolData().m_PoolIndex, false);
	Reset();
}
void Loot_Experience::Init()
{
	//Set safe defaults
	Reset();
}

void Loot_Experience::Release()
{
	//Release code here (deletes, clears etc)
}