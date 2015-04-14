#pragma once

#include "RenderableObject.h"

class Grid : public RenderableObject
{
public:
	Grid(int width = 10, int depth = 10);
	virtual ~Grid();

	void render(const float* MVP);
};

