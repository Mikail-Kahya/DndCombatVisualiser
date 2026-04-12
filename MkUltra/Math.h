#pragma once

#include <numbers>

namespace mk {

	constexpr float EPSILON{ 1e-5f };
	constexpr float ToDegrees(float radians) { return radians * 180.0f / std::numbers::pi_v<float>; }
	constexpr float ToRadians(float degrees) { return degrees * std::numbers::pi_v<float> / 180.0f;  }
	
	struct Vector2
	{
		static float AngleBetween(const Vector2& a, const Vector2& b); // Returns the angle in radians
		static float Dot(const Vector2& a, const Vector2& b);
		static float Determinant(const Vector2& a, const Vector2& b);
		static float SignedAngle(const Vector2& a, const Vector2& b); // Returns the atan2 signed angle

		static Vector2 Displacement(const Vector2& a, const Vector2& b); // Returns the displacement from Vec1 to Vec2
		static float Distance(const Vector2& a, const Vector2& b); // Returns the distance from Vec1 to Vec2
		static float DistanceSqr(const Vector2& a, const Vector2& b); // Returns the squared distance from Vec1 to Vec2

		static Vector2 Perpendicular(const Vector2& vec);
		static Vector2 Project(const Vector2& a, const Vector2& b); // Returns b projected on a
		static Vector2 Reflect(const Vector2& vec, Vector2 normal); // Returns reflected vector assuming the normal is normalized
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float t); // Lerps between a and b within a range (t) of [0, 1]

		float Length() const; // Uses square root! If possible, prefer LengthSqr.
		float LengthSqr() const;
		Vector2& Normalize(); // Normalizes the vector and returns the vector
		Vector2 Normalized() const; // Returns a copy of the vector normalized. Overuse can affect performance
		Vector2 ClampLength(float maxLength) const; // Returns a copy of the vector with clamped length
		Vector2& Rotate(float radians); // Rotates the vector and returns the vector
		Vector2 Rotated(float radians) const; // Returns a copy of the vector rotated

		float Angle() const; // Returns angle in radians using atan2
		bool IsZero(float epsilon = EPSILON) const;
		bool NearlyEqual(const Vector2& other, float epsilon = EPSILON) const;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		float operator*(const Vector2& other) const;
		Vector2 operator-() const;

		Vector2 operator*(float scalar) const;
		Vector2 operator/(float divider) const;
		friend Vector2 operator*(float scalar, const Vector2& vec);
		friend Vector2 operator/(float divider, const Vector2& vec);

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		bool operator==(const Vector2& other) const;

		float x{};
		float y{};
	};
};