#pragma once
#include "Input.h"
#include "MkMath.h"

namespace mk
{
	class ControllerInput final
	{
	public:
		ControllerInput(uint8_t idx);
		~ControllerInput();

		ControllerInput(const ControllerInput& other)					= delete;
		ControllerInput(ControllerInput&& other) noexcept;
		ControllerInput& operator=(const ControllerInput& other)		= delete;
		ControllerInput& operator=(ControllerInput&& other) noexcept;

		void UpdateInput();
		// Deadzone is in a range between 0 and 1
		void SetDeadzone(float deadzone);

		Vector2 GetLeftStickInput() const noexcept;
		Vector2 GetRightStickInput() const noexcept;
		Vector2 GetDPadInput() const noexcept;

		bool ButtonDown(Input input) const noexcept;
		bool ButtonHold(Input input) const noexcept;
		bool ButtonUp(Input input) const noexcept;

	private:
		class XControllerInput;

		XControllerInput* m_XInputImpl{};
	};
}
