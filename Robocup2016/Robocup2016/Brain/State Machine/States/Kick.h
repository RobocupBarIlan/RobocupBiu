#include "../State.h"


class Kick : public State
{
	public:
	Kick(bool isGoalFound);
	virtual ~Kick();

	virtual void Run();

	private:

	bool m_isGoalFound;
	void turnToGoal();
};

