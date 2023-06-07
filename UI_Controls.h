#pragma once
#include "UITypes.h"
#include "GameConstants.h"
class UI_Controls
{
public:
	///////////////////
	/// Definitions ///
	///////////////////

	enum class Actions : unsigned
	{
		NO_ACTION,
		SELECTION_MADE
	};

	////////////////////
	/// Constructors ///
	////////////////////

	UI_Controls() { }
	~UI_Controls() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Load custom UI setup from JSON file
	void InitialiseUI(std::string& fp, unsigned index);

	void Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap);

	Actions ProcessInput(char key);


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

	//UI Elements

	//Main frame text
	NonNavUI::Data m_WindowText;

	//Menu indexing

	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };

};

