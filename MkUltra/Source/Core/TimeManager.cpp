#include "TimeManager.h"

using namespace mk;

TimeManager::TimeManager()
	: start{ std::chrono::steady_clock::now() }
{
}

float TimeManager::GetTotalTime() const
{
	using namespace std::chrono;
	return duration<float>(high_resolution_clock::now() - start).count();
}

float TimeManager::GetFPS() const
{
	return 1 / deltaTime;
}
