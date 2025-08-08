#include "Transform.h"

#include "Renderer.h"

using namespace mk;

Transform::Transform()
{
	m_Position.z = Renderer::GetInstance().GetNextDepth();
}

glm::vec2 Transform::GetPosition() const
{
	return { m_Position.x, m_Position.y };
}

float Transform::GetRotation() const
{
	return m_Rotation;
}

const glm::vec2& Transform::GetScale() const
{
	return m_Scale;
}

float Transform::GetDepth() const
{
	return m_Position.z;
}

void Transform::AddOffset(const glm::vec2& offset)
{
	m_Position.x += offset.x;
	m_Position.y += offset.y;
}

void Transform::AddRotation(float deltaAngle)
{
	m_Rotation += deltaAngle;
}

void Transform::AddScale(const glm::vec2& deltaScale)
{
	m_Scale += deltaScale;
}

void Transform::AddDepth(float deltaDepth)
{
	m_Position.z += deltaDepth;
}

void Transform::SetPosition(const glm::vec2& position)
{
	m_Position.x = position.x;
	m_Position.y = position.y;
}

void Transform::SetRotation(float angle)
{
	m_Rotation = angle;
}

void Transform::SetScale(const glm::vec2& scale)
{
	m_Scale = scale;
}

void Transform::SetDepth(float depth)
{
	m_Position.z = depth;
}
