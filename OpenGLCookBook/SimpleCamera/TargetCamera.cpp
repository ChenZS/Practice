#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <algorithm>
#include "TargetCamera.h"

#include <glm/gtx/euler_angles.hpp>

TargetCamera::TargetCamera() 
: mMinRy(-60),
  mMaxRy(60),
  mMinDistance(1),
  mMaxDistance(10)
{
}

void TargetCamera::update()
{
	glm::mat4 R = glm::yawPitchRoll(glm::radians(mYaw), glm::radians(mPitch), 0.0f);
	glm::vec3 T = glm::vec3(0, 0, mDistance);
	T = glm::vec3(R * glm::vec4(T, 0.0f));
	mPosition = mTarget + T;
	mLook = glm::normalize(mTarget - mPosition);
	mUp = glm::vec3(R * glm::vec4(sUp, 0.0f));
	mRight = glm::cross(mLook, mUp);
	mV = glm::lookAt(mPosition, mTarget, mUp);
}

void TargetCamera::setTarget(const glm::vec3 &tgt)
{
	mTarget = tgt;
	mDistance = glm::distance(mPosition, mTarget);
	mDistance = std::max(mMinDistance, std::min(mDistance, mMaxDistance));
	mV = glm::lookAt(mPosition, mTarget, mUp);

	mYaw = 0;
	mPitch = 0;

	if (mV[0][0] < 0)
	{
		mYaw = glm::degrees((float)(M_PI - asinf(-mV[2][0])));
	}
	else
	{
		mYaw = glm::degrees(asinf(-mV[2][0]));
	}
	mPitch = glm::degrees(asinf(-mV[1][2]));
}

void TargetCamera::rotate(const float yaw, const float pitch, const float roll)
{
	mYaw += yaw;
	mPitch += pitch;
	mPitch = std::min(std::max(mPitch, mMinRy), mMaxRy);
	update();
}

void TargetCamera::pan(const float dx, const float dy) {
	glm::vec3 X = mRight*dx;
	glm::vec3 Y = mUp*dy;
	mPosition += X + Y;
	mTarget += X + Y;
	update();
}


void TargetCamera::zoom(const float amount) {
	mPosition += mLook * amount;
	mDistance = glm::distance(mPosition, mTarget);
	mDistance = std::max(mMinDistance, std::min(mDistance, mMaxDistance));
	update();
}

void TargetCamera::move(const float dx, const float dy) {
	glm::vec3 X = mRight*dx;
	glm::vec3 Y = mLook*dy;
	mPosition += X + Y;
	mTarget += X + Y;
	update();
}
