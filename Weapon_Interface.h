#pragma once

#include "D3DUtils.h"	//DX Types
#include "GameTimer.h"	//Game Time

#include "GameTypes.h"  //Stats & Data

/*
	Base class for other weapons to be derived from
*/
class Weapon_Interface
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	Weapon_Interface() { Init(); }
	virtual ~Weapon_Interface() { Release(); }


	////////////////
	/// Virtuals ///
	////////////////

	//Main update window through the weapons behaviour is updated
	virtual void Update(GameTimer& gt) = 0;
	//Main render function
	virtual void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) = 0;

	//Initialise the weapon with desired stat package
	virtual void InitialiseWeapon(WeaponStats& statPack) { m_Stats = statPack; }

	//////////////////
	/// Operations ///
	//////////////////



	///////////
	/// Get ///
	///////////

	WeaponStats& GetStats() { return m_Stats; }

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


	////////////
	/// Data ///
	////////////

	//Basic weapon stats
	WeaponStats m_Stats;

};