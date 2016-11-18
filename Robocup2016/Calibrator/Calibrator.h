#ifndef CALIBRATOR_H_
#define CALIBRATOR_H_

#include "Includes.h"
#include "Calibration.h"

#define ImageWindow "Image"
#define CalibrationWindow "Calibration"
#define TrackbarName "Calibration type"

void onMouse(int event, int x, int y, int flag, void* param);

enum
{
	White_Calibration,
	Green_Calibration,
	Opposite_GK_Calibration,
	Our_GK_Calibration
};

static const char* CalibrationTypeStrings[] = { "White", "Green", "Opposite GK", "Our GK" };

class Calibrator
{
public:
	// Singletone
	static Calibrator* GetInstance();
	
	void Calibrate();
	void CalibrateGreen(Mat image, Point2i position);
	void CalibrateWhite(Mat image, Point2i position);
	void CalibrateOurGK(Mat image, Point2i position);
	void CalibrateOppGK(Mat image, Point2i position);

	void ResetValuesToClick(Mat image, Point2i position);

	void ReadCalibrationFromFile();

private:
	static Calibrator* m_instance;
	Calibrator();
	void InitializeWindow();

	// Returns the min\max scalar in the area of 3X3 around the given point in the given image.
	Scalar MinAreaScalar(Mat image, Point2i point);
	Scalar MaxAreaScalar(Mat image, Point2i point);
	bool IsInImageBorders(int x, int y);

	void ShowCalibration(Mat image);

	Scalar Min(Scalar a, Scalar b);
	Scalar Max(Scalar a, Scalar b);

	void ParseLineFromCalibrationFile(string line);
	void WriteCalibrationToFile();

	Calibration* m_Calibration;
};

#endif /* CALIBRATOR_H_ */
