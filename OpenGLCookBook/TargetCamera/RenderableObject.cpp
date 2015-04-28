#include "RenderableObject.h"
#include <glm/glm.hpp>

void RenderableObject::Init()
{
	glGenVertexArrays(1, &mVaoID);
	glGenBuffers(1, &mVboVerticesID);
	glGenBuffers(1, &mVboIndicesID);

	glBindVertexArray(mVaoID);
		glBindBuffer(GL_ARRAY_BUFFER, mVboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, mTotalVertices * sizeof(glm::vec3), 0, GL_STATIC_DRAW);


	glBindVertexArray(0);
}

void RenderableObject::Destroy()
{

}