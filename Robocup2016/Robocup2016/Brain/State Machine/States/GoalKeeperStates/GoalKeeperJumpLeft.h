/*
 * GoalKeeperJumpLeft.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPLEFT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPLEFT_H_

#include "../../GoalKeeperState.h"


class GoalKeeperJumpLeft: public GoalKeeperState {
public:
	GoalKeeperJumpLeft();
	virtual ~GoalKeeperJumpLeft();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPLEFT_H_ */
