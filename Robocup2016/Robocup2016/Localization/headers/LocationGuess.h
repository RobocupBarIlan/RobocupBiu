#ifndef _LOCATIONGUESS_HEADER_
#define _LOCATIONGUESS_HEADER_
struct LocationGuess
{
	LocationGuess(float _x=0, float _y=0, float _angle=0) :x(_x), y(_y), angle(_angle){}
	float x;
	float y;
	float angle; //angle
};

#endif
