/*
 * DistanceDBBuilder.h
 *
 *  Created on: May 11, 2016
 *      Author: root
 */

#ifndef DISTANCEDBBUILDER_H_
#define DISTANCEDBBUILDER_H_

#include "Includes.h"
#include "Constants.h"
#include "../Calibrator/Calibration.h"
#include <numeric>
#include "DistanceDBConstants.h"
#include "DatabaseWrapper.h"

class DistanceDBBuilder
{
public:
	DistanceDBBuilder();
	virtual ~DistanceDBBuilder();

	void CreateDatabaseForTilt(Mat whiteImage, int angle);


private:
	string intToString(int number);
	string GetDatabaseFilePath(int angle);

	map<int, int> CreateDatabse(Mat whiteImage, int zeroPixelDistance);
	void WriteDatabaseToFile(map<int,int> database, string filePath);

	DatabaseWrapper* m_angleToZeroPixelDistance;
};

#endif /* DISTANCEDBBUILDER_H_ */
