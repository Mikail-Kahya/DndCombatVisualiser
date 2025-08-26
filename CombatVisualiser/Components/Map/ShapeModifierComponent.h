#pragma once
#include "IComponent.h"
#include "IObserver.h"

namespace mk
{
	class ShapeModifierComponent : public IComponent, public IObserver
	{
	public:
		ShapeModifierComponent() = default;

		ShapeModifierComponent(const ShapeModifierComponent& other)					= delete;
		ShapeModifierComponent(ShapeModifierComponent&& other) noexcept				= delete;
		ShapeModifierComponent& operator=(const ShapeModifierComponent& other)		= delete;
		ShapeModifierComponent& operator=(ShapeModifierComponent&& other) noexcept	= delete;

		void OnNotify(ISubject* subjectPtr, IEvent* event) override;

	private:
	};
}
