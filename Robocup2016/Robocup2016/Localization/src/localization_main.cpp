#include "../headers/defines.h"
#include "../headers/PointOfInterest.h"
#include "../headers/Localization.h"
#include "../headers/RobocupField.h"
#include "../headers/MotionInfo.h"
#include "../headers/ParticleFilterAlgorithm.h"

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

using namespace std;

void fanalize(POIReport* p);
void initialize();
POIReport* testGenerateReports(int i = 0);

/*int main(int argc, char* argv[])
{
	Localization::GetInstance();

	initialize();
	POIReport* reports;
	for (int j = 0; j < 10; j++) {
		reports= testGenerateReports(0);
		//MotionInfo motion(5, 53.13*DEG2RAD);
		MotionInfo motion(0,0);
		Localization::GetInstance()->updateLocationByMotion(motion.distance, motion.angle);
		Localization::GetInstance()->updateLocationByVision(reports,27);
		sleep(2);
		char buf[100];
		sprintf(buf, "Incoming Inputs loop num: %d\n", j);
		WriteLogFile(buf);
	}

	fanalize(reports);
	cout << "Visionupdate= " << Localization::GetInstance()->isVisionUpdated << " Motionupdate=  " <<  Localization::GetInstance()->isMotionUpdated << endl;
	cout << "Ending Main" << endl;
	return 0;

}*/

void generateReport(POIReport* reports, short name, Particle realPlace, float derror, float aerror) {
	double dist = 0;// randomInRange(-derror, derror);
	double radian = 0;// randomInRange(-aerror, aerror);

	dist += calcDistance2D(realPlace.x,realPlace.y,RobocupField::AllPOIs[name].x, RobocupField::AllPOIs[name].y);
	radian += atan2(RobocupField::AllPOIs[name].y - realPlace.y, RobocupField::AllPOIs[name].x - realPlace.x)
		- realPlace.angle;
	// XY space from Camera space. so (dteta) becomes (Localization::GetInstance()->angles + dteta)
	
	reports[name].angle = radian;
	reports[name].distance = dist;
	reports[name].type = PointOfInterest::GENERAL_SHAPE;
}

POIReport* testGenerateReports(int i) {

		char buf[100];
		// Add random reports:
		Particle testP; // The same particle from the documintation
		testP.x = 30 + i * 3,
		testP.y = 70 + i * 4;
		testP.angle = 0.0f*DEG2RAD;

		// Cant be more reports than the real POIs.
		POIReport* reports=new POIReport[27];
		for (int j = 0; j < 27; j++) {
			generateReport(reports, j, testP, 3.2f, 5*DEG2RAD);
			sprintf(buf, "report %d distance %f angle %f\n", j, reports[j].distance, reports[j].angle*RAD2DEG);
			WriteLogFile(buf);
		}
		return reports;
}

void initialize() {
	//srand(time(NULL));
	srand(0);
	FILE *file;
	file = fopen("LocalizationlogFile.txt", "r");
	if (file != NULL) {
		fclose(file);
		remove("LocalizationlogFile.txt");
	}
}

void fanalize(POIReport* p) {
	if (p != NULL)
		delete[] p;
}
