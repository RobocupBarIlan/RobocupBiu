#include "../headers/ParticleFilterAlgorithm.h"
#include "../headers/functions.h"
#include "Includes.h"

void ParticleFilterAlgorithm::compute(Particle* & particleArr, const MotionInfo & motion, const POIReport* reports, int reportNum)
{
	moveAllParticles(particleArr, motion);
	calcWeights(particleArr,  reports,reportNum);
	sortParticleByWeight(particleArr);
	logIt(particleArr);
	resample(particleArr);
}

void ParticleFilterAlgorithm::moveAllParticles(Particle* particlesArr  , const MotionInfo & motion)
{
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		particlesArr[i].x += motion.distance * cos(motion.angle) + rand_normal_noise(); //abys: cos & sin are in radians. Thus, reportAngle also should be in radians
		particlesArr[i].y += motion.distance * sin(motion.angle) + rand_normal_noise();
		//TODO: is "z" the rotation (direction of body/face) or the spatial direction of the particle
		particlesArr[i].angle += 0 + rand_normal_noise(); //in case it's the rotation of robot's body/face
	}
}

void ParticleFilterAlgorithm::resample(Particle* &particleArr)
{
		// Accumalte weights and distribute new particles

		Particle* _newParticle= new Particle[NUM_OF_PARTICLES];
		float accWeights[NUM_OF_PARTICLES];

		// Accummulate Weight for fast finding
		float acc = 0.0f;
		for (int i = 0; i < NUM_OF_PARTICLES; i++) {
			acc += particleArr[i].weight;
			accWeights[i] = acc;
		}

		// Choose new set proportional to weight size.
		for (int i = 0; i < NUM_OF_PARTICLES; i++) {
			int index = binaryRangeFind(accWeights);
			if (index > -1) {
				_newParticle[i] = particleArr[index];	
			}
			else {
				printf("Resample: binaryRangeFind returned %d\n", index);
			}
		}

		//// Replace old array with new
		//for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		//	particleArr[i].x = _newParticle[i].x;
		//	particleArr[i].y = _newParticle[i].y;
		//	particleArr[i].angle = _newParticle[i].angle;
		//}
		if (particleArr != NULL) {
			delete[] particleArr;
			particleArr = _newParticle;
		}

		// Add noise (for kidnapped problem):
		for (int i = 0; i < NUM_OF_PARTICLES; i++) {
			particleArr[i].x += rand_normal_noise();
			particleArr[i].y += rand_normal_noise();
			//TODO: is "z" the rotation (direction of body/face) or the spatial direction of the particle
			particleArr[i].angle += rand_normal_noise();
		}
	
}

void ParticleFilterAlgorithm::calcWeights(Particle * particleArr, const POIReport * reports, int reportNum)
{
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		float w = 0;
		for (int j = 0; j <  reportNum; j++)
		{
			w+= weightParticleByReport(particleArr[i], reports[j]);		
		}
		particleArr[i].weight = w;
	}
}

float ParticleFilterAlgorithm::weightParticleByPOI(const Particle & particle, const POIReport & report, const PointOfInterest & poi)
{
	float dcoeff = 10.0f;
	float tcoeff = 1.0f;
	const float normalized_factor = 0.01f;
	float	d = dcoeff *  abs(report.distance - abs(calcDistance2D(normalized_factor*poi.x, normalized_factor*poi.y, particle.x, particle.y)));
	float	t = tcoeff * abs(
		report.angle
		- calcRelativeAngle(atan2(normalized_factor*poi.y - particle.y, normalized_factor*poi.x - particle.x), particle.angle) // angle in radians.
		);
	return exp(-d - t);

}

float ParticleFilterAlgorithm::weightParticleByReport(const Particle& particle,const POIReport& report)
{
	float w = 0;
	PointOfInterest* relevantPOIs;
	int POIsNum = 0;
	switch (report.type) {
	case PointOfInterest::CIRCLE_SHAPE: { relevantPOIs = RobocupField::CirclePOIs; POIsNum = RobocupField::CPOICount; break; }
	case PointOfInterest::DOT_SHAPE: { relevantPOIs = RobocupField::DotPOIs; POIsNum = RobocupField::DPOICount; break; }
	case PointOfInterest::L_SHAPE: { relevantPOIs = RobocupField::LTypePOIs; POIsNum = RobocupField::LPOICount; break; }
	case PointOfInterest::PLUS_SHAPE: { relevantPOIs = RobocupField::PlusPOIs; POIsNum = RobocupField::PPOICount; break; }
	case PointOfInterest::T_SHAPE: { relevantPOIs = RobocupField::TTypePOIs; POIsNum = RobocupField::TPOICount; break; }
	default : {relevantPOIs = RobocupField::AllPOIs; POIsNum = RobocupField::AllCount; break; }
	}
	for (int i = 0; i< POIsNum ; i++) {
		w += weightParticleByPOI(particle, report, relevantPOIs[i]);
	}
	return w;
}

void ParticleFilterAlgorithm::sortParticleByWeight(Particle * particleArr)
{
	for (int i = 0; i < NUM_OF_PARTICLES; i++)
	{
		for (int j = 0; j < NUM_OF_PARTICLES - 1 - i; j++)
		{
			if (particleArr[j].weight < particleArr[j+1].weight)
			{
				swap(particleArr[j], particleArr[j + 1]);
			}
		}
	}
}



void ParticleFilterAlgorithm::logIt(Particle* arr) {
	char buf[100];
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		sprintf(buf, "particle location (%f,%f,%f) weights %f\n", arr[i].x, arr[i].y, arr[i].angle*RAD2DEG, arr[i].weight);
		WriteLogFile(buf);
	}
	sprintf(buf, "****==================================================================================***\n");
	WriteLogFile(buf);
}
