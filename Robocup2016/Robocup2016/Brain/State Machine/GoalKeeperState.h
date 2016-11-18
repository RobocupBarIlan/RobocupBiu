/*
 * GoalKeeperState.h
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPERSTATE_H_
#define BRAIN_GOALKEEPERSTATE_H_

#include "State.h"

enum GoalKeeperStateName{
	GKState_Init,
	GKState_LooKForBall,
	GKState_FollowBall,
	GKState_JumpRight,
	GKState_JumpLeft,
	GKState_Wave,
	GKState_GetUpFromRight,
	GKState_GetUpFromLeft,
	GKState_GetUpStraight
};

class GoalKeeperState: public State {
public:
	GoalKeeperState();
	virtual ~GoalKeeperState();

	virtual void Run() = 0;

	virtual string NameToString();

	GoalKeeperStateName Name();

	virtual string GetStateVariable();

	string GKStateNamesStrings[9] = {"GKState_Init",
								"GKState_LooKForBall",
								"GKState_FollowBall",
								"GKState_JumpRight",
								"GKState_JumpLeft",
								"GKState_Wave",
								"GKState_GetUpFromRight",
								"GKState_GetUpFromLeft",
								"GKState_GetUpStraight"
								  };
	protected:
		Motion* m_Motion;
		Vision* m_Vision;
		void WriteLine(string message);
		GoalKeeperStateName m_GKStateName;
		string m_GKStateVariable;
	private:
		Log* Logger;
	};


#endif /* BRAIN_GOALKEEPERSTATE_H_ */
