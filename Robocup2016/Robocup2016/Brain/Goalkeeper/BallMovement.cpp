/*
 * BallMovement.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "BallMovement.h"

BallMovement::BallMovement(JumpDirection direction, double msToJump)
	: Direction(direction), MsToJump(msToJump), DetectionStatus(Status::Valid)
{
}

BallMovement::BallMovement()
	: Direction(JumpDirection::NONE), MsToJump(-1), DetectionStatus(Status::NotEnoughSamples)
{

}

BallMovement::BallMovement(Status status)
	: Direction(JumpDirection::NONE), MsToJump(-1), DetectionStatus(status)
{

}

void BallMovement::PrintDetailsOnImage(Mat image, Point startingPoint)
{
	char message[256];
	PrintMessageOnImage(image, "Ball Movement details:", startingPoint);

	sprintf(message, "Direction: %s", JumpDirectionStrings[Direction]);
	PrintMessageOnImage(image, message, Point(startingPoint.x, startingPoint.y + 20));

	sprintf(message, "Time to jump: %f", MsToJump);
	cout << message << endl;
	if (0 < MsToJump && MsToJump < 1000)
	{
		cout << "JUMP!!" << endl;
	}
	PrintMessageOnImage(image, message, Point(startingPoint.x, startingPoint.y + 40));

	sprintf(message, "Status: %s", StatusStrings[DetectionStatus]);
	PrintMessageOnImage(image, message, Point(startingPoint.x, startingPoint.y + 60));
}
