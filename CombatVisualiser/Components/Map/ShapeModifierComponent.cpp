#include "ShapeModifierComponent.h"

#include "ShapeComponent.h"
#include "ShapeHoverComponent.h"

void mk::ShapeModifierComponent::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	IObserver::OnNotify(subjectPtr, event);

	HoverEvent* hoverEventPtr{ dynamic_cast<HoverEvent*>(event) };
	if (!hoverEventPtr)
		return;

	switch (hoverEventPtr->GetType())
	{
	case HoverEvent::Type::Enter:
		hoverEventPtr->GetShape()->SetColor({ 0, 255, 0, 255 });
		break;
	case HoverEvent::Type::Exit:
		break;
	}
}
