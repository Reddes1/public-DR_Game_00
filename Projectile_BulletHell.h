#pragma once

#include "Projectile_Interface.h"	//Parent

/*
	Special projectile-based controller for use with ability "Bullet Hell". Produces several additional
	projectiles is variable angles.
*/
class Projectile_BulletHell : public Projectile_Interface
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class StateID : unsigned
	{
		SPAWNING,
		ACTIVE,
		DESPAWNING,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	Projectile_BulletHell() { Init(); }
	~Projectile_BulletHell() { Release(); }

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this weapon is a normalised state (if required)
	bool Reset() override;

	//////////////////
	/// Operations ///
	//////////////////

	///////////
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

	//Set core functionality stats in one go
	void SetFunctionalityData(float interval, short projCount, float offsetX, float offsetY, float initAngleOffset, bool reverse);

private:

	//////////////////
	/// Operations ///
	//////////////////

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();

	////////////
	/// Data ///
	////////////

	//Internal state controll
	StateID m_State = StateID::SPAWNING;

	//Positional offsets
	float m_OffsetX = 0.f;
	float m_OffsetY = 0.f;

	//Operation data
	float m_Elapsed = 0.f;
	float m_Interval = 0.12f;
	short m_RemainingProj = 50;
	float m_AngleInc = 0.f;
	float m_CurrentAngle = 0.f;

	int m_SoundDelay = 0;
};