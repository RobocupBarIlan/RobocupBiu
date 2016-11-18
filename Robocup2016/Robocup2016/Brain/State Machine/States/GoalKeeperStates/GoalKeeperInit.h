/*
 * GoalKeeperInit.h
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERINIT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERINIT_H_

#include "../../GoalKeeperState.h"

class GoalKeeperInit: public GoalKeeperState
{
public:
	GoalKeeperInit();
	virtual ~GoalKeeperInit();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERINIT_H_ */
