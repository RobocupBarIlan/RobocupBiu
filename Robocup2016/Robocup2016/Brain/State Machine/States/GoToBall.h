#include "../State.h"


class GoToBall : public State
{
	public:
	GoToBall();
	virtual ~GoToBall();

	virtual void Run();
	bool CenterBall();
	const double DISTANCE_WHEN_LOSING_BALL = 7; //21; //cm
};

