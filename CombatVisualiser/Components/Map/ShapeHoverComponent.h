#pragma once
#include "IComponent.h"
#include "IObserver.h"
#include "MkMath.h"

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
		HoverEvent(const Vector2& _mousePos, ShapeComponent* _shapePtr, Type _type);

		HoverEvent(const HoverEvent& other)					= delete;
		HoverEvent(HoverEvent&& other) noexcept				= delete;
		HoverEvent& operator=(const HoverEvent& other)		= delete;
		HoverEvent& operator=(HoverEvent&& other) noexcept	= delete;

		void Log() const;

		const Vector2 mousePos{};
		ShapeComponent* const shapePtr{};
		const Type type{ Type::Enter };
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
