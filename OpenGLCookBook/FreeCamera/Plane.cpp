#include "Plane.h"

static const float kEpsilon = 0.0001f;

Plane::Plane()
: mN(glm::vec3(0, 1, 0)), mDist(0)
{
}


Plane::~Plane()
{
}

Plane::Plane(const glm::vec3& normal, const glm::vec3& p)
: mN(normal), mDist(-glm::dot(normal, p))
{

}

Plane Plane::fromPoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	Plane pl;
	glm::vec3 e1 = p2 - p1;
	glm::vec3 e2 = p3 - p1;
	pl.mN = glm::normalize(glm::cross(e1, e2));
	pl.mDist = -glm::dot(pl.mN, p1);
	return pl;
}

float Plane::distance(const glm::vec3& p)
{
	return glm::dot(mN, p) + mDist;
}

Plane::Where Plane::classify(const glm::vec3& p)
{
	float res = distance(p);
	if (res > kEpsilon)
		return kFront;
	else if (res < kEpsilon)
		return kBack;
	else
		return kCoPlanar;
}