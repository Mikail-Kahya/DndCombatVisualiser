#pragma once
#include "IEvent.h"


namespace mk
{
	struct DamageTakenEvent : public IEvent
	{
		DamageTakenEvent(int _health) : health{ _health } {}
		virtual ~DamageTakenEvent()	= default;
		DamageTakenEvent(const DamageTakenEvent& other)					= delete;
		DamageTakenEvent(DamageTakenEvent&& other) noexcept				= delete;
		DamageTakenEvent& operator=(const DamageTakenEvent& other)		= delete;
		DamageTakenEvent& operator=(DamageTakenEvent&& other) noexcept	= delete;

		int health{};
	};

	struct ObjectDiedEvent : public IEvent
	{
		ObjectDiedEvent(int _score) : score{ _score } {}
		virtual ~ObjectDiedEvent()	= default;
		ObjectDiedEvent(const ObjectDiedEvent& other)					= delete;
		ObjectDiedEvent(ObjectDiedEvent&& other) noexcept				= delete;
		ObjectDiedEvent& operator=(const ObjectDiedEvent& other)		= delete;
		ObjectDiedEvent& operator=(ObjectDiedEvent&& other) noexcept	= delete;

		int score{};
	};

	struct ScoreChangedEvent : public IEvent
	{
		ScoreChangedEvent(int _score) : score{ _score } {}
		virtual ~ScoreChangedEvent() = default;
		ScoreChangedEvent(const ScoreChangedEvent& other)					= delete;
		ScoreChangedEvent(ScoreChangedEvent&& other) noexcept				= delete;
		ScoreChangedEvent& operator=(const ScoreChangedEvent& other)		= delete;
		ScoreChangedEvent& operator=(ScoreChangedEvent&& other) noexcept	= delete;

		int score{};
	};
}
