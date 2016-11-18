/*
 * GoalKeeperGetUpFromRight.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperGetUpFromRight.h"

GoalKeeperGetUpFromRight::GoalKeeperGetUpFromRight() {
	m_GKStateName = GKState_GetUpFromRight;
}

GoalKeeperGetUpFromRight::~GoalKeeperGetUpFromRight() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperGetUpFromRight::Run(){
	cout <<"in GoalKeeperGetUpFromRight "<<endl;
	//Motion::GetInstance()->GetUp();
	//add turn to the left
}
