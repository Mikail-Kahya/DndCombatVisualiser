#include "ShapeGridComponent.h"

#include <iostream>

#include "GameObject.h"
#include "SceneManager.h"
#include "ShapeComponent.h"
#include "ShapeHoverComponent.h"
#include "ShapeModifierComponent.h"
#include "Functions/Pathfinding.h"

using namespace mk;

ShapeGridComponent::ShapeGridComponent(int nrRows, int nrCols, float gap)
	: m_Rows{ std::max(nrRows, 1) }
	, m_Cols{ std::max(nrCols, 1) }
	, m_Gap{ gap }
{
}

void ShapeGridComponent::Start()
{
	IComponent::Start();

	auto& scene{ SceneManager::GetInstance().GetScene() };

	auto* templateShapePtr = GetOwner()->AddComponent<ShapeComponent>();
	templateShapePtr->SetVisibility(false);

	algo::Graph graph{};
	algo::Node* prevNode{};
	// Generate grid with shape similar to template
	for (int row{}; row < m_Rows; ++row)
	{
		const float y{ row * (templateShapePtr->GetHeight() + m_Gap) };
		
		for (int col{}; col < m_Cols; ++col)
		{
			const glm::vec2 pos{ col * (templateShapePtr->GetWidth() + m_Gap), y };
			auto cellPtr{ scene.SpawnObject("Shape") };
			cellPtr->SetLocalPosition(pos);
			cellPtr->SetParent(GetOwner());

			auto cellShapePtr{ cellPtr->AddComponent<ShapeComponent>(templateShapePtr->GetPoints()) };
			cellShapePtr->SetFill(templateShapePtr->IsFilled());
			cellShapePtr->SetColor(templateShapePtr->GetColor());
			cellShapePtr->SetCloseShape(templateShapePtr->IsClosed());
			cellShapePtr->SetDepth(templateShapePtr->GetDepth());

			cellPtr->AddComponent<ShapeHoverComponent>()->AddObserver(cellPtr->AddComponent<ShapeModifierComponent>());

			graph.nodes.emplace_back(std::make_unique<GridNode>(pos, cellShapePtr));
			algo::Node* const currNode{ graph.nodes.back().get() };
			// 5FT MAGIC NUMBER FOR NOW
			if (prevNode)
				graph.connections.emplace_back(prevNode, currNode, 5.f);
			prevNode = currNode;
		}
	}

	
	algo::AStar algo{std::move(graph), [](float a, float b) { return a + b; } };
	algo.FindPath(6, 12);

	for (const auto& currNode : algo.GetPath())
	{
		std::cout << currNode->GetX() << ", " << currNode->GetY() << std::endl;
		dynamic_cast<GridNode*>(currNode)->shapePtr->SetColor({ 0, 0, 255, 255 });
	}
}
