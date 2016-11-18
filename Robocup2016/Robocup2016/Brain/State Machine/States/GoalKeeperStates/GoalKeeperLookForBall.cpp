/*
 * GoalKeeperLookForBall.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperLookForBall.h"

GoalKeeperLookForBall::GoalKeeperLookForBall() {
	m_GKStateName = GKState_LooKForBall;
	cout <<"inside GoalKeeperLookForBall constructor "<<endl;
}

GoalKeeperLookForBall::~GoalKeeperLookForBall() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperLookForBall::Run(){
	cout <<"inside GoalKeeperLookForBall Run"<<endl;
	float pan;
	float tilt;

	tilt = -30;
	pan = 0;
	usleep(3000*1000);
	while (!m_Vision->Ball->Get()->IsDetected && MotionStatus::FALLEN == STANDUP)
	{
		cout<< "GoalKeeperLookForBall Run: inside while"<<endl;
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		usleep(400*4*1000);
		if (pan > PanMaxLeft){
			pan = PanMaxRight;
		}
		else pan += 15;
	}

	if (MotionStatus::FALLEN != STANDUP){
		m_Motion->GetUp();
		m_GKStateVariable == "FellDown";
	}
	else{
		//Ball detected successfully
		WriteLine("Ball found");
		//LookForBall::CenterBall();
		m_GKStateVariable = "BallFound";
	}
	return;
}

