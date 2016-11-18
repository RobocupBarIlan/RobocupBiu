#include "../State.h"

#ifndef BRAIN_STATE_MACHINE_STATES_GOTOBASEPOSITION_H_
#define BRAIN_STATE_MACHINE_STATES_GOTOBASEPOSITION_H_

class GoToBasePosition : public State
{
public:
	GoToBasePosition();
	virtual ~GoToBasePosition();
	virtual void Run();
};

#endif /* BRAIN_STATE_MACHINE_STATES_GOTOBASEPOSITION_H_ */
