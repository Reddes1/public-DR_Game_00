#pragma once

#include "EM_Sprite.h"
#include "GameTimer.h"
#include "UITypes.h"

//Forward Dec
class Entity_Player;

/*
	UI class for displaying player health & ability bars below model.
*/
class UI_FNP_Player
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class BarIDs : unsigned
	{
		HEALTH_BACK_BAR,
		ABILITY_BACK_BAR,
		HEALTH_BAR,
		ABILITY_BAR,
		COUNT
	};

	////////////////////
	/// Constructors ///
	////////////////////

	UI_FNP_Player() { }
	~UI_FNP_Player() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Main update window
	void Update(GameTimer& gt);

	//Main render window
	void Render(DirectX::SpriteBatch& sb, DirectX::DescriptorHeap* heap);

	//Initialise the UI from file
	void InitialiseUI(std::string& fp, unsigned index);

	///////////
	/// Set ///
	///////////

	//Set player pointer ONCE
	void SetPlayerPointer(Entity_Player* player);

private:

	////////////
	/// Data ///
	////////////

	//Sprites for frames
	EM_Sprite m_Frames[(unsigned)BarIDs::COUNT];

	//Hold player pointer for quicker access to stats
	Entity_Player* m_Player = nullptr;
};
