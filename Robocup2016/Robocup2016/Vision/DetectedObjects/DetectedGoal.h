#include "DetectedObject.h"

#pragma once

enum Post
{
	POST_NONE,
	POST_LEFT,
	POST_RIGHT,
	POST_BOTH
};

static const char* PostStrings[] = { "None", "Left", "Right", "Both" };

class DetectedGoal : public DetectedObject
{
public:
	RotatedRect LeftPost;
	RotatedRect RightPost;
	Post DetectedPost;
	bool IsOurGoal;

	virtual float Distance();
	virtual void Draw(Mat& image);

	DetectedGoal();
	~DetectedGoal();

	DetectedGoal(RotatedRect leftPost, RotatedRect rightPost, bool isOurGoal);
	DetectedGoal(RotatedRect post, Post detectedPost, bool isOurGoal);

private:
	Point2f GetGoalCenter(RotatedRect post1, RotatedRect post2);
	void PrintDetailsOnImage(Mat image);
};



