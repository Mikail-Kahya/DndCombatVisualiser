#include "KeyboardInput.h"

#include <algorithm>

using namespace mk;

KeyboardInput::KeyboardInput()
{
	m_PrevKeys.resize(SDL_NUM_SCANCODES);
	m_CurrentKeys.resize(SDL_NUM_SCANCODES);
}

void KeyboardInput::Flush()
{
	std::copy(m_CurrentKeys.begin(), m_CurrentKeys.end(), m_PrevKeys.begin());
}

void KeyboardInput::Update(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN:
		if (!sdlEvent.key.repeat)
			m_CurrentKeys[sdlEvent.key.keysym.scancode] = true;
		break;
	case SDL_KEYUP:
		m_CurrentKeys[sdlEvent.key.keysym.scancode] = false;
		break;
	}
}

bool KeyboardInput::ButtonDown(SDL_Scancode key) const
{
	return !m_PrevKeys[key] && m_CurrentKeys[key];
}

bool KeyboardInput::ButtonHold(SDL_Scancode key) const
{
	return m_CurrentKeys[key];}

bool KeyboardInput::ButtonUp(SDL_Scancode key) const
{
	return m_PrevKeys[key] && !m_CurrentKeys[key];
}