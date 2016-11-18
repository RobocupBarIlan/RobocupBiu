/*
 * TimedDetectedBall.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef VISION_DETECTEDOBJECTS_TIMEDDETECTEDBALL_H_
#define VISION_DETECTEDOBJECTS_TIMEDDETECTEDBALL_H_

#include "DetectedBall.h"

class TimedDetectedBall : public DetectedBall
{
public:
	TimedDetectedBall();
	TimedDetectedBall(DetectedBall& detectedBall);
	virtual ~TimedDetectedBall();

	double DetectionTime;

	virtual void PrintDetailsOnImage(Mat image);
};

#endif /* VISION_DETECTEDOBJECTS_TIMEDDETECTEDBALL_H_ */
