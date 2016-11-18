/*
 * GoalKeeperStateMachine.h
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_GOALKEEPERSTATEMACHINE_H_
#define BRAIN_STATE_MACHINE_GOALKEEPERSTATEMACHINE_H_

#include "StateMachine.h"
#include "States/GoalKeeperStates/GoalKeeperInit.h"
#include "States/GoalKeeperStates/GoalKeeperLookForBall.h"
#include "States/GoalKeeperStates/GoalKeeperFollowBall.h"
#include "States/GoalKeeperStates/GoalKeeperGetUpFromLeft.h"
#include "States/GoalKeeperStates/GoalKeeperGetUpFromRight.h"
#include "States/GoalKeeperStates/GoalKeeperGetUpStraight.h"
#include "States/GoalKeeperStates/GoalKeeperJumpRight.h"
#include "States/GoalKeeperStates/GoalKeeperJumpLeft.h"
#include "States/GoalKeeperStates/GoalKeeperWave.h"

class GoalKeeperStateMachine: public StateMachine {
public:
	GoalKeeperStateMachine();
	virtual ~GoalKeeperStateMachine();
	virtual void Run();

private:
	GoalKeeperState* m_GKCurrentState;
	GoalKeeperState* m_GKNextState;
};

#endif /* BRAIN_STATE_MACHINE_GOALKEEPERSTATEMACHINE_H_ */
