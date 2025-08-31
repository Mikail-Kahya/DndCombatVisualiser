#pragma once
#include <cstdint>
#include <vector>

#include "IComponent.h"
#include "IObserver.h"
#include "Functions/Pathfinding.h"

/*
 * Creates child objects with the shape component information
 * Rows and columns are [1,+inf[
*/
// If optimizing is needed, use one base vector for the vertices and apply this to all shapes

namespace mk
{
	class ShapeComponent;

	struct GridNode final : algo::Node
	{
		GridNode(const glm::vec2& _position, ShapeComponent* _shapePtr) : position{ _position }, shapePtr{ _shapePtr } {}

		GridNode(const GridNode& other) = delete;
		GridNode(GridNode&& other) noexcept = delete;
		GridNode& operator=(const GridNode& other) = delete;
		GridNode& operator=(GridNode&& other) noexcept = delete;

		std::unique_ptr<Node> Clone() const override { return std::make_unique<GridNode>(position, shapePtr); }
		float GetX() const override { return  position.x; }
		float GetY() const override { return  position.y; }

		const glm::vec2 position{};
		ShapeComponent* shapePtr{};
	};

	class ShapeGridComponent : public IComponent, public IObserver
	{
	public:	
		ShapeGridComponent(int nrRows, int nrCols, float gap = 0.f);

		ShapeGridComponent(const ShapeGridComponent& other)					= delete;
		ShapeGridComponent(ShapeGridComponent&& other) noexcept				= delete;
		ShapeGridComponent& operator=(const ShapeGridComponent& other)		= delete;
		ShapeGridComponent& operator=(ShapeGridComponent&& other) noexcept	= delete;

		void Start() override;

		void OnNotify(ISubject* subjectPtr, IEvent* event) override;

	private:
		void ResetField() const;

		std::unique_ptr<algo::AStar> m_Pathfinding{};
		GridNode* m_ActiveNodePtr{};
		int m_Rows{ 1 };
		int m_Cols{ 1 };
		float m_Gap{};
	};
}
