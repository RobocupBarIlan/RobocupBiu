/*
 * DetectedLine.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#include "DetectedLine.h"

DetectedLine ::DetectedLine()
		:DetectedObject(UnknownLocation, false)
{
	DetectedLineType = LineType_None;
}

DetectedLine::DetectedLine(Point2f location, LineType lineType)
	:DetectedObject(location, true)
{
	DetectedLineType = lineType;
}



DetectedLine::~DetectedLine() {
	// TODO Auto-generated destructor stub
}

void DetectedLine::Draw(Mat& image)
{

}

float DetectedLine::Distance()
{
	throw VisionException("Distance is not implemented for Gate");
}
