/*
 * DetectedLine.h
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#ifndef DETECTEDLINE_H
#define DETECTEDLINE_H

#include "../DetectedObjects/DetectedObject.h"
#pragma once

enum LineType
{
	LineType_MidCircle,
	LineType_Side,
	LineType_None
};

class DetectedLine : public DetectedObject
{
public:
	DetectedLine();
	DetectedLine(Point2f location, LineType lineType);
	~DetectedLine();

	virtual float Distance();
	virtual void Draw(Mat& image);

	LineType DetectedLineType;
};

#endif /* DETECTEDLINE_H */
