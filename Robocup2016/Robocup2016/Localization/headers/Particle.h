#ifndef _PARTICALE_HEADER_
#define _PARTICALE_HEADER_
struct Particle
{
	Particle(float _x=0, float _y=0, float _angle=0) :x(_x), y(_y), angle(_angle),weight(0) {}
	float x;
	float y;
	float angle;
	float weight;
};

#endif
