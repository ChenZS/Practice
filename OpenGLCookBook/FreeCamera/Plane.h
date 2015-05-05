#pragma once
#include <glm/glm.hpp>


class Plane
{
	enum Where { kCoPlanar, kFront, kBack };
public:
	Plane();
	Plane(const glm::vec3& normal, const glm::vec3& pos);
	~Plane();

	static Plane fromPoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	Where classify(const glm::vec3& p);
	float distance(const glm::vec3& p);

private:
	glm::vec3 mN;
	float mDist;
};

