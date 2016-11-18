/*
 * GoalKeeperLookForBall.h
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERLOOKFORBALL_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERLOOKFORBALL_H_

#include "../../GoalKeeperState.h"

class GoalKeeperLookForBall: public GoalKeeperState {
	private:
		DetectedBall* m_GKdetectedBall;
	public:
		GoalKeeperLookForBall();
		virtual ~GoalKeeperLookForBall();
		virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERLOOKFORBALL_H_ */
