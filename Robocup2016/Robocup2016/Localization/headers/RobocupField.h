#ifndef _POIS_
#define _POIS_
#include <vector>
#include "PointOfInterest.h"
using namespace std;
	
class RobocupField
{

public:
	
	enum FieldConsts {
		A = 900,
		B = 600,
		C = 50,
		D = 180,
		E = 60,
		F = 345,
		G = 180,
		H = 150,
		I = 70
	};

	enum POINumber {
		CPOICount = 1,
		PPOICount = 2,
		DPOICount = 2,
		TPOICount = 10,
		LPOICount = 12,
		AllCount = 27
	};

	static PointOfInterest LTypePOIs[LPOICount];
	static PointOfInterest TTypePOIs[TPOICount];
	static PointOfInterest CirclePOIs[CPOICount];
	static PointOfInterest DotPOIs[DPOICount];
	static PointOfInterest PlusPOIs[PPOICount];
	static PointOfInterest AllPOIs[AllCount];


};

#endif