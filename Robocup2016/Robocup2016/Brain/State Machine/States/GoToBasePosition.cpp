#include "GoToBasePosition.h"
#include "../../BrainData.h"

GoToBasePosition::GoToBasePosition()
{
	m_StateName = State_GoToBasePosition;
}

GoToBasePosition::~GoToBasePosition() {
	// TODO Auto-generated destructor stub
}

void GoToBasePosition::Run()
{
	double angleToDefPos = BrainData::GetInstance()->AngleToDefaultPos;
	m_Motion->TurnByAngle(angleToDefPos);
	m_Motion->StartWalking(BrainData::GetInstance()->DistanceToDefaultPos);
	double angleBackToGoal;
	if (angleToDefPos > 0)
	{
		angleBackToGoal = 180 - angleToDefPos;
	}
	else
	{
		angleBackToGoal = -180 - angleToDefPos;
	}
	m_Motion->TurnByAngle(angleBackToGoal);

	BrainData::GetInstance()->Reset();
}
