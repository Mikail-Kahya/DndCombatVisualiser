#include "IObserver.h"
#include <algorithm>

using namespace mk;

IObserver::~IObserver()
{
	for (ISubject* subjectPtr : m_Subjects)
		subjectPtr->RemoveObserver(this);
}

void IObserver::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	if (dynamic_cast<ObserveEvent*>(event))
	{
		subjectPtr->AddObserver(this);
		m_Subjects.emplace_back(subjectPtr);
	}
	else if (dynamic_cast<StopObserveEvent*>(event) ||
		dynamic_cast<ObjectDestroyEvent*>(event))
	{
		subjectPtr->RemoveObserver(this);
		m_Subjects.erase(std::find(m_Subjects.begin(), m_Subjects.end(), subjectPtr));
	}
}

ISubject::~ISubject()
{
	const std::unique_ptr event{std::make_unique<ObjectDestroyEvent>()};
	for (IObserver* observerPtr : m_Observers)
		observerPtr->OnNotify(this, event.get());
}

void ISubject::AddObserver(IObserver* observerPtr)
{
	const auto foundIter = std::find(m_Observers.begin(), m_Observers.end(), observerPtr);
	if (foundIter == m_Observers.end())
	{
		const std::unique_ptr event{ std::make_unique<ObserveEvent>() };
		m_Observers.emplace_back(observerPtr);
		observerPtr->OnNotify(this, event.get());
	}
}

void ISubject::RemoveObserver(IObserver* observerPtr)
{
	const auto foundIter = std::find(m_Observers.begin(), m_Observers.end(), observerPtr);
	if (foundIter == m_Observers.end())
	{
		const std::unique_ptr event{ std::make_unique<StopObserveEvent>() };
		observerPtr->OnNotify(this, event.get());
	}
	m_Observers.erase(foundIter);
}

void ISubject::Notify(const std::unique_ptr<IEvent>& event)
{
	for (IObserver* observerPtr : m_Observers)
		observerPtr->OnNotify(this, event.get());
}