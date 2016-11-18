/*
 * BallMovementCalculator.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_
#define BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_

#define MAX_TIME_DIFF 1000000
#define MIN_TIME_DIFF 500
#define MIN_LOCATION_DIFF 10
#define CENTER_MARGIN 50
#define CROSSING_PIXEL FRAME_HEIGHT + 100

#include "BallMovement.h"
#include "../../Vision/DetectedObjects/TimedDetectedBall.h"
#include "LeastSquareApproximation/ParabolaApprox.h"
#include "LeastSquareApproximation/LineApprox.h"

class BallMovementCalculator {
public:
	BallMovementCalculator();
	virtual ~BallMovementCalculator();

	BallMovement CalculateBallMovement(TimedDetectedBall timedDetectedBall);
	void ResetSamples();

private:
	void PushDetectedBallToQueue(TimedDetectedBall timedDetectedBall);
	JumpDirection CalculateDirection(LeastSquareApprox* directionApprox);
	double CalculateJumpingTime(LeastSquareApprox* timingApprox);

	bool IsNewSampleRelevant(TimedDetectedBall timedDetectedBall);
	bool IsTimeDiffValid();
	bool IsLocationDiffValid();
	bool IsEnoughSamples();

	vector<TimedDetectedBall> GetRelevantSamples();

	double GetRelativeTimeToFirstSample(double detectionTime);

	TimedDetectedBall m_SamplesArray[NUM_OF_SAMPLES];

	vector<double> GetYValues(vector<TimedDetectedBall> samples);
	vector<double> GetXValues(vector<TimedDetectedBall> samples);
	vector<double> GetTimes(vector<TimedDetectedBall> samples);

	void PrintCalculationData(Mat& image, vector<TimedDetectedBall> relevantSamples);
};

#endif /* BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_ */
