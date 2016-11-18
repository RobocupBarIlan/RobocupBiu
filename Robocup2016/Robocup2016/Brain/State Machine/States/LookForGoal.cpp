#include "LookForGoal.h"

LookForGoal::LookForGoal()
{
	m_StateName = State_LookForGoal;
}

LookForGoal::~LookForGoal()
{
}

void LookForGoal::Run()
{
	float pan;
	float tilt;

	tilt = -20;
	pan = PanMaxRight;

	while (!m_Vision->Gate->Get()->IsDetected)
	{
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		usleep(300*1000);

		if (pan > PanMaxLeft)
		{
			pan = PanMaxRight;
			tilt += 20;
			if (tilt > 9)
			{
				m_stateVariable = "GoalNotFound";
				return;
			}
		}
		pan += 20;
	}

	//ball detected succesfully
	WriteLine("Goal found");
	//CenterBall();
	//angleToBall = m_Motion->GetHeadTilt()->Pan;
	m_stateVariable = "GoalFound";
	return;
}


