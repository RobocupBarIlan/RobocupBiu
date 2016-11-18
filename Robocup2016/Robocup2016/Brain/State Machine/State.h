#ifndef _State_H_
#define _State_H_

#include "../../Motion/Motion.h"
#include "../../Vision/Vision.h"
#include "../../Log/Log.h"

enum StateName
{
	State_Init,
	State_GoToBall,
	State_ChangeSpot,
	State_Kick,
	State_LookForBall,
	State_LookForGoal,
	State_GoToBasePosition,
	State_Finished
};

static const int TiltMax = -10;
static const int TiltMin = -54;
static const int PanMaxLeft = 67;
static const int PanMaxRight = -67;

class State
{
public:
	virtual ~State();

	virtual void Run() = 0;

	virtual string NameToString();

	StateName Name();

	virtual string GetStateVariable();

	string StateNamesStrings[8] = {"Init",
								"GoToBall",
								"ChangeSpot",
								"Kick",
								"LookForBall",
								"LookForGoal",
								"GoToBasePosition",
								"Finished"};

protected:
	State();

	Motion* m_Motion;
	Vision* m_Vision;

	void WriteLine(string message);
	StateName m_StateName;
	string m_stateVariable;

private:
	Log* Logger;
};

#endif


