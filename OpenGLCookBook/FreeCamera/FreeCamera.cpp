#include "FreeCamera.h"
#include <glm/gtx/euler_angles.hpp>


FreeCamera::FreeCamera()
: mTranslation(glm::vec3(0)), mSpeed(0.5f)
{
}


FreeCamera::~FreeCamera()
{
}

void FreeCamera::update()
{
	glm::mat4 R = glm::yawPitchRoll(mYaw, mPitch, mRoll);
	mPosition += mTranslation;

	mLook = glm::vec3(R * glm::vec4(0, 0, 1, 0));
	mUp = glm::vec3(R * glm::vec4(0, 1, 0, 0));
	mRight = glm::cross(mLook, mUp);

	glm::vec3 tgt = mPosition + mLook;
	mV = glm::lookAt(mPosition, tgt, mUp);
}

void FreeCamera::walk(const float dt)
{
	mTranslation += (mLook * mSpeed * dt);
	update();
}

void FreeCamera::strafe(const float dt)
{
	mTranslation += (mRight * mSpeed * dt);
	update();
}

void FreeCamera::lift(const float dt)
{
	mTranslation += (mUp * mSpeed * dt);
	update();
}