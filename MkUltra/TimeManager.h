#pragma once
#include <chrono>

namespace mk
{
	struct TimeManager
	{
		TimeManager();

		TimeManager(const TimeManager& other) = default;
		TimeManager(TimeManager&& other) = default;
		TimeManager& operator=(const TimeManager& other) = default;
		TimeManager& operator=(TimeManager&& other) = default;

		float GetTotalTime() const;
		float GetFPS() const;

		float deltaTime{};
		float fixedDeltaTime{ 0.02f };

	private:
		std::chrono::steady_clock::time_point start{};
	};
}