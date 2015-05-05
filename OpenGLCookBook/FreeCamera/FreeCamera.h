#pragma once
#include "Camera.h"
class FreeCamera : public Camera
{
public:
	FreeCamera();
	virtual ~FreeCamera();

	virtual void update();
};

