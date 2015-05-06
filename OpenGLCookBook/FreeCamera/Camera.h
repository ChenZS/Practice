#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	const glm::mat4 viewMatrix() const
	{
		return mV;
	}

	const glm::mat4 projectionMatrix() const
	{
		return mP;
	}

	const glm::vec3 Camera::position() const
	{
		return mPosition;
	}

	void setPosition(const glm::vec3 &v)
	{
		mPosition = v;
	}

	const float FOV() const
	{
		return mFOV;
	}

	const float aspectRatio() const
	{
		return mAspectRatio;
	}

	void setupProjection(const float fovy = 45.0f, const float aspectRatio = 1.33333f);
	glm::mat4 matrixRPY(const float yaw, const float pitch, const float roll);

	virtual void update() = 0;
	virtual void rotate(const float yaw, const float pitch, const float roll);

protected:
	float mFOV, mAspectRatio;
	glm::vec3 mLook;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mPosition;
	glm::mat4 mV;
	glm::mat4 mP;

	static glm::vec3 sUp;

	float mYaw, mPitch, mRoll;
};