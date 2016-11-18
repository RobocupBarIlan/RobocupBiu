/*
 * GoalKeeperStateMachine.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperStateMachine.h"

GoalKeeperStateMachine::GoalKeeperStateMachine() {
	m_GKCurrentState = new GoalKeeperInit();
}

GoalKeeperStateMachine::~GoalKeeperStateMachine() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperStateMachine::Run()
{
	GoalKeeperStateName GKStateName;
	string stateVariable = "";
	while (m_GKCurrentState != NULL)
	{
		if (MotionStatus::FALLEN != STANDUP){
			cout <<"in FALLEN != STANDUP state. suppose to get up. Init GoalKeeper"<<endl;
			//Motion::GetInstance()->GetUp();
			m_GKCurrentState = new GoalKeeperInit(); //TODO - check
		}

		m_GKCurrentState->Run();
		GKStateName = m_GKCurrentState->Name();
		stateVariable = m_GKCurrentState->GetStateVariable();
		cout<< "before switch case"<<endl;
		switch (GKStateName)
		{
		case GKState_Init:{
			cout<< "in init case"<<endl;
			m_GKNextState = new GoalKeeperLookForBall();
			cout<<"new state name "<<m_GKNextState->Name()<<endl;
			break;
		}
		case GKState_LooKForBall:{
			cout<< "in LooKForBall case"<<endl;
			if (stateVariable == "BallFound")
			{
				cout<< "in LooKForBall case inside if of BallFound"<<endl;
				m_GKNextState = new GoalKeeperFollowBall();
			}
			break;
		}
		case GKState_FollowBall:{
			cout<< "in GKState_FollowBall case"<<endl;
			if 		 (stateVariable == "JumpLeft")	m_GKNextState = new GoalKeeperJumpLeft();
			else if  (stateVariable == "JumpRight")	m_GKNextState = new GoalKeeperJumpRight();
			else if	 (stateVariable == "Wave")		m_GKNextState = new GoalKeeperWave();
			else /*None*/ m_GKNextState = new GoalKeeperLookForBall();
			break;
		}
		case GKState_JumpRight:{
			cout<< "in GKState_JumpRight case"<<endl;
			/*Motion jump to right - complete*/
			m_GKNextState = new GoalKeeperGetUpFromRight();
			break;
		}
		case GKState_JumpLeft:{
			cout<< "in GKState_JumpLeft case"<<endl;
			/*Motion jump to left - complete*/
			m_GKNextState = new GoalKeeperGetUpFromLeft();
			break;
		}
		case GKState_Wave:{
			cout<< "in GKState_Wave case"<<endl;
			/*Motion wave or bend/sit - need to complete*/
			m_GKNextState = new GoalKeeperGetUpStraight();
			break;
		}
		case GKState_GetUpFromRight:{
			cout<< "in GKState_GetUpFromRight case"<<endl;
			/*Motion get up + return to goal center- need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;
		}
		case GKState_GetUpFromLeft:{
			cout<< "in GKState_GetUpFromLeft case"<<endl;
			/*Motion get up + return to goal center- need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;
		}
		case GKState_GetUpStraight:{
			cout<< "in GKState_GetUpStraight case"<<endl;
			/*Motion get up - need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;
		}
		default:{
			cout<< "in default case"<<endl;
			m_GKNextState = NULL;
			break;
		}
		}
		if (stateVariable == "FellDown"){
			cout<< "after switch case. inside Felldown"<<endl;
			m_GKNextState = new GoalKeeperLookForBall();
		}
		cout<<"current state "<<m_GKCurrentState->Name()<<endl;
		/*if (m_GKCurrentState!=NULL){
			cout<< "inside m_GKCurrentState!=NULL"<<endl;
			delete m_GKCurrentState;
			cout<< "after deletion state name "<<m_GKCurrentState->Name()<<endl;
		} 41 246 14*/
		m_GKCurrentState = m_GKNextState;
	}
}
