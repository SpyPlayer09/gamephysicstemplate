#include "Spring.h"

Spring::Spring(int masspoint1, int masspoint2, float stiffness, float initialLength)
{
	this->masspoint1 = masspoint1;
	this->masspoint2 = masspoint2;
	this->stiffness = stiffness;
	this->initialLength = initialLength;
}

void Spring::draw(DrawingUtilitiesClass* DUC, Vec3 position1, Vec3 position2)
{
	DUC->drawLine(position1, Vec3(0, 1, 1), position2, Vec3(1, 0, 1));
}
