#pragma once
#include "GLSLShader.h"

class RenderableObject
{
public:
	RenderableObject() : mVaoID(0), mVboVerticesID(0), mVboIndicesID(0) {}
	virtual ~RenderableObject() { Destroy(); }
	virtual void render(const float* MVP) = 0;

	void Init();
	void Destroy();

protected:
	GLuint mVaoID;
	GLuint mVboVerticesID;
	GLuint mVboIndicesID;
	int mTotalIndices;
	int mTotalVertices;
	GLenum mPrimitiveType;
	GLSLShader mShader;
};