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

			cellPtr->AddComponent<ShapeHoverComponent>()->AddObserver(this);

			graph.nodes.emplace_back(std::make_unique<GridNode>(pos, cellShapePtr));
		}
	}

	// Fill graph with connections
	const int nrNodes{ static_cast<int>(graph.nodes.size()) };
	for (int idx{}; idx < nrNodes; ++idx)
	{
		// Check every neighbouring node
		const int checkRanges[]{ -m_Cols - 1, -m_Cols, -m_Cols + 1,
								 -1,				    1,
								  m_Cols - 1,  m_Cols,  m_Cols + 1 };
		for (auto checkRange : checkRanges)
		{
			constexpr float moveWeight{ 5.f };
			const int checkIdx{ idx + checkRange };
			if (checkIdx >= 0 && checkIdx < nrNodes)
				graph.connections.emplace_back(	graph.nodes[idx].get(),
												graph.nodes[checkIdx].get(),
												moveWeight);
		}
	}
	
	m_Pathfinding = std::make_unique<algo::AStar>(std::move(graph), [](float a, float b) { return a + b; });
}

void ShapeGridComponent::OnNotify(ISubject* subjectPtr, IEvent* event)
{
	IObserver::OnNotify(subjectPtr, event);

	HoverEvent* hoverEventPtr{ dynamic_cast<HoverEvent*>(event) };
	if (!hoverEventPtr)
		return;

	size_t hoverIdx{};
	for (size_t idx{}; idx < m_Pathfinding->GetGraph().nodes.size(); ++idx)
	{
		auto gridNodePtr{ dynamic_cast<GridNode*>(m_Pathfinding->GetGraph().nodes[idx].get())};
		gridNodePtr->shapePtr->SetColor({ 0, 0, 255, 255 });
		if (gridNodePtr->shapePtr == hoverEventPtr->shapePtr)
			hoverIdx = idx;
	}

	switch (hoverEventPtr->type)
	{
	case HoverEvent::Type::Enter:
		m_Pathfinding->FindPath(0, hoverIdx);
		for (const auto nodePtr : m_Pathfinding->GetPath())
			dynamic_cast<GridNode*>(nodePtr)->shapePtr->SetColor({ 255, 0, 0, 255 });
		hoverEventPtr->shapePtr->SetColor({ 0, 255, 0, 255 });
		break;
	}
}
