#pragma once

#include "EM_Sprite.h"
#include "GameTimer.h"
#include "UITypes.h"

/*
	UI Class for displaying player experience bar on the screen
*/
class UI_ExperienceBar
{
public:

	////////////
	/// Data ///
	////////////

	enum class FrameIDs
	{
		FRAME,
		BAR,
		COUNT
	};
	enum class TextIDs
	{
		LEVEL,
		TIMER,
		KILL_COUNT,
		COUNT
	};



	////////////////////
	/// Constructors ///
	////////////////////

	UI_ExperienceBar() { }
	~UI_ExperienceBar() { }


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
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

private:

	//////////////////
	/// Operations ///
	//////////////////

	////////////
	/// Data ///
	////////////

	//Sprites equal to the required frames
	EM_Sprite m_Frames[(unsigned)FrameIDs::COUNT];
	NonNavUI::Data m_Texts[(unsigned)TextIDs::COUNT];
};