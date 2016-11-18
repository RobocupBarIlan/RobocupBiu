/*
 * GoalKeeperGetUpFromLeft.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMLEFT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMLEFT_H_

#include "../../GoalKeeperState.h"

class GoalKeeperGetUpFromLeft: public GoalKeeperState {
public:
	GoalKeeperGetUpFromLeft();
	virtual ~GoalKeeperGetUpFromLeft();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMLEFT_H_ */
