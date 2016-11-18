/*
 * GoalKeeperFollowBall.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_

#include "../../GoalKeeperState.h"
#include "../../../../Vision/Vision.h"


class GoalKeeperFollowBall: public GoalKeeperState
{
	private:
		const double MIN_MS_TO_JUMP;
		BallMovementCalculator m_BallMovementCalculator;
	public:
		GoalKeeperFollowBall();
		virtual ~GoalKeeperFollowBall();
		virtual void Run();
};


#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_ */
