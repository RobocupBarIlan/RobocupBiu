/*
 * GoalKeeperJumpRight.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperJumpRight.h"

GoalKeeperJumpRight::GoalKeeperJumpRight() {
	m_GKStateName = GKState_JumpRight;
}

GoalKeeperJumpRight::~GoalKeeperJumpRight() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperJumpRight::Run(){
	cout <<"in GoalKeeperJumpRight Run"<<endl;
	m_Motion->RunAction(ActionPage::RightDive);
}
