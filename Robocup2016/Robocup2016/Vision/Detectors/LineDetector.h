/*
 * LineDetector.h
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

//#ifndef LINEDETECTOR_H
//#define LINEDETECTOR_H
#include "../DetectedObjects/DetectedLine.h"
#include "Detector.h"
using namespace cv;

class LineDetector: public Detector {
public:
	LineDetector();
	virtual ~LineDetector();
	virtual DetectedObject* DetectObject(Mat& inputImage);
};

//#endif /* LineDetector_H */
