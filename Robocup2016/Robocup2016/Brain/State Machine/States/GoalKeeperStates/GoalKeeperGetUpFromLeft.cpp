/*
 * GoalKeeperGetUpFromLeft.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperGetUpFromLeft.h"

GoalKeeperGetUpFromLeft::GoalKeeperGetUpFromLeft() {
	m_GKStateName = GKState_GetUpFromLeft;
}

GoalKeeperGetUpFromLeft::~GoalKeeperGetUpFromLeft() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperGetUpFromLeft::Run(){
	cout <<"in GoalKeeperGetUpFromLeft "<<endl;
	cout <<"in GoalKeeperGetUpFromLeft suppose to get up from left "<<endl;
	//Motion::GetInstance()->GetUp();
	//add turn to the right
}
