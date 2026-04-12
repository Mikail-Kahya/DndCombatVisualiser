#pragma once
#include <vector>

#include "IComponent.h"
#include "IRenderable.h"
#include "Texture2D.h"

// Shape component creating primitives based on vertices given
// Vertices are in the order of the vector

namespace mk
{
	class ShapeComponent : public IComponent, public IRenderable
	{
	public:
		ShapeComponent();
		ShapeComponent(const std::vector<Vector2>& points);
		~ShapeComponent() override = default;

		ShapeComponent(const ShapeComponent& other) = delete;
		ShapeComponent(ShapeComponent&& other) = delete;
		ShapeComponent& operator=(const ShapeComponent& other) = delete;
		ShapeComponent& operator=(ShapeComponent&& other) = delete;

		void Render() const override;

		const std::vector<Vector2>& GetPoints() const;
		const std::pair<Vector2, Vector2>& GetBounds() const;
		const Color& GetColor() const;
		float GetHeight() const;
		float GetWidth() const;
		bool IsFilled() const;
		bool IsClosed() const;

		bool IsPointInShape(const Vector2& point) const;

		void SetPoints(const std::vector<Vector2>& points);
		void SetColor(const Color& color);
		void SetFill(bool isFilled);
		void SetCloseShape(bool isClosed);

		void AddPoint(const Vector2& point);

	private:
		void CalculateBounds();

		std::vector<Vector2> m_Points{};
		std::pair<Vector2, Vector2> m_Bounds{};
		Color m_Color{ 255, 255, 255, 255 };
		float m_Width{};
		float m_Height{};
		bool m_IsFilled{ false };
		bool m_CloseShape{ true };
	};
}
