#include "LookForBall.h"
#include "../../BrainData.h"

LookForBall::LookForBall()
{
	m_StateName = State_LookForBall;
}

LookForBall::~LookForBall()
{
}

void LookForBall::Run()
{
	float pan;
	float tilt;

	tilt = TiltMin;
	pan = PanMaxRight;
	//usleep(3000*1000);
	while (!m_Vision->Ball->Get()->IsDetected && MotionStatus::FALLEN == STANDUP)
	{
		usleep(1000*1000);
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));

		//usleep(500*1000);
		if (pan > PanMaxLeft)
		{
			pan = PanMaxRight;
			tilt += 15;
			if (tilt > -10)
			{
				m_stateVariable = "BallNotFound";
				m_Motion->SetHeadTilt(HeadTilt(0, 0));
				return;
			}
		}
		pan += 15;
	}

	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable == "FellDown";
	}

	//Ball detected successfully
	WriteLine("Ball found");
	if (!CenterBall())
	{
		m_stateVariable = "BallLost";
	}
	else
	{
		m_stateVariable = "BallFound";
	}
	return;
}

bool LookForBall::CenterBall()
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
