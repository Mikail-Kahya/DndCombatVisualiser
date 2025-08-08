#include "InputManager.h"

#include "Controller.h"
#include "GUI.h"


using namespace mk;

bool InputManager::ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) 
	{
		if (GUI::GetInstance().ProcessSDLEvents(e))
			return true;

		Controller::PollKeyboard(e);

		if (e.type == SDL_QUIT)
			return false;
	}

	for (auto& controller : m_Controllers)
		controller.HandleInput();

	Controller::FlushKeyboard();

	return true;
}

Controller* InputManager::AddController()
{
	m_Controllers.emplace_back(m_Idx);
	++m_Idx;
	return &m_Controllers.back();
}

void InputManager::RemoveController(Controller* controllerPtr)
{
	const auto foundController = std::ranges::find_if(m_Controllers, [controllerPtr](const Controller& controller)
								{
									return controllerPtr == &controller;
								});

	m_Controllers.erase(foundController);
}

void InputManager::RemoveCommand(Command* commandPtr)
{
	auto commandIter = std::ranges::find_if(m_Commands, [commandPtr](const std::unique_ptr<Command>& command)
		{
			return command.get() == commandPtr;
		});

	m_Commands.erase(commandIter);
}