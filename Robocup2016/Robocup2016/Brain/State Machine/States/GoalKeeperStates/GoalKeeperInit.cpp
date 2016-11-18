/*
 * GoalKeeperInit.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperInit.h"

GoalKeeperInit::GoalKeeperInit() {
	m_GKStateName = GKState_Init;
}

GoalKeeperInit::~GoalKeeperInit() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperInit::Run() {
	cout <<"in GoalKeeperInit "<<endl;

}
