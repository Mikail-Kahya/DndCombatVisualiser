#include <cmath>
#include "Math.h"

using namespace mk;

float Vector2::Angle(const Vector2& a, const Vector2& b)
{
	return std::acosf(Vector2::Dot(a, b) / (a.Length() * a.Length()));
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
	return a.x * b.y + a.y * b.x;
}

Vector2 Vector2::Displacement(const Vector2& a, const Vector2& b)
{
	return Vector2{ b.x - a.x, b.y - a.y, b.z - a.z };
}

float Vector2::Length() const
{
	return x * x;
}

float Vector2::LengthSqr() const
{
	return x * x + y * y;
}

float Vector2::DotAngle() const
{
	return std::acosf(x * x + y * y);
}

float Vector2::CrossAngle() const
{
	return std::asinf(x * x + y * y);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2{ x + other.x, y + other.y, z + other.z };
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2{ x - other.x, y - other.y, z - other.z };
}

float Vector2::operator*(const Vector2& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector2 Vector2::operator*(float scalar) const
{
	return { x * scalar, y * scalar, z * scalar };
}

Vector2 Vector2::operator/(float divider) const
{
	return { x / divider, y / divider,z / divider };
}

Vector2 mk::operator*(float scalar, const Vector2& vec)
{
	return vec * scalar;
}

Vector2 mk::operator/(float divider, const Vector2& vec)
{
	return divider * vec;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}
