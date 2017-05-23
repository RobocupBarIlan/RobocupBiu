//============================================================================
// Name        : RoboCup2017.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "../Vision/VisionThread.h"






/*
 * This method makes sure that all signals had been registered to the different threads before any operation is done.
 */
void waitRegisterSignalDone()
{
	while (!VisionThread::IsRegisterSingalsDone()) //Wait until VisionThread registered all signal listeners.
	{
	}
}


int main() {

	cout << "~~~~~~~~~~~~~~Initiating threads:~~~~~~~~~~~~~~" << endl; // prints !!!Hello World!!!
	VisionThread::GetVisionThreadInstance()->init();
	//Test:

	waitRegisterSignalDone();

	while(true) //Test only! : keep asking for ball detection.
	{
		pthread_kill(VisionThread::GetVisionThreadInstance()->getVisionThread(),VisionThread::GET_BALL_CENTER_IN_FRAME_AND_DISTANCE);
	}
	pthread_exit(NULL); //Exit the main thread while keeping the other threads alive.

	return 0;
}


