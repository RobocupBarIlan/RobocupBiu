// Calibrator.cpp : Defines the entry point for the console application.
//

#include "Calibrator.h"

int main(int argc, char* argv[])
{
	Calibrator* calibrator = Calibrator::GetInstance();
	calibrator->Calibrate();
	delete calibrator;
	return 0;
}
