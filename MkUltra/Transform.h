#pragma once
#include "MkMath.h"

namespace mk
{
	class Transform final
	{
	public:
		Transform();

		Vector2 GetPosition() const;
		float GetRotation() const;
		Vector2 GetScale() const;
		float GetDepth() const;

		void AddOffset(const Vector2& offset);
		void AddRotation(float deltaAngle);
		void AddScale(const Vector2& deltaScale);
		void AddDepth(float deltaDepth);

		void SetPosition(const Vector2& position);
		void SetRotation(float angle);
		void SetScale(const Vector2& scale);
		void SetDepth(float depth);

	private:
		Vector2 m_Position{};
		float m_Depth{};
		float m_Rotation{};
		Vector2 m_Scale{};
	};
}
