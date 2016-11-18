#include "StateMachine.h"
#include "States/Init.h"
#include "States/ChangeSpot.h"
#include "States/LookForBall.h"
#include "States/GoToBall.h"
#include "States/Kick.h"
#include "States/LookForGoal.h"
#include "States/GoToBasePosition.h"
#include "../../Communication/Communication.h"
#include "../../Communication/RoboCupGameControlData.h"

StateMachine::StateMachine()
{
	m_currentState = new Init();
}

StateMachine::~StateMachine()
{
}

void StateMachine::SetCurrent(State* state)
{
	m_currentState = state;
}

void StateMachine::Run()
{
	StateName stateName;
	string stateVariable = "";
	RoboCupGameControlData refereeData;

	while (m_currentState != NULL)
	{
		////// FALL CHECK ////////////

		if (MotionStatus::FALLEN != STANDUP)
		{
			Motion::GetInstance()->GetUp();
			m_currentState = new LookForBall(); //TODO - check
		}

		////////////// REFEREE MESSAGE ///////////////////////

		if (IsNewRefereeMsg())
		{
			refereeData = Communication::GetInstance()->ReadDataAndClearFlag();
			switch (refereeData.state)
			{
				case STATE_INITIAL:
					break;

				case STATE_READY:
					m_currentState = new GoToBasePosition();
					break;

				case STATE_SET:
					m_currentState = new Init();
					break;

				case STATE_PLAYING:
					m_currentState = new LookForBall();
					break;

				case STATE_FINISHED:
					stateName = State_Finished;
					break;
			}
		}

		m_currentState->Run();
		stateName = m_currentState->Name();
		//cout << "State name is " << m_currentState->NameToString() << endl;
		stateVariable = m_currentState->GetStateVariable();
		//cout << "stateVariable is " << stateVariable;

		/*if (m_currentState != NULL)
		{
			cout << "deleteing" << endl;
			delete m_currentState;
		}*/

		switch (stateName)
		{
		case State_Init:
			/*while (!IsNewRefereeMsg())
			{
				//wait for game to start
				cout << "Waiting for game to start..." << endl;
			}
			if (Communication::GetInstance()->ReadDataAndClearFlag().state == STATE_PLAYING)
			{
				m_nextState = new LookForBall();
			}*/
			m_nextState = new LookForBall(); //in case we want to ignore the referee
			break;

		case State_LookForGoal:
			if (stateVariable == "GoalFound")
			{
				m_nextState = new Kick(true);
			}
			else if (stateVariable == "GoalNotFound")
			{
				m_nextState = new Kick(false);
			}
			break;

		case State_LookForBall:
			if (stateVariable == "BallFound")
			{
				m_nextState = new GoToBall();
			}
			else if (stateVariable == "BallNotFound")
			{
				//keep looking
				m_nextState =  new ChangeSpot();
			}
			else if(stateVariable == "FellDown")
			{
				m_nextState = new LookForBall();
			}
			else if (stateVariable == "BallLost")
			{
				m_nextState = new LookForBall();
			}
			break;

		case State_GoToBall:
			if (stateVariable == "InKickDistance")
			{
				m_nextState = new Kick(true);
			}
			else if (stateVariable == "BallLost")
			{
				m_nextState = new LookForBall();
			}
			break;

		case State_Kick:
			m_nextState = new LookForBall();
			break;

		case State_ChangeSpot:
			m_nextState = new LookForBall();
			break;

		case State_GoToBasePosition:
			m_nextState = new Init();
			break;

		case State_Finished:
			m_nextState = NULL;

		default:
			m_nextState = NULL;
			break;
		}
		m_currentState = m_nextState;
	}
}

bool StateMachine::IsNewRefereeMsg()
{
	return Communication::GetInstance()->getFlag();
}
