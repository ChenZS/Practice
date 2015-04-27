#pragma once
#include "RenderableObject.h"
class TexturedPlane : public RenderableObject
{
public:
	TexturedPlane(const int width = 1000, const int depth = 1000);
	virtual ~TexturedPlane();

	virtual void render(const GLfloat* MVP);

	int totalVertices() const { return 4; }
	int totalIndices() const { return 6;  }

	GLenum primitiveType() const { return GL_TRIANGLES;  }

	void fillVertexBuffer(GLfloat* pBuf);
	void fillIndexBuffer(GLuint* pBuf);
private:
	int mWidth, mDepth;
};

