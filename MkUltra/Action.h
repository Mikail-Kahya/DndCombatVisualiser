#pragma once
#include "Input.h"
#include "SDL_scancode.h"

namespace mk
{
	class KeyboardInput;
	class ControllerInput;

	class Action final
	{
	public:
		Action() = default;
		~Action() = default;

		Action(const Action& other)					= default;
		Action(Action&& other) noexcept				= default;
		Action& operator=(const Action& other)		= default;
		Action& operator=(Action&& other) noexcept	= default;

		void SetControllerInput(Input button);
		void SetKeyboardInput(SDL_Scancode button);
		void SetType(ActionType inputType);
		bool Triggered(const ControllerInput& controller, const KeyboardInput& keyboard) const;

	private:
		Input m_ControllerInput{};
		SDL_Scancode m_KeyboardInput{};
		ActionType m_Type{};
	};
}
