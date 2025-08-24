#include "ShapeHoverComponent.h"

#include "GameObject.h"
#include "InputManager.h"
#include "ShapeComponent.h"


mk::HoverEvent::HoverEvent(ShapeComponent* shapePtr, Type type)
	: m_ShapePtr{ shapePtr }
	, m_Type{ type }
{
}

mk::HoverEvent::Type mk::HoverEvent::GetType() const
{
	return m_Type;
}

mk::ShapeComponent* mk::HoverEvent::GetShape() const
{
	return m_ShapePtr;
}

void mk::ShapeHoverComponent::Start()
{
	IComponent::Start();
	m_ShapeCompPtr = GetOwner()->AddComponent<ShapeComponent>();
}

void mk::ShapeHoverComponent::Update()
{
	IComponent::Update();
	const bool isHovering{ m_ShapeCompPtr->IsPointInShape(InputManager::GetInstance().GetMousePosition()) };
	if (isHovering == m_IsHovering)
		return;

	Notify(std::make_unique<HoverEvent>(m_ShapeCompPtr, isHovering ? HoverEvent::Type::Enter : HoverEvent::Type::Exit));
	m_IsHovering = isHovering;
}
