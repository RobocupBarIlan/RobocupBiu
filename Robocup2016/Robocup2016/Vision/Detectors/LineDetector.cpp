/*
 * LineDetecnputor.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#include "LineDetector.h"

LineDetector::LineDetector() {
	// TODO Auto-generated constructor stub

}

LineDetector::~LineDetector() {
	// TODO Auto-generated destructor stub
}

DetectedObject* LineDetector::DetectObject(Mat& inputImageHSV)
{
	Mat  cdst, dst, tempImage, src, blackWhiteImgeBGR;
	Mat onlyWhiteImageLineDetector;
	//GetWhiteImage(inputImageHSV, onlyWhiteImageLineDetector);
	GetWhiteImage(inputImageHSV, src);
	Mat elementG4 = getStructuringElement(MORPH_RECT, Size(5, 3), Point(2, 2));
	//dilate(src, src, elementG4);
	imshow("src black&white image", src);
	//waitKey(0);
	//medianBlur(src, src, 15);
	//imshow("src after blur", src);
	//cvtColor(onlyWhiteImageLineDetector, blackWhiteImgeBGR, CV_HSV2BGR);
	//imshow("inputImageHSV after converting CV_HSV2BGR", inputImageHSV);
	imshow("inputImageHSV", inputImageHSV);
	//waitKey(0);
	//ImageShowOnDebug("****Field and Contours - Shepet", "LineDetector", onlyWhiteImageLineDetector);

	//string filePath = "C:\\Users\\arabinow\\git\\RoboCup2016\\Media\\Photos\\White Lines\\StraightLine.png";

		//cvtColor(blackWhiteImgeBGR, src, CV_BGR2GRAY);
		//inputImageHSV = imread(filePath);
		   vector<Mat> spl;
		//imshow("blackWhiteImgeBGR after converting to gray scale from BGR", src);
		//waitKey(0);
		//split(inputImageHSV,spl);
		//imshow("spl1",spl[0]);
		//imshow("spl2",spl[1]);
		//imshow("spl3",spl[2]);
		//inputImageHSV = spl[1] - spl[0];
		//imshow("sub", inputImageHSV);
		Canny(src, dst, 50, 200, 3);
		imshow("dst after canny", dst);
		//waitKey(0);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		Mat field;
		FindField(inputImageHSV, field);
		Mat fieldAOrContours;
		bitwise_and(field, dst, fieldAOrContours);
		imshow("fieldAOrContours after bitwise_and", fieldAOrContours);
		//waitKey(0);
		cvtColor(fieldAOrContours, cdst, CV_GRAY2BGR);
		imshow("detected lines before line detection", cdst);
		//waitKey(0);

		vector<Vec2f> lines;
		HoughLines(fieldAOrContours, lines, 1, CV_PI / 180, 50, 0, 0);
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			double degree = (theta*180)/3.14;
			//cout<<degree<<endl;
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
		}
		//cout<<lines.size()<<endl;
		imshow("source", src);
		imshow("detected lines", cdst);
		//waitKey(0);
	//return 0;
	return new DetectedLine();
}


