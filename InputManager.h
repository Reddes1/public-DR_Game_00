#pragma once

/*
	Original design provided by Mark Featherstone.
	Modified and expanded upon by Benjamin Kimberley.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <string>
#include <assert.h>
#include <math.h>
#include <sstream>

#include <bitset>
#include "D3D.h"

/*
	Virtual key codes.
*/
#define VK_NUMPAD_ENTER 0xF0		//these three are very naughty
#define VK_LALT			0xF1		//they are actually some oem
#define VK_RALT			0xF2		//keys ATTN/FINISH/COPY - but lets pinch them :)

#define VK_0	0x30
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39
//these codes match ascii
#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4a
#define VK_K	0x4b
#define VK_L	0x4c
#define VK_M	0x4d
#define VK_N	0x4e
#define VK_O	0x4f
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5a

/*
	Mouse and keyboard input manager. Processes rawinputs provided by message handler,
	with tracking functionality for knowing inputs for frame, previous frame, and key release events.
*/
class KeyboardAndMouse
{
public:

	///////////////////
	/// Definitions ///
	///////////////////

	enum class ButtonTypes : unsigned
	{
		LBUTTON = 0,
		MBUTTON = 1,
		RBUTTON = 2
	};

	////////////////////
	/// Constructors ///
	////////////////////

	KeyboardAndMouse() { Reset(); }
	~KeyboardAndMouse() { }

	//////////////////
	/// Operations ///
	//////////////////

	//Setup mouse/key input systems, pass main window handle and optional cursor status flags
	//Call ONCE at start of program
	void Initialise(HWND hwnd, bool showMouse = true, bool confineMouse = false);

	//Zeroes all buffers and values
	void Reset();

	//Processes rawinput and signals buffers appropriately
	//Plug into message handler/pump function
	void MessageEvent(HRAWINPUT rawInput);

	//Call after all frame operations complete
	void PostProcess();

	//
	//Keyboard
	//

	//Check if specific key is pressed this frame
	bool IsPressed(unsigned short keyCode) const;
	//Check if key is pressed, with no repetition on inputs allowed till released
	bool IsPressedNoRepeat(unsigned short keyCode);
	//Check if key was released this frame
	bool IsKeyReleased(unsigned short keyCode) const;

	//
	//Mouse
	//

	//Check if mouse button pressed this frame
	bool IsMouseButtonPressed(ButtonTypes index) const { return m_MouseButtonBuffer[(unsigned)index]; }

	//Optional resetting of mouse position if mouse lost or after events (e.g window<->fullscreen)
	void OnLost();
	void OnReset();

	//Clear the Key Release Array
	void ClearKeyReleaseArray();

	///////////
	/// Get ///
	///////////

	//Text list with all keys being currently held down
	void GetPressedKeyNames(std::wstring& msg);
	//Text list with all mouse buttons and cursor position info
	void GetMouseNames(std::wstring& msg);



	//Returns collision values with optional cursor size argument (2.f, 2.f by default)
	DirectX::SimpleMath::Vector4 GetCursorCollision(DirectX::SimpleMath::Vector2 mouseSize = { 2, 2 });

	//Returns coordinates of the mouse relative to the window
	DirectX::SimpleMath::Vector2 GetCursorAbsolutePosition() { return m_CursorScreenPos; }
	//Returns the difference in movement between frames
	DirectX::SimpleMath::Vector2 GetCursorMoveDifference() { return m_CursorMoveDiff; }

	///////////
	/// Set ///
	///////////

	void SetCursorVisablity(bool showMouse);
	void SetCursorConfineFlag(bool confineMouse);
	//Set all cursor details in a single call
	void SetCursorStatus(bool showMouse, bool confineMouse);
	//Set cursor position manually
	void SetCursorPosition(int posX, int posY);

private:

	//////////////////
	/// Defintions ///
	//////////////////

	//Define buffers sizes, specific flags etc
	enum
	{
		RAWBUFF_SIZE = 512,
		KEYBUFF_SIZE = 255,
		KMASK_IS_DOWN = 1,
		MAX_BUTTONS = 3
	};

	//////////////////
	/// Operations ///
	//////////////////

	void ProcessKeys(RAWINPUT* raw);
	void ProcessMouse(RAWINPUT* raw);
	void GetMousePosAbsolute(DirectX::SimpleMath::Vector2& pos);

	////////////
	/// Data ///
	////////////

	//main window handle
	HWND m_HWND;

	//Raw input buffer
	BYTE m_RawInputBuffer[RAWBUFF_SIZE];
	//Track key pressed events
	unsigned short m_KeyPressedBuffer[KEYBUFF_SIZE];
	//Track key up events
	unsigned short m_KeyUpBuffer[KEYBUFF_SIZE];
	//Store the last frames input buffer
	unsigned short m_LastFrameKeyPressedBuffer[KEYBUFF_SIZE];

	//Track button inputs events
	bool m_MouseButtonBuffer[MAX_BUTTONS];
	//Track button release events
	bool m_MouseButtonUpBuffer[MAX_BUTTONS];

	//Track current mouse screen position
	DirectX::SimpleMath::Vector2 m_CursorScreenPos;
	//Track movement amount between frames
	DirectX::SimpleMath::Vector2 m_CursorMoveDiff;

	//Cursor window confinement flag
	bool m_ConfineCursor = false;
	//Track current and previous clipping area for cursor
	RECT m_NewCursorClip;
	RECT m_OldCursorClip;
};

/*
	Controller class.
*/
class Gamepads
{
public:

	//Gamepad Data, one used per pad
	struct State
	{
		int port = -1;  //controller ID
		float leftStickX = 0;
		float leftStickY = 0;
		float rightStickX = 0;
		float rightStickY = 0;
		float leftTrigger = 0;
		float rightTrigger = 0;
		float deadzoneX = 0.1f;
		float deadzoneY = 0.1f;
		bool inputEnabled_ = true;
		XINPUT_STATE state;
	};

	///////////////////////
	/// Setup/Utilities ///
	///////////////////////

	//Call once at startup
	void Initialise();

	//NEEDS TESTING, presumably cleared the input buffer of any input, usefull from going from one state to another
	void ClearInputBuffer();


	///////////////
	/// Setters ///
	///////////////

	//Mechanical devices can provide input when not being touched (stick drift), due to age or damage.
	//Deadzone will specify a range of input values to ignore.
	void SetDeadZone(int padID, float x, float y)
	{
		//Check that pad ID is valid
		assert(padID >= 0 && padID < XUSER_MAX_COUNT);
		pads_[padID].deadzoneX = x;
		pads_[padID].deadzoneY = y;
	}


	///////////////
	/// Getters ///
	///////////////

		//Get the state of a single pad
	const State& GetState(int padID) const
	{
		//Check that pad ID is valid
		assert(padID >= 0 && padID < XUSER_MAX_COUNT);
		return pads_[padID];
	}

	//Get a list of pad activity - motion of sticks and triggers
	void GetAnalogueNames(int padID, std::wstring& msg);

	//Get a list of pad activity - button presses
	void GetDigitalNames(int padID, std::wstring& msg);


	/////////////////
	/// Processes ///
	/////////////////

	//Called every update
	void Update();

	//Is a key pressed?
	bool IsPressed(int padID, unsigned short buttonID);

	//Check to see if pad is still plugged in
	bool IsConnected(int padID)
	{
		//Check that pad ID is valid
		assert(padID >= 0 && padID < XUSER_MAX_COUNT);
		return pads_[padID].port != -1;
	}

	//Use previous state data to check if the button is still down between frames
	bool IsButtonDown(int padID, unsigned short buttonID);


private:
	//a copy of state for each of 4 pads
	State pads_[XUSER_MAX_COUNT];
	//Hold the previous state of each pad
	State previousStates_[XUSER_MAX_COUNT];
	//Stick range
	const int STICK_RANGE = 32767;
};
