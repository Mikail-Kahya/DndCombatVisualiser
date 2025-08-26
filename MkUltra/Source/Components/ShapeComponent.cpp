#include "ShapeComponent.h"

#include <algorithm>
#include <iostream>

#include "GameObject.h"
#include "Renderer.h"
#include "glm/vec2.hpp"

using namespace mk;

ShapeComponent::ShapeComponent()
{
}

ShapeComponent::ShapeComponent(const std::vector<Point>& points)
	: m_Points{ points }
{
	CalculateBounds();
}

void ShapeComponent::Render() const
{
	if (m_Points.empty())
		return;

	const glm::vec2 pos{ GetOwner()->GetWorldPosition() };
	std::vector<Point> points{ m_Points };
	std::transform(points.cbegin(), points.cend(), points.begin(), [&pos](const Point& point)
		{
			return Point{ point.x + pos.x, point.y + pos.y };
		});

	if (m_CloseShape)
		points.emplace_back(*points.begin());

	Renderer::GetInstance().RenderShape(points, m_Color);

	if (!m_IsFilled)
		return;

	for (int x { (int) m_Bounds.first.x }; x < m_Bounds.second.x; ++x)
	{
		for (int y{ (int)m_Bounds.first.y }; y < m_Bounds.second.y; ++y)
		{
			const glm::vec2 point{ glm::vec2{ x, y} + pos };
			if (IsPointInShape(point))
				Renderer::GetInstance().RenderPoint(point, m_Color);
		}
	}
}

const std::vector<ShapeComponent::Point>& ShapeComponent::GetPoints() const
{
	return m_Points;
}

const std::pair<ShapeComponent::Point, ShapeComponent::Point>& ShapeComponent::GetBounds() const
{
	return m_Bounds;
}

const Color& ShapeComponent::GetColor() const
{
	return m_Color;
}

float ShapeComponent::GetHeight() const
{
	return m_Height;
}

float ShapeComponent::GetWidth() const
{
	return m_Width;
}

bool ShapeComponent::IsFilled() const
{
	return m_IsFilled;
}

bool ShapeComponent::IsClosed() const
{
	return m_CloseShape;
}

bool ShapeComponent::IsPointInShape(const glm::vec2& point) const
{
	const glm::vec2 worldPos{ GetOwner()->GetWorldPosition() };
	if (point.x < m_Bounds.first.x + worldPos.x || point.x > m_Bounds.second.x + worldPos.x)
		return false;

	if (point.y < m_Bounds.first.y + worldPos.y || point.y > m_Bounds.second.y + worldPos.y)
		return false;

	const int32_t nrPoints{ static_cast<int32_t>(m_Points.size()) };
	const glm::vec2 p2{ point.x + m_Width, point.y };
	int32_t intersectCount{};

	// Use own cross rather than glm::vec3 for performance (looped function)
	auto cross = [](const glm::vec2& a, const glm::vec2& b) -> float
		{ return a.x * b.y - b.x * a.y; };

	// Loop over every edge (last edge is looped back)
	for (size_t idx{}; idx < m_Points.size(); ++idx)
	{
		const glm::vec2& q1{ m_Points[idx] + worldPos };
		const glm::vec2& q2{ m_Points[(idx + 1) % nrPoints] + worldPos };

		
		const glm::vec2 r{ p2 - point };
		const glm::vec2 s{ q2 - q1 };
		const glm::vec2 pq{ q1 - point };

		const float rsCross{ cross(r, s) };
\
		// Skip parallel
		if (abs(rsCross) < FLT_EPSILON)
			continue;

		const float t = cross(pq, s) / rsCross;
		const float u = cross(pq, r) / rsCross;

		// Intersection if t and u are in [0,1]
		if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1))
			++intersectCount;
	}

	// Even results in false
	return intersectCount % 2;
}

void ShapeComponent::SetPoints(const std::vector<Point>& points)
{
	m_Points = points;
	CalculateBounds();
}

void ShapeComponent::SetColor(const Color& color)
{
	m_Color = color;
}

void ShapeComponent::SetFill(bool isFilled)
{
	m_IsFilled = isFilled;
}

void ShapeComponent::SetCloseShape(bool isClosed)
{
	m_CloseShape = isClosed;
}

void ShapeComponent::AddPoint(const Point& point)
{
	m_Points.emplace_back(point);
	CalculateBounds();
}

void ShapeComponent::CalculateBounds()
{
	float minX{ FLT_MAX };
	float minY{ FLT_MAX };
	float maxX{ FLT_MIN };
	float maxY{ FLT_MIN };

	for (const Point& point : m_Points)
	{
		if (point.x < minX)
			minX = point.x;

		if (point.x > maxX)
			maxX = point.x;

		if (point.y < minY)
			minY = point.y;

		if (point.y > maxY)
			maxY = point.y;
	}

	m_Bounds = { { minX, minY }, {maxX, maxY} };
	m_Width = maxX - minX;
	m_Height = maxY - minY;
}
