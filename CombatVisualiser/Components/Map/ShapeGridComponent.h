#pragma once
#include <cstdint>
#include <vector>

#include "IComponent.h"

/*
 * Creates child objects with the shape component information
 * Rows and columns are [1,+inf[
*/
// If optimizing is needed, use one base vector for the vertices and apply this to all shapes

namespace mk
{
	class ShapeComponent;

	class ShapeGridComponent : public IComponent
	{
	public:	
		ShapeGridComponent(int nrRows, int nrCols, float gap = 0.f);
		~ShapeGridComponent() override = default;

		ShapeGridComponent(const ShapeGridComponent& other)					= delete;
		ShapeGridComponent(ShapeGridComponent&& other) noexcept				= delete;
		ShapeGridComponent& operator=(const ShapeGridComponent& other)		= delete;
		ShapeGridComponent& operator=(ShapeGridComponent&& other) noexcept	= delete;

		void Start() override;

	private:
		int m_Rows{ 1 };
		int m_Cols{ 1 };
		float m_Gap{};
	};
}
