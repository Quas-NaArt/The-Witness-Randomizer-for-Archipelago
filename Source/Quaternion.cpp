#include "Quaternion.h"
#define _USE_MATH_DEFINES // For M_PI_4
#include <math.h>
#include <cmath>

Quaternion::Quaternion(float X, float Y, float Z, float W)
{
    x = X; y = Y; z = Z; w = W;
}

Quaternion::Quaternion(float pitch, float yaw, float roll)
{
    pitch *= M_PI / 180.0f; yaw *= M_PI / 180.0f; roll *= M_PI / 180.0f;
    x = std::sin(roll / 2) * std::cos(pitch / 2) * std::cos(yaw / 2) - std::cos(roll / 2) * std::sin(pitch / 2) * std::sin(yaw / 2);
    y = std::cos(roll / 2) * std::sin(pitch / 2) * std::cos(yaw / 2) + std::sin(roll / 2) * std::cos(pitch / 2) * std::sin(yaw / 2);
    z = std::cos(roll / 2) * std::cos(pitch / 2) * std::sin(yaw / 2) - std::sin(roll / 2) * std::sin(pitch / 2) * std::cos(yaw / 2);
    w = std::cos(roll / 2) * std::cos(pitch / 2) * std::cos(yaw / 2) + std::sin(roll / 2) * std::sin(pitch / 2) * std::sin(yaw / 2);
}

double Quaternion::Length() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::Normalize() {
    float length = (float)Length();
    w /= length;
    x /= length;
    y /= length;
    z /= length;
    return *this;
}

Quaternion Quaternion::Mul(const Quaternion& other) const {
    return Quaternion{
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    };
}

void Quaternion::RotateVector(std::vector<float>& v)
{
    std::vector<float> vector;
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
    float num15 = ((v[0] * ((1.0f - num5) - num3)) + (v[1] * (num7 - num9))) + (v[2] * (num6 + num10));
    float num14 = ((v[0] * (num7 + num9)) + (v[1] * ((1.0f - num8) - num3))) + (v[2] * (num4 - num11));
    float num13 = ((v[0] * (num6 - num10)) + (v[1] * (num4 + num11))) + (v[2] * ((1.0f - num8) - num5));
    v[0] = num15;
    v[1] = num14;
    v[2] = num13;
}