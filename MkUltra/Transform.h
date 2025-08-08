#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

namespace mk
{
	class Transform final
	{
	public:
		Transform();

		glm::vec2 GetPosition() const;
		float GetRotation() const;
		const glm::vec2& GetScale() const;
		float GetDepth() const;

		void AddOffset(const glm::vec2& offset);
		void AddRotation(float deltaAngle);
		void AddScale(const glm::vec2& deltaScale);
		void AddDepth(float deltaDepth);

		void SetPosition(const glm::vec2& position);
		void SetRotation(float angle);
		void SetScale(const glm::vec2& scale);
		void SetDepth(float depth);

	private:
		glm::vec3 m_Position{};
		float m_Rotation{};
		glm::vec2 m_Scale{};
	};
}
