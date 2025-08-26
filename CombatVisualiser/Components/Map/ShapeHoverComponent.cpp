#include "ShapeHoverComponent.h"

#include <iostream>

#include "GameObject.h"
#include "InputManager.h"
#include "ShapeComponent.h"


mk::HoverEvent::HoverEvent(const glm::vec2& _mousePos, ShapeComponent* _shapePtr, Type _type)
	: mousePos{ _mousePos }
	, shapePtr{ _shapePtr }
	, type{ _type }
{
}

void mk::HoverEvent::Log() const
{
	const glm::vec2 shapePos{ shapePtr->GetOwner()->GetWorldPosition() };
	std::cout << "==================================\n"
		<< "EventInfo:\n"
		<< "---------\n"
		<< "MousePos: [" << mousePos.x << ", " << mousePos.y << "]\n"
		<< "Bounds:  XMin; " << shapePtr->GetBounds().first.x + shapePos.x << "\tXMax; " << shapePtr->GetBounds().second.x + shapePos.x << "\n"
		<< "         YMin; " << shapePtr->GetBounds().first.y + shapePos.y << "\tYMax; " << shapePtr->GetBounds().second.y + shapePos.y << "\n"
		<< "==================================\n";
}

void mk::ShapeHoverComponent::Start()
{
	IComponent::Start();
	m_ShapeCompPtr = GetOwner()->AddComponent<ShapeComponent>();
}

void mk::ShapeHoverComponent::Update()
{
	IComponent::Update();
	const glm::vec2 mousePos{ InputManager::GetInstance().GetMousePosition() };
	const bool isHovering{ m_ShapeCompPtr->IsPointInShape(mousePos) };
	if (isHovering == m_IsHovering)
		return;

	const HoverEvent::Type eventType{ isHovering ? HoverEvent::Type::Enter : HoverEvent::Type::Exit };
	Notify(std::make_unique<HoverEvent>(mousePos, m_ShapeCompPtr, eventType));
	m_IsHovering = isHovering;
}
