#ifndef _REPORT_HEADER_
#define _REPORT_HEADER_

#include "PointOfInterest.h"

struct POIReport
{
	POIReport(float _dis=0,float _angle=0, PointOfInterest::POItypes _type= PointOfInterest::GENERAL_SHAPE):distance(_dis),angle(_angle),type(_type){}
	float distance;
	float angle;
	PointOfInterest::POItypes  type;
};

#endif