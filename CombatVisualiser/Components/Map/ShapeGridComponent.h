#pragma once
#include "IComponent.h"

namespace mk
{
	class ShapeGridComponent : public IComponent
	{
	public:
		ShapeGridComponent() = default;
		~ShapeGridComponent() override = default;

		ShapeGridComponent(const ShapeGridComponent& other) = delete;
		ShapeGridComponent(ShapeGridComponent&& other) = delete;
		ShapeGridComponent& operator=(const ShapeGridComponent& other) = delete;
		ShapeGridComponent& operator=(ShapeGridComponent&& other) = delete;

	private:
	};
}
