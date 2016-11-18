#include "Kick.h"
#include "../../BrainData.h"

Kick::Kick(bool isGoalFound)
{
	m_StateName = State_Kick;
	m_isGoalFound = isGoalFound;
}

Kick::~Kick()
{
}

void Kick::Run()
{
	//If goal is found
	//if (m_isGoalFound)
		turnToGoal(); //turn around the ball to face the goal
	//kick
	m_Motion->RunAction(ActionPage::BendToBall);
	usleep(300*1000);
	if (m_Vision->Ball->Get()->IsDetected)
	{
		if (m_Vision->Ball->Get()->Location.x >320)
		{
			m_Motion->RunAction(ActionPage::LeftKick);
		}
		else
		{
			m_Motion->RunAction(ActionPage::RightKick);
		}
	}

}

void Kick::turnToGoal()
{
	double angle = BrainData::GetInstance()->AngleFromGoal;
	m_Motion->GetInstance()->TurnToGoal(angle);

	/*
	///// FACING ENEMY GOAL ////////
	if((DegreeBetweenRobotAndGoal >= 0 && DegreeBetweenRobotAndGoal <= 90))
	{
		//Right circle around the ball - Left turn of -Degree
	}
	else if((DegreeBetweenRobotAndGoal < 0 && DegreeBetweenRobotAndGoal >= -90))
	{
		//Left circle around the ball - Right turn of Degree
	}

	///// FACING OWN GOAL ///////
	else if((DegreeBetweenRobotAndGoal > 90 && DegreeBetweenRobotAndGoal <= 180))
	{
		//Right circle around the ball - Left turn of -Degree
	}
	else if((DegreeBetweenRobotAndGoal < -90 && DegreeBetweenRobotAndGoal >= -180))
	{
		//Left circle around the ball - Right turn of Degree

	}*/


}





