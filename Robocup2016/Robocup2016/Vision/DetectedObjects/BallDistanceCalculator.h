/*
 * BallDistanceCalculator.h
 *
 *  Created on: May 4, 2016
 *      Author: root
 */

#ifndef VISION_DETECTEDOBJECTS_BALLDISTANCECALCULATOR_H_
#define VISION_DETECTEDOBJECTS_BALLDISTANCECALCULATOR_H_

#include "../../../DistanceCalculator/DatabaseWrapper.h"
#include "../VisionUtils.h"

class BallDistanceCalculator {
public:
	BallDistanceCalculator();
	virtual ~BallDistanceCalculator();

	int CalculateDistance(Point2f ballLocation, float radius, int headTilt);

private:
	vector<string> FindFilesInFolder(const char* folderPath);

	map<int,DatabaseWrapper*> m_tiltToDatabase;
	int FindClosestTiltInMap(int headTilt);
	int FindClosestLargerTiltInMap(int headTilt);
	int FindClosestSmallerTiltInMap(int headTilt);
};

#endif /* VISION_DETECTEDOBJECTS_BALLDISTANCECALCULATOR_H_ */
