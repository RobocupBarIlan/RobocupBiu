#include "Vision.h"
#include <sys/time.h>

void SigTermHandler(int signal)
{
	cout << "Waiting until next frame and killing" << endl;
	Vision::GetInstance()->TerminateThread();
}

Vision::Vision()
{
	Calibration::ReadFromFile();

	m_VideoCapture.open(0);
	m_VideoCapture.set(CV_CAP_PROP_FPS, 50);
	m_VideoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	m_VideoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	Gate = new GateDetector();
	Ball = new BallDetector();
	Line = new LineDetector();

	IsVisionThreadRunning = false;
	m_log = Log::GetInstance();
	m_logCategory = typeid(this).name();
	m_SaveVideo = false;
	m_RunBallDetection = true;
	m_RunGoalDetection = true;
	m_RunLineDetection = false;
	m_RunBallMovementCalc = false;

	// Video settings
	if (m_SaveVideo)
	{
		const string outputFile =
				"/home/robot/workspace2/RoboCup2016/RoboCup2016/GoalKeeper2016/demo.avi";
		m_OutputVideo.open(outputFile, CV_FOURCC('M', 'J', 'P', 'G'), 10,
				Size(FRAME_WIDTH, FRAME_HEIGHT), true);
	}
}

Vision::~Vision()
{
	destroyAllWindows();
	Vision::m_instance = NULL;

	delete Ball;
	delete Gate;
}

Vision* Vision::m_instance = NULL;
Vision* Vision::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Vision();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}

void Vision::Run()
{
	try
	{
		signal(SIGTERM, SigTermHandler);

		Vision::GetInstance()->IsVisionThreadRunning = true;

		// capture loop
		char key = 0;
		while (key != 'q'
				&& Vision::GetInstance()->IsVisionThreadRunning == true)
		{
			try
			{
				Mat currentFrame;
				m_VideoCapture >> currentFrame;

				Vision::GetInstance()->ProcessCurrentFrame(currentFrame);
				imshow("Output", currentFrame);
				key = waitKey(1);

				if (m_SaveVideo)
				{
					m_OutputVideo.write(currentFrame);
				}
			}
			catch (exception& ex)
			{
				m_log->Error(ex.what(), m_logCategory);
				cout << ex.what() << endl;
			}
		}
	}
	catch (exception& ex)
	{
		m_log->Error(ex.what(), m_logCategory);
		cout << ex.what() << endl;
	}

	destroyAllWindows();
}

void Vision::ProcessCurrentFrame(Mat& currentFrame)
 {
	if (m_RunGoalDetection.SafeRead()) {
		Gate->Detect(currentFrame);
	}

	if (m_RunBallDetection.SafeRead()) {
		Ball->Detect(currentFrame);

		if (m_RunBallMovementCalc.SafeRead())
		{
			TimedDetectedBall timedDetectedBall(*(DetectedBall*)Ball->Get());
			BallMovement ballMovement = m_ballMovementCalculator.CalculateBallMovement(timedDetectedBall);
			BallMovementCalc.SafeWrite(ballMovement);
		}
	}

	if (m_RunLineDetection.SafeRead()) {
		Line->Detect(currentFrame);
	}
}

void Vision::TerminateThread()
{
	IsVisionThreadRunning = false;
}

void Vision::StartBallDetection()
{
	m_RunBallDetection.SafeWrite(true);
}
void Vision::StopBallDetection()
{
	m_RunBallDetection.SafeWrite(false);
	m_ballMovementCalculator.ResetSamples();
}

void Vision::StartBallMovementCalc()
{
	m_RunBallMovementCalc.SafeWrite(true);
}
void Vision::StopBallMovementCalc()
{
	m_RunBallMovementCalc = false;
}

void Vision::StartGoalDetection()
{
	m_RunGoalDetection.SafeWrite(true);
}
void Vision::StopGoalDetection()
{
	m_RunGoalDetection.SafeWrite(false);
}

void Vision::StartLineDetection()
{
	m_RunLineDetection.SafeWrite(true);
}
void Vision::StopLineDetection()
{
	m_RunLineDetection.SafeWrite(false);
}
