#pragma once
#include "IComponent.h"
#include "IObserver.h"

/*
 *	A helper component that alerts the observers when the mouse hovers over the shape
 *	Component itself does not affect the gameobject or shape
 */

namespace mk
{
	class ShapeComponent;

	struct HoverEvent : IEvent
	{
		enum class Type : uint8_t { Enter, Exit };
		HoverEvent(ShapeComponent* shapePtr, Type type);

		HoverEvent(const HoverEvent& other) = delete;
		HoverEvent(HoverEvent&& other) noexcept = delete;
		HoverEvent& operator=(const HoverEvent& other) = delete;
		HoverEvent& operator=(HoverEvent&& other) noexcept = delete;

		Type GetType() const;
		ShapeComponent * GetShape() const;

	private:
		ShapeComponent* const m_ShapePtr{};
		const Type m_Type{ Type::Enter };
	};

	class ShapeHoverComponent : public IComponent, public ISubject
	{
	public:
		ShapeHoverComponent() = default;

		ShapeHoverComponent(const ShapeHoverComponent& other)					= delete;
		ShapeHoverComponent(ShapeHoverComponent&& other) noexcept				= delete;
		ShapeHoverComponent& operator=(const ShapeHoverComponent& other)		= delete;
		ShapeHoverComponent& operator=(ShapeHoverComponent&& other)	noexcept	= delete;

		void Start() override;
		void Update() override;

	private:
		ShapeComponent* m_ShapeCompPtr{};
		bool m_IsHovering{ false };
	};
}
