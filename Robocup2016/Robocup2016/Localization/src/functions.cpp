#include <cstdlib>
#include "../headers/defines.h"
#include "../headers/functions.h"
#include "Includes.h"

double rand_normal_noise() {
	return 0;
	/*std::default_random_engine generator;
	std::normal_distribution<double> distribution(5.0, 2.0);
	return distribution(generator);*/
}

void WriteLogFile(const char* szString)
{
	ofstream localizationLogFile("LocalizationlogFile.txt");
	if (localizationLogFile.is_open())
	{
		localizationLogFile << szString << endl;
	}
}

float  randomFrom0to1() {
	return ((float)rand() / (float)RAND_MAX);
}

float  randomInRange(float a, float b) {
	return (a + randomFrom0to1()*(b - a));
}

float calcRelativeAngle(float particleAndPOIAngleInPlane, float particleAngleInPlan)
{
	float relativeAngle = particleAndPOIAngleInPlane - particleAngleInPlan;
	while (relativeAngle < -180) relativeAngle += 360;
	while (relativeAngle > 180) relativeAngle -= 360;
	return relativeAngle;
}

float calcDistance2D(float x1, float y1, float x2, float y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
		
}

template<typename t>
void swap(t& a, t& b)
{
	t temp = a;
	a = b;
	b = temp;
}

int binaryRangeFind(float* arr) {
	int steps = 0;
	int start = 0, end = NUM_OF_PARTICLES - 1;

	if (!(end >= start)) return-1; // Empty array

	float target = randomFrom0to1()* arr[NUM_OF_PARTICLES - 1];

	if (target <= arr[0]) return 0;

	if (NUM_OF_PARTICLES - 2 >= 0)
		if (target > arr[NUM_OF_PARTICLES - 2]) return (NUM_OF_PARTICLES - 1);

	while (start < end) {
		steps++;

		int middle = (end - start - (end - start) % 2 + 1) / 2 + start;
		if (target > arr[middle]) {
			start = middle + 1;
		}
		else {
			end = middle;
		}
	}
	return end;
}
