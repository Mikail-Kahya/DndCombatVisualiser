#pragma once
#include "IComponent.h"
#include "IObserver.h"

namespace mk
{
	class TextComponent;

	class HealthBarComponent : public IComponent, public IObserver
	{
	public:
		HealthBarComponent() = default;
		~HealthBarComponent() override = default;

		HealthBarComponent(const HealthBarComponent& other)					= delete;
		HealthBarComponent(HealthBarComponent&& other) noexcept				= delete;
		HealthBarComponent& operator=(const HealthBarComponent& other)		= delete;
		HealthBarComponent& operator=(HealthBarComponent&& other) noexcept	= delete;

		void Start() override;

		void OnNotify(ISubject* subjectPtr, IEvent* event) override;

	private:
		TextComponent* m_TextCompPtr{};
	};
}
