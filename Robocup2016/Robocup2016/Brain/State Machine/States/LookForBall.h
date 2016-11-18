#include "../State.h"

class LookForBall : public State
{
	public:
	LookForBall();
	virtual ~LookForBall();

	virtual void Run();
	bool CenterBall();
};
