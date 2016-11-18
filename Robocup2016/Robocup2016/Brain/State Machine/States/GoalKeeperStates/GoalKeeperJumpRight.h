/*
 * GoalKeeperJumpRight.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPRIGHT_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPRIGHT_H_

#include "../../GoalKeeperState.h"

class GoalKeeperJumpRight: public GoalKeeperState {
public:
	GoalKeeperJumpRight();
	virtual ~GoalKeeperJumpRight();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERJUMPRIGHT_H_ */
