#include "Geometry.h"

using namespace Geometry;

bool Geometry::PointInBox(const glm::vec2& point, const glm::vec2& location, const glm::vec2& boxExtent)
{
	const glm::vec2 min{ location - boxExtent };
	const glm::vec2 max{ location + boxExtent };

	if (min.x > point.x || point.x > max.x)
		return false;

	return min.y < point.y && point.y < max.y;
}

Box Geometry::GetBoxMinMax(const glm::vec2& location, const glm::vec2& boxExtent)
{
	return { location - boxExtent, location + boxExtent };
}