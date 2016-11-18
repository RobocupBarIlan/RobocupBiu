#include "VisionUtils.h"

namespace Colors
{
	Scalar Blue = Scalar(255, 0, 0);
	Scalar Red = Scalar(0, 0, 255);
	Scalar Green = Scalar(0, 255, 0);
	Scalar Purple = Scalar(255, 0, 255);
}

void ImageShowOnDebug(const string& state, const string& category, Mat image)
{
	if (DEBUG_MODE == 1)
	{
		string winName = category + " - " + state;
		namedWindow(winName, CV_WINDOW_NORMAL);
		imshow(winName, image);
	}
}

void DrawCircle(Mat &image, Point2d &point, Scalar color, int thickness)
{
	circle(image, point, 10, color, 1);
}

void DrawRectangle(Mat &image, RotatedRect &rect, Scalar color, int thickness)
{
	Point2f rect_points[4];
	rect.points(rect_points);
	for (int j = 0; j < 4; j++)
	{
		line(image, rect_points[j], rect_points[(j + 1) % 4], color, thickness);
	}
}

bool FindField(Mat& inputImage, Mat& field)
{
	Mat onlyGreenImage;
	inRange(inputImage, Calibration::GetInstance()->MinGreenHSV, Calibration::GetInstance()->MaxGreenHSV, onlyGreenImage);
	ImageShowOnDebug("OnlyGreenImage", "FindField", onlyGreenImage);

	vector< vector<Point> > contoursVector = FindContours(onlyGreenImage, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat binaryThresholdShapes;
	threshold(onlyGreenImage, binaryThresholdShapes, 255, 0, THRESH_BINARY); // Make shapes black 1U image.
	field = binaryThresholdShapes.clone(); // Clone (copy) blank image with same Dimention (Width,Height);

	if (contoursVector.size() == 0)
	{
		return false;
	}

	int maxindex = -1; // Find the max shape by area = the green field.
	int maxArea = -1, currentArea;
	for (size_t i = 0; i < contoursVector.size(); i++)
	{
		if ((currentArea = contourArea(contoursVector[i], false)) > maxArea)
		{
			maxindex = i;
			maxArea = currentArea;
		}
	}

	if (maxindex > -1)   // Found field
	{
		//  Find Convex of field:
		vector<vector<Point> > hull;
		hull.push_back(vector<Point>());

		convexHull(contoursVector[maxindex], hull[0], true, true);
		drawContours(field, hull, 0, Scalar(255, 255, 255), -1, 4);
		ImageShowOnDebug("Field", "FindField", field);
		return true;
	}

	return false;
}

void GetWhiteImage(Mat& hsvImage, Mat& onlyWhiteImage)
{
	inRange(hsvImage, Calibration::GetInstance()->MinWhiteHSV, Calibration::GetInstance()->MaxWhiteHSV, onlyWhiteImage);
	ImageShowOnDebug("OnlyWhiteImage", "GetWhiteImage", onlyWhiteImage);
}

void CloseImage(Mat& inputImage, Mat& closedImage)
{
	Mat elementG2 = getStructuringElement(MORPH_RECT, Size(5, 5));
	erode(inputImage, closedImage, elementG2);

	Mat elementG1 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2));
	dilate(inputImage, closedImage, elementG1);
}

vector< vector<Point> > FindContours(Mat inputImage, int mode, int method)
{
	vector< vector<Point> > contoursVector;

	// findContours changes the input mat so we use a copy.
	Mat copyForContours = inputImage.clone();
	findContours(copyForContours, contoursVector, mode,	// Only external shapes. (not one inside other)
			method);			// how much point data about shape (From API )
	copyForContours.release();

	return contoursVector;
}

void PrintMessageOnImage(Mat &src, const char* message, Point location, double fontScale)
{
	putText(src, message, location, Font_Type, fontScale, Font_Colour, 2); //Display the text in image window
}

double GetCurrentTime()
{
	timeval time;
	gettimeofday(&time, 0);
	return time.tv_sec + time.tv_usec/(double)1000000 - 1466622800;
}
