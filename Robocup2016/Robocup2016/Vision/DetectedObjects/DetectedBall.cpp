/*
 * DetectedBall.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: laptopyellow
 */

#include "../DetectedObjects/DetectedBall.h"

DetectedBall::DetectedBall()
	:DetectedObject(UnknownLocation, false)
{
	Radius = 0;
	m_Distance = 0;
}

DetectedBall::DetectedBall(Point2f center, float radius, float distance)
	:DetectedObject(center, true)
{
	Radius = radius;
	m_Distance = distance;
}

DetectedBall::~DetectedBall()
{
}

void DetectedBall::Draw(Mat& image)
{
	circle(image,Location, Radius, Colors::Blue, 3);
	circle(image,Location, 1, Colors::Red, 4);
	PrintDetailsOnImage(image);
}

void DetectedBall::PrintDetailsOnImage(Mat image)
{
	char message[256];
	PrintMessageOnImage(image, "Ball details:", Point(0,130));

	sprintf(message, "IsDetected: %s", (IsDetected ? "true" : "false"));
	PrintMessageOnImage(image, message, Point(0,150));

	sprintf(message, "Location: (%.3f, %.3f)", Location.x, Location.y);
	PrintMessageOnImage(image, message, Point(0,170));

	sprintf(message, "Distance: %.2f", Distance());
	PrintMessageOnImage(image, message, Point(0,190));
}

float DetectedBall::Distance()
{
	return m_Distance;
}

