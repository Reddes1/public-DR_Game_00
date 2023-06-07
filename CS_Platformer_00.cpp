#include "CS_Platformer_00.h"  //Owner

#include "Game.h" //Manager Access


void CS_Platformer_00::UpdateInputs()
{
	Game* game = Game::GetGame();

	for (unsigned i(0); i < m_ActionPressed.size(); ++i)
		m_ActionPressed[i] = game->GetKBMManager().IsPressed(m_MappedActionKeys[i]);
}

void CS_Platformer_00::UpdateProcessKey(char key)
{
	char nkey = std::toupper(key);
	switch (nkey)
	{
	case(static_cast<unsigned short>(DefaultInputs::MOVE_LEFT)):
		m_ActionPressed[0] = true;
		break;
	case(static_cast<unsigned short>(DefaultInputs::MOVE_RIGHT)):
		m_ActionPressed[1] = true;
		break;
	case(static_cast<unsigned short>(DefaultInputs::JUMP)):
		m_ActionPressed[2] = true;
		break;
	}
}

void CS_Platformer_00::ResetKeysToDefault()
{
	m_MappedActionKeys[0] = static_cast<unsigned short>(DefaultInputs::MOVE_LEFT);
	m_MappedActionKeys[1] = static_cast<unsigned short>(DefaultInputs::MOVE_RIGHT);
	m_MappedActionKeys[2] = static_cast<unsigned short>(DefaultInputs::JUMP);	
}
