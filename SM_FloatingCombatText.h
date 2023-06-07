#pragma once
#include "GameTimer.h"

#include "SimpleMath.h"

/*
	Floating Combat Text class that takes a postion, value to display and offset information, stores it as part of
	DoD structure and renders it based on lifetime.
*/
class SM_FloatingCombatText
{
public:

	////////////
	/// Data ///
	////////////



	////////////////////
	/// Constructors ///
	////////////////////

	SM_FloatingCombatText() { Init(); }
	~SM_FloatingCombatText() { Release(); }


	////////////////
	/// Virtuals ///
	////////////////



	//////////////////
	/// Operations ///
	//////////////////

	void Update(GameTimer & gt);
	void Render();

	//Find and setup a free FCT entry
	bool SetupFreeFCTEntry(DirectX::XMFLOAT2& pos, short dmgValue, float yOffset = -10.f);
	//Find and setup a free FCT entry, and set custom colour
	bool SetupFreeFCTEntry(DirectX::XMFLOAT2& pos, short value, float yOffset, DirectX::SimpleMath::Vector4& colour);

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

	//Basic Init Function
	void Init();
	//Basic Release Function
	void Release();


	////////////
	/// Data ///
	////////////

	//DoD based structure for managing floating combat text
	struct DoD_FloatingCombatText
	{
		static constexpr unsigned MAX_COUNT = 2048;
		static constexpr unsigned MAX_CHAR_LENGTH = 6;
		static constexpr float LIFE_TIME_MAX = 1.75f;

		//Colours
		_declspec(align(16)) DirectX::SimpleMath::Vector4 m_Colour[MAX_COUNT];
		//Positions
		_declspec(align(16)) float m_PosX[MAX_COUNT];
		_declspec(align(16)) float m_PosY[MAX_COUNT];
		//Font ID
		_declspec(align(16)) unsigned m_FontID[MAX_COUNT];
		//Font Lifetime
		_declspec(align(16)) float m_Lifetime[MAX_COUNT];
		//Draw strings
		_declspec(align(16)) char m_DrawString[MAX_COUNT][MAX_CHAR_LENGTH];


	};
	typedef DoD_FloatingCombatText DoD_FCT;
	DoD_FCT* m_Data;


};