#include "../State.h"


class ChangeSpot : public State
{
	public:
	ChangeSpot();
	virtual ~ChangeSpot();

	virtual void Run();
};

