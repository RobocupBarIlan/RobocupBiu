#include "../State.h"

class LookForGoal : public State
{
	public:
	LookForGoal();
	virtual ~LookForGoal();

	virtual void Run();
};
