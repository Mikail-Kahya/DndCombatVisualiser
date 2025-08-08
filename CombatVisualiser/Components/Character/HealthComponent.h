#pragma once
#include "IComponent.h"
#include "IEvent.h"
#include "IObserver.h"

namespace mk
{
	class HealthComponent : public IComponent, public IObserver, public ISubject
	{
	public:
		HealthComponent(int maxHealth, int startHealth);
		~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other)					= delete;
		HealthComponent(HealthComponent&& other) noexcept				= delete;
		HealthComponent& operator=(const HealthComponent& other)		= delete;
		HealthComponent& operator=(HealthComponent&& other)	noexcept	= delete;

		void Start() override;
		void OnNotify(ISubject* subjectPtr, IEvent* event) override;

		int GetHealth() const;
		bool IsDead() const;

	private:
		void Hit();
		void Reset();

		const int m_MaxHealth{};
		int m_Health{};
	};
}
