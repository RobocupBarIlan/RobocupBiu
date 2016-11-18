/*
 * GoalKeeperWave.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperWave.h"

GoalKeeperWave::GoalKeeperWave() {
	m_GKStateName = GKState_Wave;
}

GoalKeeperWave::~GoalKeeperWave() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperWave::Run(){
	cout <<"inside GoalKeeperWave Run"<<endl;
	//Action::GetInstance()->Start(Page_Wave);
	//m_Motion->WaitForActionFinish();
}
