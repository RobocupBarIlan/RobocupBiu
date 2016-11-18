//============================================================================
// Name        : DistanceCalculator.cpp
// Author      : Hen Shoob & Assaf Rabinowitz
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "DistanceDBBuilder.h"
#include "../RoboCup2016/Motion/Motion.h"

void WaitForKey();
void BackupCurrentDatabase();
void DrawMiddleColumn(Mat &image);

int main(int argc, char** argv)
{
//	WaitForKey();
	//BackupCurrentDatabase();

	Calibration::ReadFromFile();
	Calibration* calibration = Calibration::GetInstance();

	DistanceDBBuilder distanceDBBuilder;

	VideoCapture videoCapture;
	videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	videoCapture.open(0);
	int tilt = atoi(argv[0]);

	cout << "Press \"s\" to continue. Current tilt: " << tilt << endl;
	while(true)
		{
			Mat currentFrame;
//			videoCapture.open("/home/robot/smaple.jpg");
			videoCapture >> currentFrame;

			imshow("Output", currentFrame);

			cvtColor(currentFrame, currentFrame, CV_BGR2HSV);
			Mat whiteImage;
			inRange(currentFrame, calibration->MinWhiteHSV, calibration->MaxWhiteHSV, whiteImage);

			int morph_size = 3;
			Mat element = getStructuringElement(MORPH_RECT,
					Size(2 * morph_size + 1, 2 * morph_size + 1),
					Point(morph_size, morph_size));
			medianBlur(whiteImage, whiteImage, 5);

			Mat whiteOutput = whiteImage.clone();
			cvtColor(whiteOutput, whiteOutput, CV_GRAY2BGR);
			DrawMiddleColumn(whiteOutput);
			//morphologyEx(whiteImage, whiteImage, MORPH_OPEN, element);

			imshow("White", whiteOutput);

			char exitKey = waitKey(30);
			if(exitKey == 'q')
			{
				break;
			}
			if(exitKey == 's')
			{
				//Motion::GetInstance()->SetHeadTilt(HeadTilt(0, tilt));
				distanceDBBuilder.CreateDatabaseForTilt(whiteImage, tilt);
				cout << "Press \"s\" to continue. Current tilt: " << tilt << endl;
			}
		}

	cout << "Distance DB finished" << endl;
	return 0;
}

void DrawMiddleColumn(Mat &image)
{
	int middleColumn = FRAME_WIDTH / 2;
	for (int row = FRAME_HEIGHT - 1; row > 0 ; row--)
	{
		image.at<Vec3b>(row, middleColumn)[0] = 255;
		image.at<Vec3b>(row, middleColumn)[1] = 0;
		image.at<Vec3b>(row, middleColumn)[2] = 0;
	}
}

void WaitForKey()
{
	cout << "Press any key to start the database creation" << endl;
	getchar();
}

void BackupCurrentDatabase()
{
	// TODO
}

