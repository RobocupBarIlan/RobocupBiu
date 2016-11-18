#include "../DetectedObjects/DetectedGoal.h"
#include "Detector.h"

using namespace cv;

class GateDetector : public Detector
{
public:
	GateDetector();
	~GateDetector();
	virtual DetectedObject* DetectObject(Mat& inputImage);

	static inline DetectedObject* GetDefault() {return new DetectedGoal();}

private:
	Calibration* m_calibration;

	int CountAndDrawWhitePixels(Mat& BWImage, Mat&outputImageToDraw, float top, float buttom, float left, float right);
	RotatedRect FindLargestCandidate(vector<RotatedRect>& postCandidates);
	void GetVerticalObjects(Mat &BWImage);
	void GetHorizontalObjects(Mat &BWImage);
	bool IsPartiallyOnField(Mat& field, Point2f bottomPoint, Point2f centerPoint);
	bool IsAngleStraight(float angle);
	void FindPostCandidates(Mat& field, vector<RotatedRect>& minRect, vector<RotatedRect>& postCandidates);

	bool FindGKColor(Mat& image, RotatedRect post1, RotatedRect post2);
	bool IsOurGKColor(Vec3b pixel);
	bool IsOppGKColor(Vec3b pixel);
};




