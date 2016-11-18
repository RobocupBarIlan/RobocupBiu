/*
 * GoalKeeperGetUpStraight.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperGetUpStraight.h"

GoalKeeperGetUpStraight::GoalKeeperGetUpStraight() {
	m_GKStateName = GKState_GetUpStraight;
}

GoalKeeperGetUpStraight::~GoalKeeperGetUpStraight() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperGetUpStraight::Run(){
	cout <<"in GoalKeeperGetUpStraight "<<endl;
	//Action::GetInstance()->Start(Page_Init);
	//m_Motion->WaitForActionFinish();
}
