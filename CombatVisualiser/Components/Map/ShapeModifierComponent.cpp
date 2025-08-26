#include "ShapeModifierComponent.h"

#include <iostream>

#include "ShapeComponent.h"
#include "ShapeHoverComponent.h"

void mk::ShapeModifierComponent::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	IObserver::OnNotify(subjectPtr, event);

	HoverEvent* hoverEventPtr{ dynamic_cast<HoverEvent*>(event) };
	if (!hoverEventPtr)
		return;

	switch (hoverEventPtr->type)
	{
	case HoverEvent::Type::Enter:
		hoverEventPtr->shapePtr->SetColor({ 0, 255, 0, 255 });
		break;
	case HoverEvent::Type::Exit:
		hoverEventPtr->Log();
		hoverEventPtr->shapePtr->SetColor({ 255, 0, 0, 255 });
		break;
	}
}
