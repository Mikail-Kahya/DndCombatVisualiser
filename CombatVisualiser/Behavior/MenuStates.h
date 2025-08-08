#pragma once
#include "GameObjectState.h"

namespace mk
{
	class RenderComponent;

	class StartState : public GameObjectState
	{
	public:
		StartState(GameObject* startButton);
		~StartState() override = default;

		StartState(const StartState& other)					= delete;
		StartState(StartState&& other) noexcept				= delete;
		StartState& operator=(const StartState& other)		= delete;
		StartState& operator=(StartState&& other) noexcept	= delete;

		void OnEnter() override;
		void OnExit() override;

		std::string TransitionState() override;

	private:
		RenderComponent* m_RenderCompPtr{};
	};

	class VersusState : public GameObjectState
	{
	public:
		VersusState(GameObject* versusButton);
		~VersusState() override = default;

		VersusState(const VersusState& other) = delete;
		VersusState(VersusState&& other) noexcept = delete;
		VersusState& operator=(const VersusState& other) = delete;
		VersusState& operator=(VersusState&& other) noexcept = delete;

		void OnEnter() override;
		void OnExit() override;
		std::string TransitionState() override;

	private:
		RenderComponent* m_RenderCompPtr{};
	};
}
