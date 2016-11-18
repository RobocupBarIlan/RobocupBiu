#pragma once
#include <vector>
#include <string>

using namespace std;

class PointOfInterest
{
public:
	enum POItypes {
		L_SHAPE = 0,
		T_SHAPE = 1,
		DOT_SHAPE = 2,
		PLUS_SHAPE = 3,
		CIRCLE_SHAPE = 4,
		GENERAL_SHAPE = 5,	// In case we can't really identify it
	};
	PointOfInterest(int _x,int _y, const POItypes _type, const string _name) :x(_x),y(_y), type(_type), name(_name) {}
	int x;
	int y;
	POItypes type;
	string name;

};
