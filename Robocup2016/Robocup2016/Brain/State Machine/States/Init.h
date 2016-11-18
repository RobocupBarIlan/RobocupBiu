#ifndef _STATE_INIT_H
#define _STATE_INIT_H

#include "../State.h"

class Init: public State
{
public:
	Init();
	virtual ~Init();

	virtual void Run();
};

#endif
