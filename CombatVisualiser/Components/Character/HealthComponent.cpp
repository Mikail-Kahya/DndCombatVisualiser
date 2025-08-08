#include "HealthComponent.h"

#include <algorithm>

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "Events/Events.h"

using namespace mk;

HealthComponent::HealthComponent(int maxHealth, int startHealth)
	: m_MaxHealth{ maxHealth }
	, m_Health{ startHealth }
{
}

void HealthComponent::Start()
{
	BoxColliderComponent* boxPtr{ GetOwner()->GetComponent<BoxColliderComponent>() };
	if (boxPtr != nullptr)
		boxPtr->AddObserver(this);
	else
		assert(false && "No box component in healthComponent");
}

void HealthComponent::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	IObserver::OnNotify(subjectPtr, event);

	if (dynamic_cast<OverlapEvent*>(event))
		Hit();
}

int HealthComponent::GetHealth() const
{
	return m_Health;
}

void HealthComponent::Hit()
{
	m_Health = std::max(0, m_Health - 1);

	if (IsDead())
	{
		Notify(std::make_unique<ObjectDiedEvent>(100));
		Reset();
	}

	Notify(std::make_unique<DamageTakenEvent>(m_Health));
}

void HealthComponent::Reset()
{
	m_Health = m_MaxHealth;
}

bool HealthComponent::IsDead() const
{
	return m_Health <= 0;
}
