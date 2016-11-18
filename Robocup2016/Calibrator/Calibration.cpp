/*
 * Calibration.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: root
 */

#include "Calibration.h"

Calibration* Calibration::m_instance = NULL;

Calibration::Calibration()
{

}

Calibration* Calibration::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Calibration();
    }
 	return m_instance;
}

Calibration* Calibration::ReadFromFile()
{
	string line;
	ifstream myfile(CalibrationFilePath);

	if (myfile.is_open()) {
		while (getline(myfile, line))
		{
			ParseLineFromCalibrationFile(line);
		}
		myfile.close();
	}

	else {
		cout << "Unable to open color config file";
	}

	return Calibration::GetInstance();
}

void Calibration::ParseLineFromCalibrationFile(string line)
{
	size_t keyEnd = line.find(':');
	string key = line.substr(0, keyEnd);

	size_t valueStart = line.find('[');
	size_t valueEnd = line.find(']');
	string valueString = line.substr(valueStart + 1, valueEnd - valueStart - 1);
	Scalar value = ParseScalar(valueString);

	Calibration* calibration = Calibration::GetInstance();

	if (key == WHITE_MIN)
		calibration->MinWhiteHSV = value;
	if (key == WHITE_MAX)
		calibration->MaxWhiteHSV = value;
	if (key == GREEN_MIN)
		calibration->MinGreenHSV = value;
	if (key == GREEN_MAX)
		calibration->MaxGreenHSV = value;

	if (key == OUR_GOALKEEPER_MIN)
		calibration->MinOurGKHSV = value;
	if (key == OUR_GOALKEEPER_MAX)
		calibration->MaxOurGKHSV = value;
	if (key == OPPOSITE_GOALKEEPER_MIN)
		calibration->MinOppGKHSV = value;
	if (key == OPPOSITE_GOALKEEPER_MAX)
		calibration->MaxOppGKHSV = value;
}

Scalar Calibration::ParseScalar(string scalarString)
{
	vector <int> tokens;
	string delimiter = ",";

	string::size_type lastPos = 0;
	string::size_type pos = scalarString.find(delimiter, lastPos);

	while (string::npos != pos) {
		tokens.push_back(atoi(scalarString.substr(lastPos, pos - lastPos).c_str()));
		lastPos = pos + delimiter.size();
		pos = scalarString.find(delimiter, lastPos);
	}

	tokens.push_back(atoi(scalarString.substr(lastPos, scalarString.size() - lastPos).c_str()));

	Scalar result(tokens[0], tokens[1], tokens[2]);
	return result;
}

void Calibration::WriteCalibrationToFile()
{
	ofstream out(CalibrationFilePath);

	out << ScalarToString(WHITE_MIN, MinWhiteHSV);
	out << ScalarToString(WHITE_MAX, MaxWhiteHSV);
	out << endl;

	out << ScalarToString(GREEN_MIN, MinGreenHSV);
	out << ScalarToString(GREEN_MAX, MaxGreenHSV);
	out << endl;

	out << ScalarToString(OUR_GOALKEEPER_MIN, MinOurGKHSV);
	out << ScalarToString(OUR_GOALKEEPER_MAX, MaxOurGKHSV);
	out << endl;

	out << ScalarToString(OPPOSITE_GOALKEEPER_MIN, MinOppGKHSV);
	out << ScalarToString(OPPOSITE_GOALKEEPER_MAX, MaxOppGKHSV);
	out << endl;

	out.close();
}

string Calibration::ScalarToString(string scalarName, Scalar scalar)
{
	char scalarStr[256];
	sprintf(scalarStr, "%s:[%d,%d,%d]\n", scalarName.c_str(), (int)scalar[0], (int)scalar[1], (int)scalar[2]);
	return string(scalarStr);
}

bool Calibration::IsWhitePixel(Vec3b pixel)
{
	return IsInPixelRange(pixel, MinWhiteHSV, MaxWhiteHSV);
}

bool Calibration::IsGreenPixel(Vec3b pixel)
{
	return IsInPixelRange(pixel, MinGreenHSV, MaxGreenHSV);
}

bool Calibration::IsOurGKPixel(Vec3b pixel)
{
	return IsInPixelRange(pixel, MinOurGKHSV, MaxOurGKHSV);
}

bool Calibration::IsOppGKPixel(Vec3b pixel)
{
	return IsInPixelRange(pixel, MinOppGKHSV, MaxOppGKHSV);
}

bool Calibration::IsInPixelRange(Vec3b pixel, Scalar minRange, Scalar maxRange)
{
	return 	minRange[0] <= pixel[0] <= maxRange[0] &&
			minRange[1] <= pixel[1] <= maxRange[1] &&
			minRange[2] <= pixel[2] <= maxRange[2];
}
