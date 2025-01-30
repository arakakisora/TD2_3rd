#pragma once
#include <cmath>

/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    float Length() const {
        return std::sqrt(x * x + y * y);
    }
};