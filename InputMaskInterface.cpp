#include "InputMaskInterface.h"

#include "Game.h"	//Manager Access

void InputMaskInterface::BindToKBM(KeyboardAndMouse* const kbm)
{
	if (kbm != nullptr)
		m_KBM = kbm;
	else
		msg_assert(false, "BindToKBM(): Nullptr passed, cannot bind!");
}

bool InputMaskInterface::IsPressed(unsigned short keyCode)
{
	return m_KBM->IsPressed(keyCode);
}

bool InputMaskInterface::IsPressed(unsigned short pKeyCode, unsigned short sKeyCode)
{
	return m_KBM->IsPressed(pKeyCode) || m_KBM->IsPressed(sKeyCode);
}

bool InputMaskInterface::IsPressedNoRepeat(unsigned short keyCode)
{
	return m_KBM->IsPressedNoRepeat(keyCode);
}

bool InputMaskInterface::IsPressedNoRepeat(unsigned short pKeyCode, unsigned short sKeyCode)
{
	return m_KBM->IsPressedNoRepeat(pKeyCode) || m_KBM->IsPressedNoRepeat(sKeyCode);
}

bool InputMaskInterface::IsKeyReleased(unsigned short keyCode)
{
	return m_KBM->IsKeyReleased(keyCode);
}

bool InputMaskInterface::IsKeyReleased(unsigned short pKeyCode, unsigned short sKeyCode)
{
	return m_KBM->IsKeyReleased(pKeyCode) || m_KBM->IsKeyReleased(sKeyCode);
}
