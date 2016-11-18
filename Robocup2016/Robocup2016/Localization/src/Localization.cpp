#include "../headers/Localization.h"

#define _USE_MATH_DEFINES
#include <cmath>

Localization* Localization::m_instance = NULL;
Localization* Localization::GetInstance()
{
    if(m_instance == NULL)
    {
//    	m_instance = new Localization();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}


//Localization::Localization():m_gyroThread(thread(&Localization::LocalizationActionAsync,this))
//{
//	particles = new Particle[NUM_OF_PARTICLES];
//	initializeParticles();
//	m_terminateThread = false;
//	isMotionUpdated = false;
//	isVisionUpdated = false;
//
//}

Localization::~Localization()
{
	if (particles != NULL) {
		delete[] particles;
	}
	if (m_instance !=NULL){
		delete m_instance;
	}
}


LocationGuess Localization::getPosition()
{
	return locationGuess;
}

//TODO : add condition to computing alg;update locationGuess
void  Localization::LocalizationActionAsync(){

	while(!m_terminateThread){
		if (isVisionUpdated && isMotionUpdated){
			isVisionUpdated = false;
			isMotionUpdated = false;
			ParticleFilterAlgorithm::compute(particles, motionInf, reports,reportNum);
			printAllParticles();
		}
	}
}



void Localization::updateLocationByMotion(const float distance, const float angle)
{
	motionInf.angle = angle;
	motionInf.distance = distance;
	isMotionUpdated = true;
}

void Localization::initializeParticles()
{
//	particles[0].x = 135;
//	particles[0].y = -300;
//	particles[0].angle = 45.0f* DEG2RAD;

	particles[0].x = 30;
	particles[0].y = 70;
	particles[0].angle = 0.0f* DEG2RAD;

	particles[1].x = -30;
	particles[1].y = -70;
	particles[1].angle = 180.0f* DEG2RAD;

	for (int i = 2; i < NUM_OF_PARTICLES;i++) {
		particles[i].x = randomInRange((-RobocupField::A / 2), RobocupField::A / 2);
		particles[i].y = randomInRange((-RobocupField::B / 2), RobocupField::B / 2);
		particles[i].angle = randomInRange((-M_PI), M_PI);
	}
	printAllParticles();
}

void Localization::printAllParticles()
{
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		printf("printAll: x= %f y= %f angle= %f w=%f\n", particles[i].x, particles[i].y, particles[i].angle*RAD2DEG,particles[i].weight);
	}
}

void Localization::computeLocation()
{
}

vector<float> Localization::bestGuess()
{
	return vector<float>();
}

