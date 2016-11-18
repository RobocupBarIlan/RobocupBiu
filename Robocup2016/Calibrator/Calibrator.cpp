#include "Calibrator.h"

Mat currentFrame;
const int trackbarValueMax = 3;
int trackbarValue;

Calibrator* Calibrator::m_instance = NULL;

Calibrator::Calibrator()
{
	m_Calibration = Calibration::GetInstance();
}

Calibrator* Calibrator::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Calibrator();
    }
 	return m_instance;
}

void Calibrator::Calibrate()
{
	try
	{
		InitializeWindow();
	}
	catch (Exception& ex)
	{
		cout << "Couldn't initialize window" << endl;
		exit(1);
	}

	m_Calibration = Calibration::ReadFromFile();

	VideoCapture videoCapture;
	if (!videoCapture.open(0))
	{
		exit(1);
	}

	while (true)
	{
		try
		{
			videoCapture >> currentFrame;
			if (currentFrame.empty())
			{
				break; // end of video stream
			}
		}
		catch (Exception& ex)
		{
			cout << "Couldn't read image: " << ex.what() << endl;
		}

		ShowCalibration(currentFrame);

		// Close the calibrator & save settings by clicking 'q'.
		// Close without saving by clicking 'z'
		char exitKey = waitKey(20);
		if (exitKey == 'q')
		{
			m_Calibration->WriteCalibrationToFile();
			destroyAllWindows();
			return;
		}
		if (exitKey == 'z')
		{
			destroyAllWindows();
			return;
		}
	}	
}

void onMouse(int event, int mouseX, int mouseY, int flag, void* param)
{
	if (event != CV_EVENT_LBUTTONDOWN && event != CV_EVENT_RBUTTONDOWN)
	{
		return;
	}

	Calibrator* calibrator = Calibrator::GetInstance();

	Point2i mousePosition(mouseX,mouseY);
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		calibrator->ResetValuesToClick(currentFrame, mousePosition);
	}
	
	else
	{
		int calibrationType = getTrackbarPos(TrackbarName, ImageWindow);
		switch (calibrationType)
		{
			case Green_Calibration:
			{
				calibrator->CalibrateGreen(currentFrame, mousePosition);
				break;
			}
			case White_Calibration:
			{
				calibrator->CalibrateWhite(currentFrame, mousePosition);
				break;
			}
			case Our_GK_Calibration:
			{
				calibrator->CalibrateOurGK(currentFrame, mousePosition);
				break;
			}
			case Opposite_GK_Calibration:
			{
				calibrator->CalibrateOppGK(currentFrame, mousePosition);
				break;
			}
		}

	}
}

void Calibrator::ResetValuesToClick(Mat image, Point2i position)
{
	int calibrationType = getTrackbarPos(TrackbarName, ImageWindow);
	switch (calibrationType)
		{
		case Green_Calibration: {
			m_Calibration->MinGreenHSV = MinAreaScalar(image, position);
			m_Calibration->MaxGreenHSV = MaxAreaScalar(image, position);
			break;
		}
		case White_Calibration: {
			m_Calibration->MinWhiteHSV = MinAreaScalar(image, position);
			m_Calibration->MaxWhiteHSV = MaxAreaScalar(image, position);
			break;
		}
		case Our_GK_Calibration: {
			m_Calibration->MinOurGKHSV = MinAreaScalar(image, position);
			m_Calibration->MaxOurGKHSV = MaxAreaScalar(image, position);
			break;
		}
		case Opposite_GK_Calibration: {
			m_Calibration->MinOppGKHSV = MinAreaScalar(image, position);
			m_Calibration->MaxOppGKHSV = MaxAreaScalar(image, position);
			break;
		}
	}
}

void Calibrator::ShowCalibration(Mat image)
{
	imshow(ImageWindow, currentFrame);
	cvtColor(currentFrame, currentFrame, CV_BGR2HSV);

	Mat threshold;

	int calibrationType = getTrackbarPos(TrackbarName, ImageWindow);
	switch (calibrationType)
	{
	case Green_Calibration: {
		inRange(image, m_Calibration->MinGreenHSV, m_Calibration->MaxGreenHSV,
				threshold);
		break;
	}
	case White_Calibration: {
		inRange(image, m_Calibration->MinWhiteHSV, m_Calibration->MaxWhiteHSV,
				threshold);
		break;
	}
	case Our_GK_Calibration: {
		inRange(image, m_Calibration->MinOurGKHSV, m_Calibration->MaxOurGKHSV,
				threshold);
		break;
	}
	case Opposite_GK_Calibration: {
		inRange(image, m_Calibration->MinOppGKHSV, m_Calibration->MaxOppGKHSV,
				threshold);
		break;
	}
	}

	cvtColor(threshold, threshold, CV_GRAY2BGR);
	putText(threshold, CalibrationTypeStrings[calibrationType], Point(20,20), Font_Type, Font_Scale, Font_Colour, 2); //Display the text in image window
	imshow(CalibrationWindow, threshold);
}

void Calibrator::CalibrateGreen(Mat image, Point2i position)
{
	m_Calibration->MinGreenHSV = Min(MinAreaScalar(image, position), m_Calibration->MinGreenHSV);
	m_Calibration->MaxGreenHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxGreenHSV);
}

void Calibrator::CalibrateWhite(Mat image, Point2i position)
{
	m_Calibration->MinWhiteHSV = Min(MinAreaScalar(image, position), m_Calibration->MinWhiteHSV);
	m_Calibration->MaxWhiteHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxWhiteHSV);
}

void Calibrator::CalibrateOurGK(Mat image, Point2i position)
{
	m_Calibration->MinOurGKHSV = Min(MinAreaScalar(image, position), m_Calibration->MinOurGKHSV);
	m_Calibration->MaxOurGKHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxOurGKHSV);
}

void Calibrator::CalibrateOppGK(Mat image, Point2i position)
{
	m_Calibration->MinOppGKHSV = Min(MinAreaScalar(image, position), m_Calibration->MinOppGKHSV);
	m_Calibration->MaxOppGKHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxOppGKHSV);
}

Scalar Calibrator::Min(Scalar a, Scalar b)
{
	Scalar minimum;
	for (int i = 0; i < 3; i++)
	{
		minimum[i] = MIN(a[i], b[i]);
	}

	return minimum;
}

Scalar Calibrator::Max(Scalar a, Scalar b)
{
	Scalar minimum;
	for (int i = 0; i < 3; i++)
	{
		minimum[i] = MAX(a[i], b[i]);
	}

	return minimum;
}

bool Calibrator::IsInImageBorders(int x, int y)
{
	return 0 <= x && x < FRAME_WIDTH && 0 <= y && y < FRAME_HEIGHT;
}

Scalar Calibrator::MinAreaScalar(Mat image, Point2i point)
{
	Scalar minScalar(255, 255, 255);

	for (int i = point.x - 1; i <= point.x + 1; i++)
	{
		for (int j = point.y - 1; j <= point.y + 1; j++)
		{
			if (IsInImageBorders(i, j))
			{
				Vec3b currentPixel = image.at<Vec3b>(j, i);
				for (int k = 0; k < 3; k++)
				{
					minScalar[k] = MIN(minScalar[k], currentPixel[k]);
				}
			}
		}
	}

	return minScalar;
}

Scalar Calibrator::MaxAreaScalar(Mat image, Point2i point)
{
	Scalar maxScalar(0, 0, 0);

	for (int i = point.x - 1; i <= point.x + 1; i++)
	{
		for (int j = point.y - 1; j <= point.y + 1; j++)
		{
			if (IsInImageBorders(i, j))
			{
				Vec3b currentPixel = image.at<Vec3b>(j, i);
				for (int k = 0; k < 3; k++)
				{
					maxScalar[k] = MAX(maxScalar[k], currentPixel[k]);
				}
			}
		}
	}

	return maxScalar;
}

void Calibrator::InitializeWindow()
{
	namedWindow(ImageWindow, CV_WINDOW_NORMAL);
	namedWindow(CalibrationWindow, CV_WINDOW_NORMAL);
	createTrackbar(TrackbarName, ImageWindow, &trackbarValue, trackbarValueMax);
	setMouseCallback(ImageWindow, onMouse, 0);
}
