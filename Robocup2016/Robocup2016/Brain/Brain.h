#include "../Log/Log.h"
#include "State Machine/StateMachine.h"
#include "State Machine/GoalKeeperStateMachine.h"
#include "Goalkeeper/BallMovementCalculator.h"


#pragma once

class Brain
{
public:
	// Singletone
	static Brain* GetInstance();

	~Brain();

	void RunThread();
	void StartPlay();

	bool isDebug;

private:
	Brain();
	static Brain* m_instance;
};

