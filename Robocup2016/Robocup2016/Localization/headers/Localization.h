#pragma once


#include "Particle.h"
#include "POIReport.h"
#include "defines.h"
#include "functions.h"
#include "RobocupField.h"
#include "ParticleFilterAlgorithm.h"
#include "LocationGuess.h"
#include "Includes.h"

using namespace std;

class Localization
{

public:

	// Singletone
	static Localization* GetInstance();
	~Localization();

	void inline stopThread()
	{
		m_terminateThread=true;
		//m_gyroThread.join();
	}

	void inline updateLocationByVision(const POIReport* _reports, int _reportNum)
	{
		reports = _reports;
		reportNum = _reportNum;
		isVisionUpdated = true;
	}

	LocationGuess getPosition();

	void updateLocationByMotion(const float distance, const float angle);
	void initializeParticles();

	bool isMotionUpdated;
	bool isVisionUpdated;

private:
	void LocalizationActionAsync();
	Localization();
	static Localization* m_instance;
	//thread m_gyroThread;
	bool m_terminateThread;

	Particle* particles;
	MotionInfo motionInf;
	const POIReport* reports;
	int reportNum;

	LocationGuess locationGuess;

	void printAllParticles();
	void computeLocation();
	vector<float> bestGuess();
	

};
