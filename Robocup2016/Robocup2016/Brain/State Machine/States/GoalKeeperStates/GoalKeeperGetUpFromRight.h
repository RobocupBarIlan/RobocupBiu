/*
 * GoalKeeperGetUpFromRight.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMRIGHT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMRIGHT_H_

#include "../../GoalKeeperState.h"


class GoalKeeperGetUpFromRight: public GoalKeeperState {
public:
	GoalKeeperGetUpFromRight();
	virtual ~GoalKeeperGetUpFromRight();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPFROMRIGHT_H_ */
