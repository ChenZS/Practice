#pragma once
#include "Camera.h"
class FreeCamera : public Camera
{
public:
	FreeCamera();
	virtual ~FreeCamera();

	virtual void update();

	void walk(const float dt);
	void strafe(const float dt);
	void lift(const float dt);

	glm::vec3 translation() const
	{
		return mTranslation;
	}

	void setTranslation(const glm::vec3& t)
	{
		mTranslation = t;
		update();
	}

	const float speed() const
	{
		return mSpeed;
	}

	void setSpeed(const float s)
	{
		mSpeed = s;
	}

protected:
	float mSpeed;
	glm::vec3 mTranslation;
};

