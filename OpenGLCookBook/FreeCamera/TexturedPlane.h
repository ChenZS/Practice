#pragma once
#include "RenderableObject.h"
class TexturedPlane : public RenderableObject
{
public:
	TexturedPlane(const int width = 1000, const int depth = 1000);
	virtual ~TexturedPlane();

	int totalVertices() const { return 4; }
	int totalIndices() const { return 6;  }

	GLenum primitiveType() const { return GL_TRIANGLES;  }

	virtual void fillVertexBuffer(GLfloat* pBuf);
	virtual void fillIndexBuffer(GLuint* pBuf);
private:
	int mWidth, mDepth;
};

