#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>

glm::vec3 Camera::sUp = glm::vec3(0, 1, 0);

Camera::Camera()
: mLook(glm::vec3(0, 0, -1)),
  mUp(glm::vec3(0, 1, 0)),
  mRight(glm::vec3(1, 0, 0))
{
}

Camera::~Camera()
{
}

void Camera::setupProjection(const float fovy, const float aspectRatio)
{
	mP = glm::perspective(fovy, aspectRatio, 0.1f, 1000.0f);
	mFOV = fovy;
	mAspectRatio = aspectRatio;
}

glm::mat4 Camera::matrixRPY(const float yaw, const float pitch, const float roll)
{
	return glm::yawPitchRoll(yaw, pitch, roll);
}

