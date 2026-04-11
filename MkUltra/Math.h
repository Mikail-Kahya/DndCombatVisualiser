#pragma once

namespace mk {
	
	struct Vector2
	{
		static float Angle(const Vector2& a, const Vector2& b);
		static float Dot(const Vector2& a, const Vector2& b);
		static float Cross(const Vector2& a, const Vector2& b);

		static Vector2 Displacement(const Vector2& a, const Vector2& b);

		float Length() const;
		float LengthSqr() const;
		float DotAngle() const;
		float CrossAngle() const;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		float operator*(const Vector2& other) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);

		float x{};
		float y{};
		float z{};
	};

};