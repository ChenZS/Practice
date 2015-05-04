#include "RenderableObject.h"
#include <glm/glm.hpp>

void RenderableObject::Init()
{
	glGenVertexArrays(1, &mVaoID);
	glGenBuffers(1, &mVboVerticesID);
	glGenBuffers(1, &mVboIndicesID);

	glBindVertexArray(mVaoID);
		glBindBuffer(GL_ARRAY_BUFFER, mVboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, totalIndices() * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
		GLfloat *pBuf = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		fillVertexBuffer(pBuf);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glEnableVertexAttribArray(mShader["vVertex"]);
		glVertexAttribPointer(mShader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices() * sizeof(GLuint), 0, GL_STATIC_DRAW);

		GLuint* pIBuf = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		fillIndexBuffer(pIBuf);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindVertexArray(0);
}

void RenderableObject::Destroy()
{
	mShader.DeleteShaderProgram();

	glDeleteBuffers(1, &mVboVerticesID);
	glDeleteBuffers(1, &mVboIndicesID);
	glDeleteVertexArrays(1, &mVaoID);
}

void RenderableObject::render(const GLfloat* MVP)
{
	mShader.Use();
	glUniformMatrix4fv(mShader("MVP"), 1, GL_FALSE, MVP);
	glBindVertexArray(mVaoID);
	glDrawElements(primitiveType(), totalIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	mShader.UnUse();
}