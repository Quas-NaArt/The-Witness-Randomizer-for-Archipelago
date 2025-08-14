#pragma once
#include <array>
#include <cmath>
#include <numbers>

struct Quaternion {
	float x;
	float y;
    float z;
	float w;

    /*constexpr*/ Quaternion(float yaw, float pitch, float roll) {
        // STD trigonometric functions not constexpr until 2026
        pitch *= std::numbers::pi / 180.0f;
        yaw *= std::numbers::pi / 180.0f;
        roll *= std::numbers::pi / 180.0f;
        x = std::sin(roll / 2) * std::cos(pitch / 2) * std::cos(yaw / 2) - std::cos(roll / 2) * std::sin(pitch / 2) * std::sin(yaw / 2);
        y = std::cos(roll / 2) * std::sin(pitch / 2) * std::cos(yaw / 2) + std::sin(roll / 2) * std::cos(pitch / 2) * std::sin(yaw / 2);
        z = std::cos(roll / 2) * std::cos(pitch / 2) * std::sin(yaw / 2) - std::sin(roll / 2) * std::sin(pitch / 2) * std::cos(yaw / 2);
        w = std::cos(roll / 2) * std::cos(pitch / 2) * std::cos(yaw / 2) + std::sin(roll / 2) * std::sin(pitch / 2) * std::sin(yaw / 2);
    };
    constexpr Quaternion(float x, float y, float z, float w) :
        x{x}, y{y}, z{z}, w{w}
        { /*No extra processing*/ };

    constexpr Quaternion() : Quaternion(0, 0, 0, 0)
    {}

    // Basic operations
    /*constexpr*/ [[nodiscard]] double Length() const {
        // std::sqrt not constexpr until 2026
        return std::sqrt(w * w + x * x + y * y + z * z);
    };
    /*constexpr*/ Quaternion Normalize() {
        // std::sqrt not constexpr until 2026
        float length = (float)Length();
        w /= length;
        x /= length;
        y /= length;
        z /= length;
        return *this;
    };

    // Complex operations
    constexpr [[nodiscard]] Quaternion Mul(const Quaternion& other) const {
        return Quaternion{
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w
        };
    };
    constexpr [[nodiscard]] std::array<float, 3> Rotate3dArray(std::array<float, 3> array) const {

        float num12 = x + x;
        float num2 = y + y;
        float num = z + z;
        float num11 = w * num12;
        float num10 = w * num2;
        float num9 = w * num;
        float num8 = x * num12;
        float num7 = x * num2;
        float num6 = x * num;
        float num5 = y * num2;
        float num4 = y * num;
        float num3 = z * num;
        float num15 = ((array[0] * ((1.0f - num5) - num3)) + (array[1] * (num7 - num9))) + (array[2] * (num6 + num10));
        float num14 = ((array[0] * (num7 + num9)) + (array[1] * ((1.0f - num8) - num3))) + (array[2] * (num4 - num11));
        float num13 = ((array[0] * (num6 - num10)) + (array[1] * (num4 + num11))) + (array[2] * ((1.0f - num8) - num5));
        array[0] = num15;
        array[1] = num14;
        array[2] = num13;
        return array;
    };
};
