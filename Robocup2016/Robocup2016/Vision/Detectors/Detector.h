#include "../../Common/SharedMemory.h"
#include "../DetectedObjects/DetectedObject.h"
#include "../VisionUtils.h"

#pragma once

using namespace cv;

class Detector
{
public:
	Detector();
	virtual ~Detector();

	virtual DetectedObject* Detect(Mat& inputImage, bool writeToMemory = true, bool draw = true);

	DetectedObject* Get();

protected:
	virtual DetectedObject* DetectObject(Mat& inputImage) = 0;

private:
	SharedMemory<DetectedObject*> m_sharedMemory;
};




