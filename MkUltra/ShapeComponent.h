#pragma once
#include <cstdint>
#include <vector>

#include "RenderComponent.h"
#include "Texture2D.h"

namespace mk
{
	class ShapeComponent : public RenderComponent
	{
		using Point = glm::vec2;
	public:
		ShapeComponent();
		ShapeComponent(const std::vector<Point>& points);
		~ShapeComponent() override = default;

		ShapeComponent(const ShapeComponent& other) = delete;
		ShapeComponent(ShapeComponent&& other) = delete;
		ShapeComponent& operator=(const ShapeComponent& other) = delete;
		ShapeComponent& operator=(ShapeComponent&& other) = delete;

		void Render() const;

		const std::vector<Point>& GetPoints() const;
		const std::pair<Point, Point>& GetBounds() const;
		bool IsFilled() const;

		bool IsPointInShape(const glm::vec2& point) const;

		void SetPoints(const std::vector<Point>& points);
		void SetColor(const Color& color);
		void SetFill(bool isFilled);

		void AddPoint(const Point& point);

	private:
		void CalculateBounds();

		std::vector<Point> m_Points{};
		std::pair<Point, Point> m_Bounds{};
		Color m_Color{ 255, 255, 255, 255 };
		float m_Width{};
		float m_Height{};
		bool m_IsFilled{ false };
	};
}
