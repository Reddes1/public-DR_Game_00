#pragma once

#include "Projectile_Interface.h"	//Parent

/*
	Special projectile-based controller for use with ability "Heaven-On-High". Produces several additional
	projectiles at random locations.
*/
class Projectile_HoH : public Projectile_Interface
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

	Projectile_HoH() { Init(); }
	~Projectile_HoH() { Release(); }

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
	void SetFunctionalityData(float interval, short projCount, float posYoffset);

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

	//Operation data
	float m_OffsetY = 0.f;
	float m_Elapsed = 0.f;
	float m_Interval = 0.12f;
	short m_RemainingProj = 50;

	int m_SoundDelay = 0;
	//Track last index looked at between projectile spawning
};