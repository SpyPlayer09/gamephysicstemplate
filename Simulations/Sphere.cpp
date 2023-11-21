#include "Sphere.h"

Sphere::Sphere(Vec3 center, float radius)
{
	this->center = center;
	this->radius = radius;
}

bool Sphere::checkCollision(MassPoint& mp)
{
	float distance = norm(mp.position - center);
	return distance < radius;
}

void Sphere::resolveCollision(MassPoint& mp)
{
	if (!checkCollision(mp)) return;

	Vec3 normal = getNormalized(mp.position - center);
	float distance = radius - norm(mp.position - center);
	mp.velocity = reflectVector(mp.velocity, normal);
	mp.position += normal * distance;
}

void Sphere::Draw(DrawingUtilitiesClass* DUC)
{
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0.1, 0.1, 1));
	DUC->drawSphere(center, radius);
}

