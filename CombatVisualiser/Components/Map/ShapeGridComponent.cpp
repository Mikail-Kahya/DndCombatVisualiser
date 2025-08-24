#include "ShapeGridComponent.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "ShapeComponent.h"

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

	for (int row{}; row < m_Rows; ++row)
	{
		const float y{ row * (templateShapePtr->GetHeight() + m_Gap) };
		for (int col{}; col < m_Cols; ++col)
		{
			const float x{ col * (templateShapePtr->GetWidth() + m_Gap) };
			auto cellPtr{ scene.SpawnObject("Shape") };
			cellPtr->SetLocalPosition({ x, y });
			cellPtr->SetParent(GetOwner());

			auto cellShapePtr{ cellPtr->AddComponent<ShapeComponent>(templateShapePtr->GetPoints()) };
			cellShapePtr->SetFill(templateShapePtr->IsFilled());
			cellShapePtr->SetColor(templateShapePtr->GetColor());
			cellShapePtr->SetCloseShape(templateShapePtr->IsClosed());
			cellShapePtr->SetDepth(templateShapePtr->GetDepth());
		}
	}
}
