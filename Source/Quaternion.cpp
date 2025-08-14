#include "Quaternion.h"
//#define _USE_MATH_DEFINES // For M_PI_4
//#include <math.h>
//#include <cmath>
//
//Quaternion::Quaternion(float X, float Y, float Z, float W)
//
//
//
//
//void Quaternion::RotateVector(std::vector<float>& v)
//{
//    std::vector<float> vector;
//    float num12 = x + x;
//    float num2 = y + y;
//    float num = z + z;
//    float num11 = w * num12;
//    float num10 = w * num2;
//    float num9 = w * num;
//    float num8 = x * num12;
//    float num7 = x * num2;
//    float num6 = x * num;
//    float num5 = y * num2;
//    float num4 = y * num;
//    float num3 = z * num;
//    float num15 = ((v[0] * ((1.0f - num5) - num3)) + (v[1] * (num7 - num9))) + (v[2] * (num6 + num10));
//    float num14 = ((v[0] * (num7 + num9)) + (v[1] * ((1.0f - num8) - num3))) + (v[2] * (num4 - num11));
//    float num13 = ((v[0] * (num6 - num10)) + (v[1] * (num4 + num11))) + (v[2] * ((1.0f - num8) - num5));
//    v[0] = num15;
//    v[1] = num14;
//    v[2] = num13;
//}