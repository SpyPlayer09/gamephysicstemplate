#pragma once
#include "Simulator.h"


class MassPoint
{
public:
	Vec3 position;
	Vec3 velocity;
	bool isFixed;
	int mass;


	MassPoint(Vec3 position, Vec3 velocity, float mass, bool isFixed);

};

