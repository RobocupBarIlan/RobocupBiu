#include "GoToBall.h"
#include <cstdio>
#include <ctime>
#include "../../BrainData.h"

using namespace std;

GoToBall::GoToBall()
{
	m_StateName = State_GoToBall;
}

GoToBall::~GoToBall()
{

}

void GoToBall::Run()
{
	float angleToBall = m_Motion->GetHeadTilt().Pan;
	//cout<<"pan is " << angleToBall << endl;
	m_Motion->TurnByAngle(angleToBall); //turn to ball
	//start timing
	clock_t startingTime;
	double duration = 0;
	startingTime = clock();

	m_Motion->StartWalking();

	float currentDistance = m_Vision->Ball->Get()->Distance();
	float lastDistance = m_Vision->Ball->Get()->Distance();
	int counter = 0;

	while (currentDistance > 0 && MotionStatus::FALLEN == STANDUP)
	{
		if (!CenterBall())
		{
			//m_Motion->GetInstance()->StopWalking();
		}
		//Head::GetInstance()->
		//m_Motion->SetHeadTilt(HeadTilt((m_Motion->GetHeadTilt().Tilt - 1), m_Motion->GetHeadTilt().Pan));
		currentDistance = m_Vision->Ball->Get()->Distance();
		if (lastDistance != currentDistance)
		{
			lastDistance = currentDistance;
			cout << "distance from ball is " << m_Vision->Ball->Get()->Distance() << endl;
		}
		while (counter < 3 && currentDistance <= 0)
		{
			usleep(1000*1000);
			currentDistance = m_Vision->Ball->Get()->Distance();
			counter++;
			cout << "Counter is " << counter <<endl;
		}
		if (counter == 3)
		{
			break;
		}
		else
		{
			counter = 0;
		}
		//keep walking towards the ball
	}

	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable = "BallLost";
	}

	if (m_Vision->Ball->Get()->Distance() <= 0)
	{
		//pause timing
		duration += (clock() - startingTime ) / (double) CLOCKS_PER_SEC;
		usleep(500*1000);
		m_Motion->StopWalking();
		usleep(1000*1000);
		//restart timing
		startingTime = clock();
		//m_Motion->StartWalking(DISTANCE_WHEN_LOSING_BALL);
		//m_Motion->StopWalking();
		//usleep(300*1000);
		//m_Motion->RunAction(ActionPage::Init);
		//m_Motion->RunAction(ActionPage::BendToBall);
		//usleep(300*1000);
		//if (m_Vision->Ball->Get()->IsDetected)
		//{
			m_stateVariable = "InKickDistance";
		//}
		//else
		//{
			//m_stateVariable = "BallLost";
		//}
		//m_Motion->RunAction(ActionPage::Init);
	}

	//stop timing
	duration += (clock() - startingTime) / (double)CLOCKS_PER_SEC;
	BrainData::GetInstance()->UpdateAll(duration, angleToBall);
}


bool GoToBall::CenterBall()
{
	WriteLine("Centering ball");
	//usleep(3000*1000);
	Point2f location = m_Vision->Ball->Get()->Location;
	float tilt = m_Motion->GetHeadTilt().Tilt;
	float pan = m_Motion->GetHeadTilt().Pan;

	//TODO- check when x==-1 and y==-1

	while (location.x < 290 || location.x > 350 || location.y < 210 || location.y > 270)
	{
		if (location.x == -1 || location.y == -1)
		{
			return false;
		}
		if (location.x < 290)
		{
			//turn head right
			pan +=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));

		}
		else if (location.x > 350)
		{
			//turn head left
			pan -=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}

		if (location.y < 210)
		{
			//turn head up
			tilt +=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		else if (location.y > 270)
		{
			//turn head down
			tilt -=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		usleep(100*1000);
		location = m_Vision->Ball->Get()->Location;
		//cout << "x " << location.x <<endl;
		//cout << "y " << location.y <<endl;
	}
	return true;
}

