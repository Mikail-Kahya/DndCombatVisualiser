#pragma once
#include <memory>
#include <vector>

#include "InputMapping.h"
#include "ISingleton.h"
#include "Controller.h"

namespace mk
{
	class Command;

	class InputManager final : public ISingleton<InputManager>
	{
		friend class ISingleton<InputManager>;
	public:
		bool ProcessInput();
		Controller* AddController();
		void RemoveController(Controller* controllerPtr);

		template<std::derived_from<Command> CommandType, typename... Args>
		CommandType* AddCommand(const Args&... args)
		{
			std::unique_ptr<CommandType> command{ std::make_unique<CommandType>(args...) };
			CommandType* commandPtr{ command.get() };
			m_Commands.emplace_back(std::move(command));
			return commandPtr;
		}

		void RemoveCommand(Command* commandPtr);

	private:
		InputManager() = default;

		uint8_t m_Idx{};
		std::vector<Controller> m_Controllers;
		std::vector<std::unique_ptr<Command>> m_Commands;
	};

}
