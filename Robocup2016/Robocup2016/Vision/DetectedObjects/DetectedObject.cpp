/*
 * DetectedObject.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: laptopyellow
 */

#include "../DetectedObjects/DetectedObject.h"

Point2d DetectedObject::UnknownLocation = Point2d(-1,-1);

DetectedObject::DetectedObject(Point2d location, bool isFound)
	:Location(location), IsDetected(isFound)
{
}

DetectedObject::~DetectedObject() {
	// TODO Auto-generated destructor stub
}

