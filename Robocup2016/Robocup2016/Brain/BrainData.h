
class BrainData
{
	public:
	double DefaultDistanceToGoal;
	double DistanceToGoal;
	double AngleFromGoal;
	double AngleToDefaultPos;
	double DistanceToDefaultPos;
	double posX,posY;
	double DistanceWalked;
	const double WALK_SPEED = 6; //cm in 1sec
	const double DEFAULT_DISTANCE_TO_GOAL = 600; //cm


	BrainData();
	~BrainData();

	static BrainData* GetInstance();
	void CalcAngleToGoal(double distanceWalked,double angleWalked);
	void CalcDistanceToGoal(double distanceWalked,double angleWalked);
	double CalcWalkedDistance(double timeWalked);
	void DecideInitPos();
	void UpdatePos(double distanceWalked,double angleWalked);
	void UpdateAll(double timeWalked, double angleWalked);
	void CalcWayBackToDefaultPos();
	void Reset();

	private:
	static BrainData* m_instance;


};
