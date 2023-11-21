#pragma once
#include "Simulator.h"
#include "Spring.h"
#include "MassPoint.h"


class Plane
{
public:
	enum Orientation {
		XY, XZ, YZ
	};
	
public:
	//Orientation of Plane, relative Position to the origin and collider direction, if true collider points to origin
	Plane(Orientation orientation, float relativePosition);

	Orientation orientation;
	float relativePosition;


	bool checkCollision(MassPoint& mp);
	void resolveCollision(MassPoint& mp);
};


