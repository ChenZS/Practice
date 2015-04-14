#pragma once
#include "GLSLShader.h"

class RenderableObject
{
public:
	RenderableObject() : mVaoID(0), mVboVerticesID(0), mVboIndicesID(0) {}
	virtual ~RenderableObject() {}
	virtual void render(const float* MVP) = 0;

protected:
	GLuint mVaoID;
	GLuint mVboVerticesID;
	GLuint mVboIndicesID;
	int mTotalIndices;
	GLSLShader mShader;
};