#pragma once
#include "GLSLShader.h"

class RenderableObject
{
public:
	RenderableObject() : mVaoID(0), mVboVerticesID(0), mVboIndicesID(0) {}
	virtual ~RenderableObject() { Destroy(); }
	virtual void render(const float* MVP);

	virtual void fillVertexBuffer(GLfloat* pBuf) {}
	virtual void fillIndexBuffer(GLuint* pBuf) {}

	virtual int totalVertices() const { return mTotalVertices; }
	virtual int totalIndices() const { return mTotalIndices; }
	virtual GLenum primitiveType() const { return mPrimitiveType;  }

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