#pragma once
#include "Camera.h"

class TargetCamera : public Camera
{
public:
	TargetCamera();

	void update();
	void rotate(const float yaw, const float pitch, const float roll);
	
	void setTarget(const glm::vec3 &tgt);
	const glm::vec3 target() const { return mTarget; }

	void pan(const float dx, const float dy);
	void zoom(const float amount);
	void move(const float dx, const float dz);

protected:
	glm::vec3 mTarget;
	float mYaw, mPitch, mRoll;

	float mMinRy, mMaxRy;
	float mDistance;
	float mMinDistance, mMaxDistance;
};