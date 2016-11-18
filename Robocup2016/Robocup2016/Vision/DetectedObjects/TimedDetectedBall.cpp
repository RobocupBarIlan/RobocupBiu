/*
 * TimedDetectedBall.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "TimedDetectedBall.h"

TimedDetectedBall::TimedDetectedBall()
	:DetectedBall()
{
	DetectionTime = GetCurrentTime();
}

TimedDetectedBall::TimedDetectedBall(DetectedBall& detectedBall)
	: DetectedBall(detectedBall)
{
	DetectionTime = GetCurrentTime();
}

TimedDetectedBall::~TimedDetectedBall()
{
	// TODO Auto-generated destructor stub
}

void TimedDetectedBall::PrintDetailsOnImage(Mat image)
{
	char message[256];
	DetectedBall::PrintDetailsOnImage(image);
	sprintf(message, "Detection time: %f", DetectionTime);
	PrintMessageOnImage(image, message, Point(0,210));

}
