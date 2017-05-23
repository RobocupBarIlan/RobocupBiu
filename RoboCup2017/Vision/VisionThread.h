/*
 * VisionThread.h
 *
 *
 */


#ifndef NULL
#define NULL   ((void *) 0)
#endif

#ifndef VISION_VISIONTHREAD_H_
#define VISION_VISIONTHREAD_H_

#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <mutex>
#include <opencv2/opencv.hpp>
#include "BallDetector.h"

#define INIT_VALUE -1 //Will be used to initialize class members before running any code.
#define NOT_FOUND_OBJECT_VALUE -1 //Will be used when no object is found. we will set the appropriate class members to -1.

using namespace std;
class VisionThread { //Singleton - only one object should be instantiated!
private:
	pthread_t m_vision_thread; //Will control all the vision tasks.
	static int BallCenterX; //Will contain the ball center x coordinate after a signal to calculate the ball center is called. will contain INIT_VALUE
	static int BallCenterY; //Will contain the ball center y coordinate after a signal to calculate the ball center is called.
	static double BallDistance; //Will contain the distance towards the ball if ball is actually found.
    static VisionThread* Vision_Thread_Instance; //Contains the one and only possible instance of vision thread.
    static bool Is_Register_Signals_Done; //A flag to indicate registering signals is done.
    static void GetBallCenterInFrameAndDistance(); //This method called to handle a signal - GET_BALL_CENTER_IN_FRAME_AND_DISTANCE .
	VisionThread();

public:
	enum VISION_THREAD_SIGNALS { GET_BALL_CENTER_IN_FRAME_AND_DISTANCE=2, GET_GOAL_CENTER_IN_FRAME_AND_DISTANCE=3, GET_GOALKEEPER_CENTER_IN_FRAME_AND_DISTANCE=4};
	static std::mutex VisionMutex;  //Create a mutex for calls to the vision thread - so another thread (the brain for example) won't read wrong data - i.e to make the proccess thread-safe.
	pthread_t getVisionThread(); //Returns the vision_thread of type pthread_t class member.
	static VisionThread* GetVisionThreadInstance(); //This method makes sure we don't create more than 1 object of this class.
    static void RegisterSignals(); //This method registers all signals which can be sent to the vision thread.
    static void SignalCallbackHandler(int); //This method handles all the possible signals which can be sent to the vision thread.
	void init(); //This method initiates the vision thread.
	static bool IsRegisterSingalsDone(); //This method tells whether the RegisterSignals() method has already been called. It is crucial so we won't send signals before that is done.
	virtual ~VisionThread();

};

#endif /* VISION_VISIONTHREAD_H_ */
