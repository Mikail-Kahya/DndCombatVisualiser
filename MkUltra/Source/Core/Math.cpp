#include <cmath>
#include "Math.h"

#include <algorithm>
#include <cassert>

using namespace mk;

float Vector2::AngleBetween(const Vector2& a, const Vector2& b)
{
	const float denom{ a.Length() * b.Length() };
	if (denom < EPSILON) 
		return 0.f;

	const float dotAngle{ std::clamp(Dot(a, b) / denom, -1.f, 1.f) };
	return std::acosf(dotAngle);
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Determinant(const Vector2& a, const Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

float Vector2::SignedAngle(const Vector2& a, const Vector2& b)
{
	return std::atan2(Determinant(a, b), Dot(a, b));
}

Vector2 Vector2::Displacement(const Vector2& a, const Vector2& b)
{
	return b - a;
}

float Vector2::Distance(const Vector2& a, const Vector2& b)
{
	return (b - a).Length();
}

float Vector2::DistanceSqr(const Vector2& a, const Vector2& b)
{
	return (b - a).LengthSqr();
}

Vector2 Vector2::Perpendicular(const Vector2& vec)
{
	return { -vec.y, vec.x };
}

Vector2 Vector2::Project(const Vector2& a, const Vector2& b)
{
	const float lengthSqr{ b.LengthSqr() };

	assert(std::abs(lengthSqr) > EPSILON && "Vector2 projection with zero vector");

	return b * (Dot(a, b) / lengthSqr);
}

Vector2 Vector2::Reflect(const Vector2& vec, Vector2 normal)
{
	assert(std::abs(normal.LengthSqr()) > EPSILON && "Vector2 reflection with zero normal vector");
	assert(std::abs(normal.LengthSqr() - 1.f) < EPSILON && "Vector2 reflection with non-unit normal vector");

	return vec - 2.0f * Dot(vec, normal) * normal;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
	return a + (b - a) * t;
}

float Vector2::Length() const
{
	return std::sqrtf(x * x + y * y);
}

float Vector2::LengthSqr() const
{
	return x * x + y * y;
}

Vector2& Vector2::Normalize()
{
	const float length{ Length() };

	assert(std::abs(length) > EPSILON && "Vector2 normalized with zero vector");

	x /= length;
	y /= length;

	return *this;
}

Vector2 Vector2::Normalized() const
{
	const float length{ Length() };

	assert(std::abs(length) > EPSILON && "Vector2 normalized with zero vector");

	return *this / length;
}

Vector2 Vector2::ClampLength(float maxLength) const
{
	float lengthSqr = LengthSqr();

	if (lengthSqr < EPSILON * EPSILON) 
		return {};

	if (maxLength * maxLength >= lengthSqr)
		return *this;

	return *this * (maxLength / std::sqrt(lengthSqr));
}

Vector2& Vector2::Rotate(float radians)
{
	const float cos{ std::cos(radians) };
	const float sin{ std::sin(radians) };
	const float oldX{ x };

	x = oldX * cos - y * sin;
	y = oldX * sin + y * cos;

	return *this;
}

Vector2 Vector2::Rotated(float radians) const
{
	const float cos{ std::cos(radians) };
	const float sin{ std::sin(radians) };
	return { x * cos - y * sin, x * sin + y * cos };
}

float Vector2::Angle() const
{
	return std::atan2(y, x);
}

bool Vector2::IsZero(float epsilon) const
{
	return LengthSqr() < epsilon * epsilon;
}

bool Vector2::NearlyEqual(const Vector2& other, float epsilon) const
{
	return std::abs(x - other.x) < epsilon
		&& std::abs(y - other.y) < epsilon;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2{ x + other.x, y + other.y };
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2{ x - other.x, y - other.y };
}

float Vector2::operator*(const Vector2& other) const
{
	return x * other.x + y * other.y;
}

Vector2 Vector2::operator-() const
{
	return { -x, -y };
}

Vector2 Vector2::operator*(float scalar) const
{
	return { x * scalar, y * scalar };
}

Vector2 Vector2::operator/(float divider) const
{
	assert(std::abs(divider) > EPSILON && "Vector2 division by zero");

	return { x / divider, y / divider };
}

Vector2 mk::operator*(float scalar, const Vector2& vec)
{
	return vec * scalar;
}

Vector2 mk::operator/(float divider, const Vector2& vec)
{
	return vec / divider;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
	return std::abs(x - other.x) < EPSILON
		&& std::abs(y - other.y) < EPSILON;
}