#pragma once
#include "ControllerInput.h"
#include "KeyboardInput.h"
#include "InputMapping.h"
#include "SDL_events.h"

namespace mk
{
	class Controller final
	{
	public:
		Controller(uint8_t idx, bool useKeyboard = true);
		~Controller() = default;

		Controller(const Controller& other)					= delete;
		Controller(Controller&& other) noexcept				= default;
		Controller& operator=(const Controller& other)		= delete;
		Controller& operator=(Controller&& other) noexcept	= default;

		static void PollKeyboard(const SDL_Event& e);
		static void FlushKeyboard();
		void HandleInput();

		uint8_t GetIdx() const;
		const InputMapping& GetInputMapping() const;

		void SetInputMapping(InputMapping&& map);

	private:
		uint8_t m_Idx{};
		inline static bool s_KeyboardUsed{};
		inline static KeyboardInput s_Keyboard{};
		inline static const KeyboardInput EMPTY_KEYBOARD{};
		bool m_UseKeyboard{};
		ControllerInput m_Controller;
		InputMapping m_InputMapping;
	};
}
