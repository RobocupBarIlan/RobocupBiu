#include "BallDetector.h"
#include <deque>
using namespace std;

//std::queue<Point2f> maxCentersForRecursiveFilter;
//std::deque<Point2f> maxCentersForRecursiveFilter;
//std::deque<Point2f> maxCentersForRecursiveFilter;
//list<Point2f> maxCentersForRecursiveFilter;
BallDetector::BallDetector()
{
	m_BallDistanceCalculator = new BallDistanceCalculator();
}

BallDetector::~BallDetector()
{
	delete m_BallDistanceCalculator;
}

DetectedObject* BallDetector::geometricBallDetection(Mat& inputImageHSV)
{
	Mat field; // B&W image represent the field.
	if (!FindField(inputImageHSV, field))
	{
		// Cannot find field - Ball not found.
		return new DetectedBall();
	}


	Mat onlyGreenImage;
	inRange(inputImageHSV, Calibration::GetInstance()->MinGreenHSV, Calibration::GetInstance()->MaxGreenHSV, onlyGreenImage);
	//imshow("BallDetector: onlyGreenImage", onlyGreenImage);
	//waitKey(0);
	Mat nonGreenImage;
	bitwise_not(onlyGreenImage, nonGreenImage);
	//imshow("BallDetector: nonGreenImage", nonGreenImage);
	//waitKey(0);
	Mat holesInField;
	bitwise_and(nonGreenImage, field, holesInField);
	//imshow("BallDetector: holesInField", holesInField);
	//waitKey(0);
	// Smooth image with blur and closing
	medianBlur(holesInField, holesInField, 15);
	CloseImage(holesInField, holesInField);
	//imshow("BallDetector: holesInField after CloseImage", holesInField);
	//waitKey(0);
	ImageShowOnDebug("Holes in field", "BallDetector", holesInField);

	//vector< vector<Point> > contoursVector = FindContours(holesInField, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	vector< vector<Point> > contoursVector = FindContours(holesInField, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	if (contoursVector.size() == 0)
	{
		return new DetectedBall();
	}

	vector<Point2f> centersVector;
	centersVector.resize(contoursVector.size());
	vector<float> radiusesVector;
	radiusesVector.resize(contoursVector.size());

	float maxRatio = -1.0f;
	int maxRatioIndex = -1;

	// Get the contour with the maximum ratio between its area to the area of its enclosing circle.
	for (size_t i = 0; i < contoursVector.size(); i++)
	{
		minEnclosingCircle(contoursVector[i], centersVector[i], radiusesVector[i]);
		circle(holesInField, centersVector[i], radiusesVector[i], Colors::Blue, 2);

		double holeArea = contourArea(contoursVector[i]);
		double enclosingCircleArea = M_PI * pow(radiusesVector[i], 2);
		double ratio = holeArea / enclosingCircleArea;

		const float rationThreshold = 0.7;
		const float radiusThreshold = 10;
		if (ratio < rationThreshold || radiusesVector[i] < radiusThreshold)
		{
			continue;
		}

		if (maxRatio < ratio)
		{
			maxRatio = ratio;
			maxRatioIndex = i;
		}
	}
	// TODO Auto-generated destructor stub
	if (maxRatioIndex == -1)
	{
		return new DetectedBall();
	}

	if (radiusesVector[maxRatioIndex] < 0.0f || radiusesVector[maxRatioIndex] > 10000.0f)
	{
		printf("Radius is corrupt in BallDetector.FindBall()");
	}

	ImageShowOnDebug("HolesInField", "BallDetector", holesInField);
	return new DetectedBall(centersVector[maxRatioIndex], radiusesVector[maxRatioIndex], CalculateDistance(centersVector[maxRatioIndex], radiusesVector[maxRatioIndex]));

//	if (maxCentersForRecursiveFilter.size() < DEQUE_SIZE) {
//		maxCentersForRecursiveFilter.push_back(max_center);

//	} else {
//		int sumOfxAxis = 0;
//		int sumOfyAxis = 0;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//		std::deque<Point2f>::iterator it = maxCentersForRecursiveFilter.begin();
//		int i = DEQUE_SIZE;
//		while (it != maxCentersForRecursiveFilter.end()) {
//			sumOfxAxis = (it._M_cur->x + sumOfxAxis) / i;
//			sumOfyAxis = (it._M_cur->y + sumOfyAxis) / i;
//			it++;
//			i++;
//		}
//		max_center.x = sumOfxAxis;
//		max_center.y = sumOfyAxis;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//	}
}

DetectedObject* BallDetector::DetectObject(Mat& inputImageHSV)
{
	DetectedObject* detectedObject = geometricBallDetection(inputImageHSV);
	if(detectedObject->IsDetected)
		return detectedObject;
	else
	{
		detectedObject = probabilisticBallDetection(inputImageHSV);
	}

	return detectedObject;
}



DetectedObject* BallDetector::probabilisticBallDetection(Mat& inputImageHSV)
{
	int minRad, maxRad, p1, p2,dp, minDist;
	dp = 6;
	minDist = 120;
	minRad = 10;
	maxRad = 100;
	p1 = 100;
	p2 = 200;
	bool isCurrectDetection = false;
	//double
//	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);
//			createTrackbar("minrad", "Control", &minRad, 500);
//			createTrackbar("maxrad", "Control", &maxRad, 500);
//			createTrackbar("p1", "Control", &p1, 500);
//			createTrackbar("p2", "Control", &p2, 500);
//			createTrackbar("dp", "Control", &dp, 500);
//			createTrackbar("minDist", "Control", &minDist, 1000);

	Mat onlyWhiteImage, field;
	GetWhiteImage(inputImageHSV, onlyWhiteImage);
	FindField(inputImageHSV, field);
	//imshow("BallDetector: field", field);
	Mat onlyGreenImage;
	inRange(inputImageHSV, Calibration::GetInstance()->MinGreenHSV, Calibration::GetInstance()->MaxGreenHSV, onlyGreenImage);
	Mat nonGreenImage;
	bitwise_not(onlyGreenImage, nonGreenImage);
	Mat holesInField;
	bitwise_and(nonGreenImage, field, holesInField);
	// Smooth image with blur and closing
	medianBlur(holesInField, holesInField, 15);
	CloseImage(holesInField, holesInField);// end of holes
	//imshow("holesInField: probabilistic algorithm", holesInField);
	Mat houghCircleInput, houghCircleInputGray, greenMatrixImage, originalBGRImage;
	vector<Vec3f> circles, circlesInField, circlesInFieldAndBlockHole;
	cvtColor(inputImageHSV, greenMatrixImage, CV_HSV2BGR);
	cvtColor(greenMatrixImage, houghCircleInputGray, CV_BGR2GRAY);
	vector<Mat> splittedMat;
	//split(greenMatrixImage,splittedMat);
	//imshow("Red", splittedMat[2]);
	//waitKey(10);
	//imshow("spl1",splittedMat[0]);
	//imshow("spl2",splittedMat[1]);
	//imshow("spl3",splittedMat[2]);

	//imshow("inputImageHSV", inputImageHSV);
	//waitKey(0);
	//cvtColor(inputImageHSV, houghCircleInput, CV_HSV2BGR);
	//imshow("houghCircleInput", houghCircleInput);
	//waitKey(0);
	//cvtColor(houghCircleInput, houghCircleInputGray, CV_BGR2GRAY);
	//imshow("houghCircleInputGray", houghCircleInputGray);
	//waitKey(0);
	//splittedMat[0] = splittedMat[1] - splittedMat[2];
	//HoughCircles(splittedMat[1], circles, CV_HOUGH_GRADIENT,2, splittedMat[1].rows/4, 200, 100);
	/*long* currentTime;
	time(currentTime);
	cout<<"Hough circle starts: "<<currentTime<<endl;
	*/

	try
	{
		//cout << houghCircleInputGray.rows/4 << endl;
		GaussianBlur( houghCircleInputGray, houghCircleInputGray, Size(7, 7), 0.75, 0.75 );
		//medianBlur(houghCircleInputGray, houghCircleInputGray, 15);
		//blur(houghCircleInputGray, houghCircleInputGray, Size(3,3));
		HoughCircles(houghCircleInputGray, circles, CV_HOUGH_GRADIENT,6, houghCircleInputGray.rows/4, 200, 100, 10, 100); //TODO: use this line
		  //HoughCircles(houghCircleInputGray, circles, CV_HOUGH_GRADIENT, dp, minDist, p1, p2, minRad, maxRad);
		//HoughCircles(houghCircleInputGray, circles, CV_HOUGH_GRADIENT,6, houghCircleInputGray.rows/4, 100, 200, 10, 100);
	}
	catch (Exception& ex)
	{
		cout << ex.msg<< endl;
	}
	if (circles.size() == 0)
		return new DetectedBall();

	//cout<<"circles size: "<<circles.size()<<endl;
	//Mat greenMatrixImage;
	//Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
	Point* finalCenter;
	int finalRadius = 50000;
	int radius; //= cvRound(circles[0][2]);
	Scalar currentIntensity; //= inputImageHSV.at<uchar>(0,0);
	int maxDiameters = 0;
	int chosenCenterLocationInArray = 0;
	//int* diametersArray = (int*)malloc(circles.size());
    //for( size_t i = 0; i < circles.size(); i++ )
    //{
    //    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //    currentIntensity = field.at<uchar>(center);
    //    if(currentIntensity.val[0] != 0)
    //   	 circlesInField.push_back(circles[i]);
    //}

	//vector<Vec3f> circlesInField;
    for( size_t i = 0; i < circles.size(); i++ )
    {

         Point2f circleCenter(circles[i][0], circles[i][1]);
         Scalar currentPixel = field.at<uchar>(circleCenter);
         if(currentPixel.val[0] != 0)
         {
        	 Vec3f relavantCircle = Vec3f(circles[i][0], circles[i][1], circles[i][2]);
        	 circlesInField.push_back(relavantCircle);
         }
    }
    if(circlesInField.size() == 0)
    	return new DetectedBall;

    //Start of new code
    for( size_t i = 0; i < circlesInField.size(); i++ )
    {
    	 Point2f center(circlesInField[i][0], circlesInField[i][1]);
         int radius = cvRound(circlesInField[i][2]);
         int holeTrhshold = (int)(radius*4)*255*0.9;

         // draw the circle center
         //circle(greenMatrixImage, center, (float)radius, Colors::Blue, 2);
         //if(radius < finalRadius)
         //{
        //	 finalRadius = radius;
        //	 finalCenter= new Point(center);
         //}
         circle( greenMatrixImage, center, 3, Scalar(0,255,0), -1, 8, 0 );
         circle( greenMatrixImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
         //cout<<"radius ["<<i<<"] = "<<radius<<endl;
         int xPlusDiameter = 0;
         int xMinusDiameter = 0;
         int yPlusDiameter = 0;
         int yMinusDiameter = 0;

         int xPlusDiameterInHoles = 0;
         int xMinusDiameterInHoles = 0;
         int yPlusDiameterInHoles = 0;
         int yMinusDiameterInHoles = 0;
         //waitKey(0);
         for(int j = 0; j < radius; j++)
         {

        	 //xPlusDiameter += onlyWhiteImage.at<uchar>(center.x + j, center.y);
        	 //xMinusDiameter += onlyWhiteImage.at<uchar>(center.x - j, center.y);
        	 //yPlusDiameter += onlyWhiteImage.at<uchar>(center.x, center.y + j);
        	 //yMinusDiameter += onlyWhiteImage.at<uchar>(center.x, center.y -j);
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.y, center.x + j);
        	 xPlusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.y, center.x - j);
        	 xMinusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.y + j, center.x);
        	 yPlusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.y -j, center.x);
        	 yMinusDiameter += currentIntensity.val[0];

        	 currentIntensity = holesInField.at<uchar>(center.y, center.x + j);
        	 xPlusDiameterInHoles += currentIntensity.val[0];
        	 currentIntensity = holesInField.at<uchar>(center.y, center.x - j);
        	 xMinusDiameterInHoles += currentIntensity.val[0];
        	 currentIntensity = holesInField.at<uchar>(center.y + j, center.x);
        	 yPlusDiameterInHoles += currentIntensity.val[0];
        	 currentIntensity = holesInField.at<uchar>(center.y -j, center.x);
        	 yMinusDiameterInHoles += currentIntensity.val[0];

         }
         //diametersArray[i] = xPlusDiameter + xMinusDiameter + yPlusDiameter +yMinusDiameter;
         // draw the circle outline
         int currentDiameter = xPlusDiameter + xMinusDiameter + yPlusDiameter +yMinusDiameter;
         int currentDiameterInHoles = xPlusDiameterInHoles + xMinusDiameterInHoles + yPlusDiameterInHoles +yMinusDiameterInHoles;
         if(currentDiameter > maxDiameters)
         {
        	 maxDiameters = currentDiameter;
        	 chosenCenterLocationInArray = i;
        	 if(currentDiameterInHoles > holeTrhshold)
        		 isCurrectDetection = true;
        	 //isCurrectDetection =true;
         }
    }
    imshow("BallDetector: greenMatrixImage", greenMatrixImage);
    waitKey(10);
    if(!isCurrectDetection)
    	return new DetectedBall;
    finalRadius =cvRound(circlesInField[chosenCenterLocationInArray][2]);
    finalCenter= new Point(cvRound(circlesInField[chosenCenterLocationInArray][0]), cvRound(circlesInField[chosenCenterLocationInArray][1]));
    circle(houghCircleInputGray, *finalCenter, (float)finalRadius, Colors::Blue, 2);

	return new DetectedBall(*finalCenter, (float)finalRadius, CalculateDistance(*finalCenter, (float)finalRadius));


    /* TODO: strat of the old code
    for( size_t i = 0; i < circlesInField.size(); i++ )
    {
    	 Point2f center(circlesInField[i][0], circlesInField[i][1]);
         int radius = cvRound(circlesInField[i][2]);
         // draw the circle center
         //circle(greenMatrixImage, center, (float)radius, Colors::Blue, 2);
         //if(radius < finalRadius)
         //{
        //	 finalRadius = radius;
        //	 finalCenter= new Point(center);
         //}
         circle( greenMatrixImage, center, 3, Scalar(0,255,0), -1, 8, 0 );
         circle( greenMatrixImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
         //cout<<"radius ["<<i<<"] = "<<radius<<endl;
         int xPlusDiameter = 0;
         int xMinusDiameter = 0;
         int yPlusDiameter = 0;
         int yMinusDiameter = 0;
         for(int j = 0; j < radius; j++)
         {

        	 //xPlusDiameter += onlyWhiteImage.at<uchar>(center.x + j, center.y);
        	 //xMinusDiameter += onlyWhiteImage.at<uchar>(center.x - j, center.y);
        	 //yPlusDiameter += onlyWhiteImage.at<uchar>(center.x, center.y + j);
        	 //yMinusDiameter += onlyWhiteImage.at<uchar>(center.x, center.y -j);
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.x + j, center.y);
        	 xPlusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.x - j, center.y);
        	 xMinusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.x, center.y + j);
        	 yPlusDiameter += currentIntensity.val[0];
        	 currentIntensity = onlyWhiteImage.at<uchar>(center.x, center.y -j);
        	 yMinusDiameter += currentIntensity.val[0];
         }
         //diametersArray[i] = xPlusDiameter + xMinusDiameter + yPlusDiameter +yMinusDiameter;
         // draw the circle outline
         int currentDiameter = xPlusDiameter + xMinusDiameter + yPlusDiameter +yMinusDiameter;
         if(currentDiameter < maxDiameters)
         {
        	 maxDiameters = currentDiameter;
        	 chosenCenterLocationInArray = i;
         }
    }
    imshow("BallDetector: greenMatrixImage", greenMatrixImage);
    waitKey(10);
    finalRadius =cvRound(circlesInField[chosenCenterLocationInArray][2]);
    finalCenter= new Point(cvRound(circlesInField[chosenCenterLocationInArray][0]), cvRound(circlesInField[chosenCenterLocationInArray][1]));
    circle(houghCircleInputGray, *finalCenter, (float)finalRadius, Colors::Blue, 2);

	return new DetectedBall(*finalCenter, (float)finalRadius, 0);
	*/ //TODO: end of old code
	//return new DetectedBall();


//	if (maxCentersForRecursiveFilter.size() < DEQUE_SIZE) {
//		maxCentersForRecursiveFilter.push_back(max_center);

//	} else {
//		int sumOfxAxis = 0;
//		int sumOfyAxis = 0;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//		std::deque<Point2f>::iterator it = maxCentersForRecursiveFilter.begin();
//		int i = DEQUE_SIZE;
//		while (it != maxCentersForRecursiveFilter.end()) {
//			sumOfxAxis = (it._M_cur->x + sumOfxAxis) / i;
//			sumOfyAxis = (it._M_cur->y + sumOfyAxis) / i;
//			it++;
//			i++;
//		}
//		max_center.x = sumOfxAxis;
//		max_center.y = sumOfyAxis;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//	}
}

float BallDetector::CalculateDistance(Point2f center, float radius)
{
    float tilt = Motion::GetInstance()->GetHeadTilt().Tilt;
    return m_BallDistanceCalculator->CalculateDistance(center, radius, (int)tilt);
}
