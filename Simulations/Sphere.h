#pragma once
#include "Simulator.h"
#include "MassPoint.h"

class Sphere
{
public:
	Sphere(Vec3 center, float radius);

	Vec3 center;
	float radius;

	bool checkCollision(MassPoint& mp);
	void resolveCollision(MassPoint& mp);

	void Draw(DrawingUtilitiesClass* DUC);
};

