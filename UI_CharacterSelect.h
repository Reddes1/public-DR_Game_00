#pragma once
#include "UITypes.h"
#include "GameConstants.h"

class UI_CharacterSelect
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

	UI_CharacterSelect() { }
	~UI_CharacterSelect() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Clear current functions and disable associated flags
	void Reset();

	//Load custom UI setup from JSON file
	void InitialiseUI(std::string& uiFP, std::string& cdFP, std::string& wdFP, unsigned index);

	void Render(DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap);

	Actions ProcessInput(char key);


	///////////
	/// Get ///
	///////////

	///////////
	/// Set ///
	///////////

	void SetMaxIndex(unsigned max) { if (max >= MAX_CHARACTER_INDEXES) m_MaxIndex = MAX_CHARACTER_INDEXES -1; else m_MaxIndex = max; }
private:

	//////////////////
	/// Operations ///
	//////////////////
	void SelectCharacter(unsigned index);

	void SetupCharacterFrame(std::string& cdFP, std::string& wdFP, unsigned frameIndex, unsigned CharacterID);

	void SetupWeaponAndAbilityTexts(std::string& wdFP, unsigned frameIndex, unsigned weaponID, unsigned abilityID);

	void TextRenderCall(unsigned currentIndex, DirectX::SpriteBatch& sb, DirectX::SpriteFont& sf, DirectX::DescriptorHeap* heap, DirectX::SimpleMath::Vector2 origin);

	////////////
	/// Data ///
	////////////

	//UI Elements
	
	//Background
	EM_Sprite m_Background;
	//Character Option Frames
	EM_Sprite m_CharacterOptionFrame[MAX_CHARACTER_INDEXES];
	//Selection frame
	EM_Sprite m_CharacterSelection;
	//Character icons
	EM_Sprite m_CharacterImages[MAX_CHARACTER_INDEXES];
	EM_Sprite m_CharacterWepaons[MAX_CHARACTER_INDEXES];
	//Title Frame
	EM_Sprite m_TitleFrame;
	//Info frame
	EM_Sprite m_InfoFrame;
	//Main frame text
	NonNavUI::Data m_WindowText;
	//Info texts
	NonNavUI::Data m_InfoText[MAX_CHARACTER_INDEXES];
	NonNavUI::Data m_WeaponTexts[MAX_CHARACTER_INDEXES];
	NonNavUI::Data m_AbilityTexts[MAX_CHARACTER_INDEXES];

	DirectX::XMFLOAT2 m_Offests = { 0.f,0.f };
	DirectX::XMFLOAT2 m_TextOffset = { 0.f,0.f };
	//Menu indexing
	unsigned  m_CurrentIndex = 0;
	unsigned  m_MaxIndex = 0;

	//Testing

};

