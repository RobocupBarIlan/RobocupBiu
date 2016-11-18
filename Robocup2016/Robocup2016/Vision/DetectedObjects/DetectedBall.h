#ifndef DETECTEDBALL_H_
#define DETECTEDBALL_H_

#include "../DetectedObjects/DetectedObject.h"
#include "BallDistanceCalculator.h"

class DetectedBall : public DetectedObject
{
public:
	float Radius;

	DetectedBall();
	DetectedBall(Point2f center, float radius, float distance);
	~DetectedBall();

	virtual float Distance();
	virtual void Draw(Mat& image);

protected:
	float m_Distance;
	virtual void PrintDetailsOnImage(Mat image);
};

#endif /* DETECTEDBALL_H_ */
