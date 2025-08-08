#pragma once
#include <utility>

#include "glm/vec2.hpp"

namespace Geometry
{
	struct Box
	{
		glm::vec2 bottomLeft{};
		glm::vec2 topRight{};
	};

	bool PointInBox(const glm::vec2& point, const glm::vec2& location, const glm::vec2& boxExtent);
	Box GetBoxMinMax(const glm::vec2& location, const glm::vec2& boxExtent);
}
