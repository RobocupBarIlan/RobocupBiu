/*
 * VisionThread.cpp
 *
 *  Created on: May 22, 2017
 *      Author: root
 */

#include "VisionThread.h"

	// Global static pointer used to ensure a single instance of the class:
	VisionThread* VisionThread::Vision_Thread_Instance = NULL;
	bool VisionThread::Is_Register_Signals_Done=false;
	std::mutex VisionThread::VisionMutex;
	int VisionThread::BallCenterX;
	int VisionThread::BallCenterY;
	double VisionThread::BallDistance;
VisionThread::VisionThread() {

}

VisionThread::~VisionThread() {
	// TODO Auto-generated destructor stub
}


void *run(void *arg)
{
	VisionThread::RegisterSignals();
	while(true)
	{
		usleep(100000*1000);
	}
	pthread_exit(NULL);
}

/*
 * Sets up a new thread - the vision thread.
 */
void VisionThread::init()
{
	static int NUM_INIT_CALLS=0; //This variable is used to check that the init() method is called only once!
	if(NUM_INIT_CALLS==0) //If it's first time init() is called:
	{
		NUM_INIT_CALLS++;
		//Initialize static members of this class (data):
		BallCenterX=INIT_VALUE;
		BallCenterY=INIT_VALUE;
		BallDistance=INIT_VALUE;
		int status = pthread_create(&m_vision_thread, NULL, run,  (void*) "vision thread");
		if(status) //If could not start a new thread - notify me:
		{
			cout<<"Error! Could not initiate the vision thread :("<<endl;
		}
		else
		{
			cout<<"*	Vision thread successfully initiated"<<endl;
		}
	}
}
/*
 * Returns the vision thread object.
 */
pthread_t VisionThread::getVisionThread()
{
	return this->m_vision_thread;
}

	/* This function is called to create an instance of the class.
	    Calling the constructor publicly is not allowed (it is private!).
	*/

VisionThread* VisionThread::GetVisionThreadInstance()
{
		   if ( Vision_Thread_Instance==NULL)   // Allow only 1 instance of this class
			   Vision_Thread_Instance = new VisionThread();


		   return Vision_Thread_Instance;
}

/*
 * Registers all possible calls to the vision thread for data:
 */
void VisionThread::RegisterSignals()
{
	   // Register signals and signal handler:
	   signal(GET_BALL_CENTER_IN_FRAME_AND_DISTANCE , SignalCallbackHandler);
	   signal(GET_GOAL_CENTER_IN_FRAME_AND_DISTANCE , SignalCallbackHandler);
	   signal(GET_GOALKEEPER_CENTER_IN_FRAME_AND_DISTANCE , SignalCallbackHandler);
	Is_Register_Signals_Done=true;
}

void VisionThread::SignalCallbackHandler(int signum)
{
	switch(signum)
	{
		case GET_BALL_CENTER_IN_FRAME_AND_DISTANCE:
				VisionThread::GetBallCenterInFrameAndDistance();
				break;
		case GET_GOAL_CENTER_IN_FRAME_AND_DISTANCE:
			break;
		case GET_GOALKEEPER_CENTER_IN_FRAME_AND_DISTANCE:
			break;
	}
}

bool VisionThread::IsRegisterSingalsDone()
{
	return Is_Register_Signals_Done;
}


void VisionThread::GetBallCenterInFrameAndDistance()
{
	Point center;
	center=BallDetector::GetBallCenter();
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	//Critical code section - the set of ball center and distance values:
	VisionMutex.lock();
		BallCenterX=center.x;
		BallCenterY=center.y;
		//TODO - add the distance calculation function and then update here : m_ball_distance=distance.
	VisionMutex.unlock();
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	cout<<"center.x: "<<BallCenterX<<"center.y: "<<BallCenterY<<endl;
}
