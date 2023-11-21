#include "MassPoint.h"

MassPoint::MassPoint(Vec3 position, Vec3 velocity, float mass, bool isFixed)
{
	this->position = position;
	this->isFixed = isFixed;
	this->velocity = velocity;
	this->mass = mass;
}
