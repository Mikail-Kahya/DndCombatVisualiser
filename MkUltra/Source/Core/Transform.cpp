#include "Transform.h"

#include "Renderer.h"

using namespace mk;

Transform::Transform()
{
	m_Depth = Renderer::GetInstance().GetNextDepth();
}

Vector2 Transform::GetPosition() const
{
	return m_Position;
}

float Transform::GetRotation() const
{
	return m_Rotation;
}

Vector2 Transform::GetScale() const
{
	return m_Scale;
}

float Transform::GetDepth() const
{
	return m_Depth;
}

void Transform::AddOffset(const Vector2& offset)
{
	m_Position += offset;
}

void Transform::AddRotation(float deltaAngle)
{
	m_Rotation += deltaAngle;
}

void Transform::AddScale(const Vector2& deltaScale)
{
	m_Scale += deltaScale;
}

void Transform::AddDepth(float deltaDepth)
{
	m_Depth += deltaDepth;
}

void Transform::SetPosition(const Vector2& position)
{
	m_Position = position;
}

void Transform::SetRotation(float angle)
{
	m_Rotation = angle;
}

void Transform::SetScale(const Vector2& scale)
{
	m_Scale = scale;
}

void Transform::SetDepth(float depth)
{
	m_Depth = depth;
}
