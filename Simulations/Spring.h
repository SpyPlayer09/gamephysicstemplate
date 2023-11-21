#pragma once
#include "Simulator.h"

class Spring
{
public:
	int masspoint1;
	int masspoint2;
	float initialLength;

	float stiffness;

	Spring(int masspoint1, int masspoint2, float stiffness, float initialLength);

	void draw(DrawingUtilitiesClass* DUC, Vec3 position1, Vec3 position2);
};

