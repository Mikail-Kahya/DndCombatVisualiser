#include "Controller.h"

#include "Command.h"
#include "KeyboardInput.h"

using namespace mk;

Controller::Controller(uint8_t idx, bool useKeyboard)
	: m_Idx{ idx }
	, m_Controller{ idx }
{
	if (!s_KeyboardUsed)
	{
		s_KeyboardUsed = useKeyboard;
		m_UseKeyboard = useKeyboard;
	}
}

void Controller::PollKeyboard(const SDL_Event& e)
{
	s_Keyboard.Update(e);
}

void Controller::FlushKeyboard()
{
	s_Keyboard.Flush();
}

void Controller::HandleInput()
{
	m_Controller.UpdateInput();
	for (const auto& mapping : m_InputMapping.GetMappings())
	{
		if (mapping.first.Triggered(m_Controller, (m_UseKeyboard ? s_Keyboard : EMPTY_KEYBOARD)))
			mapping.second->Execute();
	}
}

uint8_t Controller::GetIdx() const
{
	return m_Idx;
}

const InputMapping& Controller::GetInputMapping() const
{
	return m_InputMapping;
}

void Controller::SetInputMapping(InputMapping&& map)
{
	m_InputMapping = map;
}
