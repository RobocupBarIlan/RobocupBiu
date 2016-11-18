#include "Includes.h"
#include "Constants.h"
#include "../Motion/Motion.h"
#include "../../Calibrator/Calibration.h"

#ifndef VISIONUTILS_H
#define VISIONUTILS_H

#define DEBUG_MODE 1

extern Scalar minWhiteHSV;
extern Scalar maxWhiteHSV;
extern Scalar minGreenHSV;
extern Scalar maxGreenHSV;

// Colors for drawing
namespace Colors
{
	extern Scalar Blue;
	extern Scalar Red;
	extern Scalar Green;
	extern Scalar Purple;
}

struct VisionException : public exception
{
   string message;
   VisionException(string exceptionMessage) : message(exceptionMessage) {}
   ~VisionException() throw () {}

	const char* what() const throw () {
		return message.c_str();
	}
};

bool FindField(Mat& inputImage, Mat& field);
void GetWhiteImage(Mat& inputImage, Mat& onlyWhiteImage);
void CloseImage(Mat& inputImage, Mat& closedImage);
vector< vector<Point> > FindContours(Mat inputImage, int mode, int method);

void ImageShowOnDebug(const string& state, const string& category, Mat image);

void DrawCircle(Mat &image, Point2d &point, Scalar color = Scalar(0,0,255), int thickness = 1);
void DrawRectangle(Mat &image, RotatedRect &rect, Scalar color = Scalar(0,0,255), int thickness = 1);
void PrintMessageOnImage(Mat &src, const char* message, Point location, double fontScale = Font_Scale);

double GetCurrentTime();
#endif
