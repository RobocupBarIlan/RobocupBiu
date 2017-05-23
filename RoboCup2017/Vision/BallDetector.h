#ifndef VISION_DETECTORS_BallDetector_H_
#define VISION_DETECTORS_BallDetector_H_

#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/cvstd.hpp"
#include <string>
#include <math.h>
#include <iostream> //Only for collecting data from algorithm (log files etc...)
#include <sstream>



using namespace cv;
using namespace std;

#define NUM_CHANNELS 3
#define MAX_HUE_VALUE 179   //Hue is in range [0,179].
#define MIN_GREEN_HUE 32 //40   //Heuristic. 32
#define MAX_GREEN_HUE 90 // 70   //Heuristic. 90
#define MIN_SATURATION_FIELD 100 //Heuristic. 120
#define HUE_DISCRETIZATION 180  //Hue is in range [0,179].
#define MAX_BALL_DIAMETER 200  //Heuristic. should be even.
#define MIN_BALL_DIAMETER 16   //Heuristic. should be even. May cause problems with field's line morphological closing performed on field_mat if below 25.
#define MAX_WHITE_SATURATION 110 //Heuristic. saturation range is [0,255]. First try - with 77. (if using gaussian blur on original img -switch to 120)
#define MIN_WHITE_VALUE 120 //Heuristic. value range is [0,255]. First try - with 178 (if using gaussian blur on original img switch to 170).
#define PI 3.14159265358979 //Value of Pi (11 digits after the point). 
#define SUB_IMAGE_HALF_WIDTH (MAX_BALL_DIAMETER-MIN_BALL_DIAMETER/2)//As we crop the image to get a sub-image(square shape) in the algorithm - we define this value to be the maximal needed half width of this image.
#define STRUCTURE_ELEMENT_SIZE 3 //The size of the structure element's matrix.
#define DEG_DISCRETIZATION 361 //Discretization level of degrees.
#define MIN_WHITE_PIXELS_RATIO_IN_BLOB 0.4 //Heuristic. Used to check whether a ball candidate in image has enough white pixels. first the ball is found using shape only and then we use this to validate it has the proper color.
#define CANDIDATE_SUB_IMAGE_CROP_EXTENSION 3 //3 pixels extension from all directions
#define LOGISTIC_REGRESSION_CLASSIFIER_LENGTH 513 //Number of features for logistic regression model.
#define VISUAL_WORDS_BLOCK_SIZE 3 //Defines the number of pixels in a visual block (for the bag of words model we use to check if a shape is like a circle).
class BallDetector{
public:

	BallDetector();
	virtual ~BallDetector();
	static Point GetBallCenter();
	//Function calculates the min hue value of the green field and max hue value of the green field.
	//Takes- hue_matrix as input. Returns field_min_hue and field_max_hue
	static void CalculateDistanceToBall(double radius); //Calculate the distance from the ball using a linear regression model.
	//TODO implement this method ^^^^^^^^
	static double Sigmoid(double x); //This method calculates the sigmoid function of a given real value.
private:
	static void FieldCalibration(Mat& hue_matrix, uchar& field_min_hue, uchar& field_max_hue) ;
	static void CalculateCorrelationToCircleHist(Mat bw_mat,double& correlation);
	static void CalculateBoundingHorizontalLine(Mat& field_mat, ushort& bounding_horizontal_line);

};

#endif /* VISION_DETECTORS_BallDetector_H_ */


