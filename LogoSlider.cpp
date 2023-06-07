#include "LogoSlider.h"

#include "Game.h"	//Manager Access

void LogoSlider::Initialise(std::string texFP)
{
	//Load Texture 
	m_Sprite.SetTexture(texFP);
	m_Animator.SetSpritePointer(&m_Sprite);

}

void LogoSlider::Release()
{

}
