#pragma once
#include <vector>

#include "SDL_events.h"

namespace mk
{
	class KeyboardInput final
	{
	public:
		KeyboardInput();
		~KeyboardInput() = default;

		KeyboardInput(const KeyboardInput& other)					= delete;
		KeyboardInput(KeyboardInput&& other) noexcept				= delete;
		KeyboardInput& operator=(const KeyboardInput& other)		= delete;
		KeyboardInput& operator=(KeyboardInput&& other) noexcept	= delete;

		void Flush();
		void Update(const SDL_Event& sdlEvent);

		bool ButtonDown(SDL_Scancode key) const;
		bool ButtonHold(SDL_Scancode key) const;
		bool ButtonUp(SDL_Scancode key) const;

	private:
		std::vector<bool> m_PrevKeys{};
		std::vector<bool> m_CurrentKeys{};
	};
}
