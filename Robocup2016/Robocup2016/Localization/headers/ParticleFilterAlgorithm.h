#pragma once
#include "defines.h"
#include "Particle.h"
#include "MotionInfo.h"
#include "POIReport.h"
#include "RobocupField.h"

class ParticleFilterAlgorithm
{
public:
	static void compute(Particle* & particleArr,const MotionInfo& motion, const POIReport* reports,int reportNum);

private:
	static void moveAllParticles(Particle* particleArr, const MotionInfo & motion);
	static void resample(Particle* & particleArr);
	static void calcWeights(Particle* particleArr, const POIReport* reports,int reportNum );
	static float weightParticleByPOI(const Particle& particle, const POIReport& report,const PointOfInterest& poi );
	static float weightParticleByReport(const Particle& particle , const POIReport& report);
	
	static void sortParticleByWeight(Particle* particleArr);
	static void logIt(Particle* arr);
};

