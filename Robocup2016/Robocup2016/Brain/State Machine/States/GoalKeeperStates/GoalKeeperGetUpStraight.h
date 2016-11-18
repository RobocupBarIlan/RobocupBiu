/*
 * GoalKeeperGetUpStraight.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPSTRAIGHT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPSTRAIGHT_H_

#include "../../GoalKeeperState.h"

class GoalKeeperGetUpStraight: public GoalKeeperState {
public:
	GoalKeeperGetUpStraight();
	virtual ~GoalKeeperGetUpStraight();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERGETUPSTRAIGHT_H_ */

