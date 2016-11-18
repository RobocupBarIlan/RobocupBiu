/*
 * BallMovement.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_BALLMOVEMENT_H_
#define BRAIN_GOALKEEPER_BALLMOVEMENT_H_

#include "../../Vision/VisionUtils.h"

enum JumpDirection {
	NONE,
	LEFT,
	RIGHT,
	CENTER
};

static const char* JumpDirectionStrings[] = { "None", "Left", "Right", "Center" };

enum Status {
	Valid,
	BallNotDetected,
	InvalidTimeDiff,
	InvalidLocationDiff,
	NotEnoughSamples
};

static const char* StatusStrings[] = { "Valid", "BallNotDetected", "InvalidTimeDiff", "InvalidLocationDiff", "NotEnoughSamples" };

class BallMovement
{
public:
	BallMovement();
	BallMovement(Status status);
	BallMovement(JumpDirection direction, double msToJump);
	void PrintDetailsOnImage(Mat image, Point startingPoint);

	JumpDirection Direction;
	double MsToJump;
	Status DetectionStatus;

	static BallMovement BallNotDetected()
	{
		BallMovement ballMovement(Status::BallNotDetected);
		return ballMovement;
	}

	static BallMovement InvalidTimeDiff()
	{
		BallMovement ballMovement(Status::InvalidTimeDiff);
		return ballMovement;
	}

	static BallMovement InvalidLocationDiff()
	{
		BallMovement ballMovement(Status::InvalidLocationDiff);
		return ballMovement;
	}

	static BallMovement NotEnoughSamples()
	{
		BallMovement ballMovement(Status::NotEnoughSamples);
		return ballMovement;
	}
};

#endif /* BRAIN_GOALKEEPER_BALLMOVEMENT_H_ */
