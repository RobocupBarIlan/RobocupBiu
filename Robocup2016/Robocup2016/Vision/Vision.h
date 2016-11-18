#include "Detectors/BallDetector.h"
#include "Detectors/GateDetector.h"
#include "Detectors/LineDetector.h"
#include "../Log/Log.h"
#include "../Brain/Goalkeeper/BallMovementCalculator.h"
#include "../Common/SharedMemory.h"

#pragma once

void RunVision();

class Vision
{
public:
	// Singletone
	static Vision* GetInstance();

	void Run();
	void TerminateThread();

	~Vision();

	Detector* Gate;
	Detector* Ball;
	Detector* Line;
	SharedMemory<BallMovement> BallMovementCalc;

	BallMovementCalculator m_ballMovementCalculator;

	bool IsOppositeGKInFrame();

	bool IsVisionThreadRunning;

	void ProcessCurrentFrame(Mat& frame);

	void StartBallDetection();
	void StopBallDetection();

	void StartBallMovementCalc();
	void StopBallMovementCalc();

	void StartGoalDetection();
	void StopGoalDetection();

	void StartLineDetection();
	void StopLineDetection();

private:
	VideoCapture m_VideoCapture;
	Log* m_log;
	string m_logCategory;

	// Whether to save a video of the current run.
	bool m_SaveVideo;
	// The video writer that saves the video if m_SaveVideo is true
	VideoWriter m_OutputVideo;

	// Whether to run ball\Goal\Line detection during the Vision run.
	SharedMemory<bool> m_RunBallDetection;
	SharedMemory<bool> m_RunBallMovementCalc;
	SharedMemory<bool> m_RunGoalDetection;
	SharedMemory<bool> m_RunLineDetection;

	static Vision* m_instance;
	Vision();
};

