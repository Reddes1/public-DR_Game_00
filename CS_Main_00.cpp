#include "CS_Main_00.h" //Owner

#include "Game.h"		//Manager Access

CS_Main_00::CS_Main_00()
	:CS_Interface(static_cast<unsigned>(MappedKeyDefintions::ACTION_COUNT))
{
	//Call reset keys to get the container initialised
	ResetKeysToDefault();
}

void CS_Main_00::UpdateInputs()
{
	//Get KBM manager
	KeyboardAndMouse& mgr = Game::GetGame()->GetKBMManager();
	
	//Cycle through each input and update the KB
}

void CS_Main_00::UpdateProcessKey(char key)
{
	//upper the key to match VK inputs
	char nkey = std::toupper(key);

	//Identify and flag input
	switch (nkey)
	{
	case static_cast<unsigned short>(DefaultInputs::MOVE_UP):
		m_ActionPressed[0] = true;
		break;
	case static_cast<unsigned short>(DefaultInputs::MOVE_DOWN):
		m_ActionPressed[1] = true;
		break;
	case static_cast<unsigned short>(DefaultInputs::MOVE_LEFT):
		m_ActionPressed[2] = true;
		break;
	case static_cast<unsigned short>(DefaultInputs::MOVE_RIGHT):
		m_ActionPressed[3] = true;
		break;
	case static_cast<unsigned short>(DefaultInputs::CONFIRM):
		m_ActionPressed[4] = true;
		break;
	case static_cast<unsigned short>(DefaultInputs::BACK):
		m_ActionPressed[5] = true;
		break;
	}
}

void CS_Main_00::ResetKeysToDefault()
{
	//Reset the actions to defaults
	m_MappedActionKeys[0] = static_cast<unsigned short>(DefaultInputs::MOVE_UP);
	m_MappedActionKeys[1] = static_cast<unsigned short>(DefaultInputs::MOVE_DOWN);
	m_MappedActionKeys[2] = static_cast<unsigned short>(DefaultInputs::MOVE_LEFT);
	m_MappedActionKeys[3] = static_cast<unsigned short>(DefaultInputs::MOVE_RIGHT);
	m_MappedActionKeys[4] = static_cast<unsigned short>(DefaultInputs::CONFIRM);
	m_MappedActionKeys[5] = static_cast<unsigned short>(DefaultInputs::BACK);

}
