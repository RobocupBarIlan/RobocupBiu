#include "../DetectedObjects/DetectedBall.h"
#include "Detector.h"

#ifndef BALLDETECTOR_H_
#define BALLDETECTOR_H_

static const int DEQUE_SIZE = 10;

class BallDetector : public Detector
{
public:
	BallDetector();
	~BallDetector();

	virtual DetectedObject* DetectObject(Mat& inputImage);
	DetectedObject* geometricBallDetection(Mat& inputImageHSV);
	DetectedObject* probabilisticBallDetection(Mat& inputImageHSV);
	static inline DetectedObject* GetDefault() {return new DetectedBall();}

private:
	float CalculateDistance(Point2f center, float radius);
	BallDistanceCalculator* m_BallDistanceCalculator;
};

#endif /* BALLDETECTOR_H_ */
