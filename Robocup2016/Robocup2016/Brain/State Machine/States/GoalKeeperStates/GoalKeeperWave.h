/*
 * GoalKeeperWave.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERWAVE_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERWAVE_H_

#include "../../GoalKeeperState.h"

class GoalKeeperWave: public GoalKeeperState {
public:
	GoalKeeperWave();
	virtual ~GoalKeeperWave();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERWAVE_H_ */
