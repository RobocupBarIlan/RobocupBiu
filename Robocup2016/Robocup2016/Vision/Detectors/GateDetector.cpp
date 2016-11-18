#include "GateDetector.h"

GateDetector::GateDetector()
{
	m_calibration = Calibration::GetInstance();
}

GateDetector::~GateDetector()
{

}

/*****************************************************************************************
* Method Name: FindGate
* Description: The function search for a couple of white vertical rectangles, whose buttom points
* 				are on green && upper part is not on green.
* 				This rectangles are marked as gate posts.
* 				If only one post is present in the frame, the function tries to find the direction
* 				of the second vertical post, using the upper post direction.
* Arguments: BWImage, Output - DetectedGate
* Return Values: DetectedGate object
* Owner: Hen Shoob, Assaf Rabinowitz
*****************************************************************************************/
DetectedObject* GateDetector::DetectObject(Mat& inputImageHSV)
{
	Mat onlyWhiteImage;
	GetWhiteImage(inputImageHSV, onlyWhiteImage);

	medianBlur(onlyWhiteImage, onlyWhiteImage, 15);

	Mat whiteVertical = onlyWhiteImage.clone();
	GetVerticalObjects(whiteVertical);
	// Matrix for debug drawing
	Mat drawingMat;
	cvtColor(whiteVertical, drawingMat, CV_GRAY2BGR);
	ImageShowOnDebug("Vertical", "GoalDetector", whiteVertical);
	//creating Mat thr with contours, using canny
	Mat canny;
	Canny(whiteVertical, canny, 50, 190, 3, false);
	//crating contours vector
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(canny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat field;
	FindField(inputImageHSV, field);
	//Showing the contours + field in one image
	Mat fieldAndContours;
	bitwise_or(field, canny, fieldAndContours);
	cvtColor(fieldAndContours, fieldAndContours, CV_GRAY2BGR);
	//finding the blocking rectangles of the contours
		vector<RotatedRect> minRect(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			minRect[i] = minAreaRect(Mat(contours[i]));
			DrawRectangle(fieldAndContours, minRect[i]);
		}

	ImageShowOnDebug("Field and Contours", "GateDetector", fieldAndContours);
	vector<RotatedRect> post1Candidates;
	FindPostCandidates(field, minRect, post1Candidates);

	RotatedRect post1 = FindLargestCandidate(post1Candidates);
	DrawRectangle(drawingMat, post1);
	ImageShowOnDebug("Post1", "GateDetector", drawingMat);
	vector<RotatedRect> post2Candidates;
	for (size_t i = 0; i < post1Candidates.size(); i++)
	{
		if (post1Candidates[i].center.x != post1.center.x)
		{
			post2Candidates.push_back(post1Candidates[i]);
		}
	}

	RotatedRect post2 = FindLargestCandidate(post2Candidates);
	DrawRectangle(drawingMat, post2);
	ImageShowOnDebug("Post2", "GateDetector", drawingMat);

	//////////////////////////////////////////////////////////////////////////////////////////

	if (post1.size.area() > 0 && post2.size.area() > 0)
	{
		RotatedRect leftPost = (post1.center.x < post2.center.x) ? post1 : post2;
		RotatedRect rightPost = (post1.center.x > post2.center.x) ? post1 : post2;
		bool isOurGoal = FindGKColor(inputImageHSV, leftPost, rightPost);
		return new DetectedGoal(leftPost, rightPost, isOurGoal);
	}

	if (post1.size.area() == 0 && post2.size.area() == 0)
	{
		return new DetectedGoal();
	}

	// One post - detect which.
	float longSide = MAX(post1.size.height ,post1.size.width);
	float shortSide = MIN(post1.size.height ,post1.size.width);

	Mat whiteHorizontal = onlyWhiteImage.clone();
	GetHorizontalObjects(whiteHorizontal);
	ImageShowOnDebug("WhiteHorizontal", "GateDetector", whiteHorizontal);

	//counting white pixels near the top of the post on both sides
	// We set the range to look for white pixels in the upper post.
	float upperPostTop = MAX(post1.center.y - longSide / 2 - 10, 0);
	float upperPostButtom = upperPostTop + shortSide + 5;	

	//Counting near the left post
	float upperPostLeft = MAX(post1.center.x - shortSide * 3, 0);
	float upperPostRight = MAX(post1.center.x - shortSide, 0);
	unsigned int leftCounter = CountAndDrawWhitePixels(whiteHorizontal, drawingMat, upperPostTop, upperPostButtom, upperPostLeft, upperPostRight);

	//finding right check rectangle, left and right side
	upperPostRight = MIN(post1.center.x + shortSide * 3, FRAME_WIDTH - 1);
	upperPostLeft = MIN(post1.center.x + shortSide, FRAME_WIDTH - 1);
	unsigned int rightCounter = CountAndDrawWhitePixels(whiteHorizontal, drawingMat, upperPostTop, upperPostButtom, upperPostLeft, upperPostRight);
	
	if (rightCounter > leftCounter  &&  rightCounter - leftCounter > 5)
	{
		return new DetectedGoal(post1, POST_LEFT, false);
	}
	else if (rightCounter < leftCounter  &&  leftCounter - rightCounter > 5)
	{
		return new DetectedGoal(post1, POST_RIGHT, false);
	}
	else
	{
		return new DetectedGoal();
	}	
}

bool GateDetector::FindGKColor(Mat& image, RotatedRect leftPost, RotatedRect rightPost)
{
	float leftPostLongEdge = MAX(leftPost.size.height, leftPost.size.width);
	float rightPostLongEdge = MAX(rightPost.size.height, rightPost.size.width);

	int top = MIN(leftPost.center.y - leftPostLongEdge/2, rightPost.center.y - rightPostLongEdge/2);
	int buttom = MAX(leftPost.center.y + leftPostLongEdge/2, rightPost.center.y + rightPostLongEdge/2);

	int ourGKCount = 0;
	int oppositeGKCount = 0;

	for (int row = top ; row <= buttom ; row++)
	{
		for (int column = leftPost.center.x ; column <= rightPost.center.x ; column++)
		{
			Vec3b currentPixel = image.at<Vec3b>(column, row);
			if (m_calibration->IsOurGKPixel(currentPixel))
			{
				ourGKCount++;
			}
			else if (m_calibration->IsOppGKPixel(currentPixel))
			{
				oppositeGKCount++;
			}
		}
	}

	return ourGKCount > oppositeGKCount;
}

int GateDetector::CountAndDrawWhitePixels(Mat& BWImage, Mat&outputImageToDraw, float top, float buttom, float left, float right)
{
	int counter = 0;

	for (size_t y = top; y < buttom; y++)
	{
		for (size_t x = left; x < right; x++)
		{
			if (BWImage.at<unsigned char>(y, x) > 100)
			{
				counter++;
				outputImageToDraw.at<Vec3b>(y, x) = Vec3b(255,0,0);
			}
		}
	}

	return counter;
}

//finding the largest rectangle among the qualified
RotatedRect GateDetector::FindLargestCandidate(vector<RotatedRect>& postCandidates)
{
	RotatedRect largestCandidate;
	if (postCandidates.size() > 0)
	{
		Size2f s = postCandidates[0].size;
		size_t pl = 0;
		for (size_t i = 0; i < postCandidates.size(); i++)
		{
			if (postCandidates[i].size.area() > s.area())
			{
				pl = i;
				s = postCandidates[i].size;
			}
		}
		largestCandidate = postCandidates[pl];
		return largestCandidate;
	}
	return largestCandidate;
}

void GateDetector::GetVerticalObjects(Mat &BWImage)
{
	//eroding to remove horizontal lines
	Mat elementG2 = getStructuringElement(MORPH_RECT, Size(1, 41));
	erode(BWImage, BWImage, elementG2);
	erode(BWImage, BWImage, elementG2);

	//dilation of white to fill the shapes
	Mat elementG3 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2));
	dilate(BWImage, BWImage, elementG3);
}

void GateDetector::GetHorizontalObjects(Mat &BWImage)
{
	//dilation of white to fill the shapes
	Mat elementG4 = getStructuringElement(MORPH_RECT, Size(5, 3), Point(2, 2));
	dilate(BWImage, BWImage, elementG4);
}

// Returns whther any point of the 30 pixels than below the post is on the field.
bool GateDetector::IsPartiallyOnField(Mat& field, Point2f bottomPoint, Point2f topPoint)
{
	for (int y = bottomPoint.y ; y < MAX(bottomPoint.y + 30 ,FRAME_HEIGHT); y++)
	{
		Point2f lowerPointThanTheBottomPoint(bottomPoint.x, y);

		if (field.at<unsigned char>(lowerPointThanTheBottomPoint) > 100
				&& field.at<unsigned char>(topPoint) < 100)
		{
			return true;
		}
	}

	return false;
}

bool GateDetector::IsAngleStraight(float angle)
{
	return (angle > -10 && angle < 10) || (100 > angle && angle > 80) || (-100 < angle && angle < -80);
}

void GateDetector::FindPostCandidates(Mat& field, vector<RotatedRect>& minRect, vector<RotatedRect>& postCandidates)
{
	// Check foreach rect:
	// 1. Bottom of the rectangle is close enough to the field.
	// 2. Long side is at least 5 times the short side.
	// 3. Angle of the rectangle is close to 0 or 90.

	for (size_t i = 0; i < minRect.size(); i++)
	{
		float longSide = MAX(minRect[i].size.height, minRect[i].size.width);
		float shortSide = MIN(minRect[i].size.height, minRect[i].size.width);

		Point2f bottomPoint;
		bottomPoint.y = MIN(minRect[i].center.y + longSide / 2 + 5, FRAME_HEIGHT - 1);
		bottomPoint.x = minRect[i].center.x;
		Point2f topPoint;
		topPoint.y = MAX(minRect[i].center.y - longSide / 2 + 5, 0);
		topPoint.x = minRect[i].center.x;

		if ((IsPartiallyOnField(field, bottomPoint, topPoint))
			&& (longSide > 3 * shortSide)
			&& (IsAngleStraight(minRect[i].angle)))
		{
			postCandidates.push_back(minRect[i]);
		}
	}
}
