#pragma once
#include <cmath>
double rand_normal_noise();
void WriteLogFile(const char* szString);
float  randomFrom0to1();
float  randomInRange(float a, float b);
//template<typename t>void swap(t& a, t& b);
int binaryRangeFind(float* arr);
float calcRelativeAngle(float particleAndPOIAngleInPlane, float particleAngleInPlan);
float calcDistance2D(float x1, float y1, float x2, float y2);
