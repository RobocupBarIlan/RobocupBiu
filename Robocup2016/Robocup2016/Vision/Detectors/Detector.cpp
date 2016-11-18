#include "GateDetector.h"

Detector::Detector()
{

}

Detector::~Detector()
{

}

DetectedObject* Detector::Get()
{
	return m_sharedMemory.SafeRead();
}

DetectedObject* Detector::Detect(Mat& inputImage, bool writeToMemory, bool draw)
{
	Mat currentFrameHSV;
	cvtColor(inputImage, currentFrameHSV, CV_BGR2HSV);

	// The detection works with HSV image, while the debug drawing works with BGR image.
	DetectedObject* detectedObject = DetectObject(currentFrameHSV);

	if (draw)
	{
		detectedObject->Draw(inputImage);
	}

	if (writeToMemory)
	{
		m_sharedMemory.SafeWrite(detectedObject);
	}

	return detectedObject;
}
