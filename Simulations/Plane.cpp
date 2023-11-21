#include "Plane.h"

Plane::Plane(Orientation orientation, float relativePosition)
{
	this->orientation = orientation;
	this->relativePosition = relativePosition;
}

bool Plane::checkCollision(MassPoint& mp) {
	//This is only for Straight walls and floors
	float compValue;
	switch (orientation)
	{
	case Plane::XY:
		compValue = mp.position.z;
		break;
	case Plane::XZ:
		compValue = mp.position.y;
		break;
	case Plane::YZ:
		compValue = mp.position.x;
		break;
	default:
		throw invalid_argument{ "Enum out of bounds" };
	}

	if (relativePosition <= 0) {
		return relativePosition > compValue;
	}
	else {
		return relativePosition < compValue;
	}
}

void Plane::resolveCollision(MassPoint& mp)
{
	if (!checkCollision(mp)) return;
	Vec3 normal;
	float currentPosition;

	switch (orientation)
	{
	case Plane::XY:
		normal = Vec3(0, 0, 1);
		currentPosition = mp.position.z;
		break;
	case Plane::XZ:
		normal = Vec3(0, 1, 0);
		currentPosition = mp.position.y;
		break;
	case Plane::YZ:
		normal = Vec3(1, 0, 0);
		currentPosition = mp.position.x;
		break;
	default:
		throw invalid_argument{ "Enum out of bounds" };
	}

	float distance = abs(currentPosition - relativePosition);
	mp.position += distance * normal;
	mp.velocity = reflectVector(mp.velocity, normal);
}


