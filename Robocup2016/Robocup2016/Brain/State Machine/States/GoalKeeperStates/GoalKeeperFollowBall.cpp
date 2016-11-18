/*
 * GoalKeeperFollowBall.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperFollowBall.h"


GoalKeeperFollowBall::GoalKeeperFollowBall():MIN_MS_TO_JUMP(2500) {
	m_GKStateName = GKState_FollowBall;
	Vision::GetInstance()->StartBallMovementCalc();
}

GoalKeeperFollowBall::~GoalKeeperFollowBall() {
	Vision::GetInstance()->StopBallMovementCalc();
}

void GoalKeeperFollowBall::Run(){
	cout <<"in GoalKeeperFollowBall "<<endl;
	int cnt_try=0;
	BallMovement ballMovement = Vision::GetInstance()->BallMovementCalc.SafeRead();
	while ((((ballMovement.DetectionStatus != Valid) || (ballMovement.MsToJump > MIN_MS_TO_JUMP) || (ballMovement.MsToJump < 0)) && (cnt_try<20))  ){
		cnt_try++;
		ballMovement = Vision::GetInstance()->BallMovementCalc.SafeRead();
	}

	cout <<"in GoalKeeperFollowBall after while"<<endl;

	switch (ballMovement.Direction){
		case LEFT:{
			cout << "ballMovement.Direction left"<<endl;
			m_GKStateVariable = "JumpLeft";
			break;}
		case RIGHT:{
			cout << "ballMovement.Direction right"<<endl;
			m_GKStateVariable = "JumpRight";
			break;}
		case CENTER:{
			cout << "ballMovement.Direction center"<<endl;
			m_GKStateVariable = "Wave";
			break;}
		default:{ /*also covers if cnt_try is bigger than 3*/
			cout << "ballMovement.Direction none"<<endl;
			m_GKStateVariable = "None";
			break;}
	}
	Vision::GetInstance()->StopBallMovementCalc(); /*instead of destructor*/
}
