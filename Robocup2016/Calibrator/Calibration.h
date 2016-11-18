/*
 * Calibration.h
 *
 *  Created on: Jun 15, 2016
 *      Author: root
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Includes.h"
#include "Constants.h"

#define WHITE_MIN "White Min HSV"
#define WHITE_MAX "White Max HSV"

#define GREEN_MIN "Green Min HSV"
#define GREEN_MAX "Green Max HSV"

#define OPPOSITE_GOALKEEPER_MIN "Opposite GK Min HSV"
#define OPPOSITE_GOALKEEPER_MAX "Opposite GK Max HSV"

#define OUR_GOALKEEPER_MIN "Our GK Min HSV"
#define OUR_GOALKEEPER_MAX "Our GK Max HSV"

static char* CalibrationFilePath = "/home/robot/src/RobocupBIU/RoboCup2016/RoboCup2016/calibration.txt";

class Calibration {
public:
	// Singletone
	static Calibration* GetInstance();

	Scalar MinWhiteHSV;
	Scalar MaxWhiteHSV;

	Scalar MinGreenHSV;
	Scalar MaxGreenHSV;

	Scalar MinOurGKHSV;
	Scalar MaxOurGKHSV;

	Scalar MinOppGKHSV;
	Scalar MaxOppGKHSV;

	static Calibration* ReadFromFile();
	void WriteCalibrationToFile();

	bool IsWhitePixel(Vec3b pixel);
	bool IsGreenPixel(Vec3b pixel);
	bool IsOurGKPixel(Vec3b pixel);
	bool IsOppGKPixel(Vec3b pixel);
	bool IsInPixelRange(Vec3b pixel, Scalar minRange, Scalar maxRange);

private:
	static Calibration* m_instance;
	Calibration();

	static void ParseLineFromCalibrationFile(string line);
	static Scalar ParseScalar(string scalarString);
	static string ScalarToString(string scalarName, Scalar scalar);
};

#endif /* CALIBRATION_H_ */
