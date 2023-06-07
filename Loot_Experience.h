#pragma once
#include "Loot_Interface.h"

/*
	Experience object for boosting the players power level.
*/
class Loot_Experience : public Loot_Interface
{
public:

	////////////
	/// Data ///
	////////////



	//////////////////
	/// Defintions ///
	//////////////////



	////////////////////
	/// Constructors ///
	////////////////////

	Loot_Experience() { Init(); }
	~Loot_Experience() { Release(); }

	/////////////////
	/// Overrides ///
	/////////////////

	//Core Update Function
	void Update(GameTimer& gt) override;
	//Core Render Function
	void Render(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap) override;

	//External window to allow callers to reset this loot object is a normalised state (if required)
	bool Reset() override;


	//////////////////
	/// Operations ///
	//////////////////

	void ProcessCollisionEvent(Entity_Player* player, EntityPool<Loot_Interface*>* loot00) override;

	///////////
	/// Get ///
	///////////

	int GetExperience() {return m_ExpBalance;}

	///////////
	/// Set ///
	///////////

	void AddExperience(int exp) { m_ExpBalance += exp; }

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

	int m_ExpBalance = 0;
	int m_ExpLevel = 0;

	// Blue, Green, Red Experience Gems
	int m_ExpLevels[3] = { 0, 2, 10 };

	float m_Speed = 300.f ;
};