#include "TexturedPlane.h"
#include <glm/glm.hpp>


TexturedPlane::TexturedPlane(const int w, const int d)
: mWidth(w), mDepth(d)
{
	mShader.LoadFromFile(GL_VERTEX_SHADER, "shaders/checker_shader.vert");
	mShader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/checker_shader.frag");
	mShader.CreateAndLinkProgram();
	mShader.Use();
	mShader.AddAttribute("vVertex");
	mShader.AddUniform("MVP");
	mShader.AddUniform("textureMap");
	glUniform1i(mShader("textureMap"), 0);
	mShader.UnUse();

	Init();
}

void TexturedPlane::fillVertexBuffer(GLfloat* pBuf)
{
	glm::vec3* vertices = (glm::vec3*)(pBuf);

	int width_2 = mWidth / 2;
	int depth_2 = mDepth / 2;

	vertices[0] = glm::vec3(-width_2, 0, -depth_2);
	vertices[1] = glm::vec3( width_2, 0, -depth_2);
	vertices[2] = glm::vec3( width_2, 0,  depth_2);
	vertices[3] = glm::vec3(-width_2, 0,  depth_2);
}

void TexturedPlane::fillIndexBuffer(GLuint* pBuf)
{
	GLuint* id = pBuf;
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;
}

TexturedPlane::~TexturedPlane()
{
}

void TexturedPlane::render(const GLfloat* MVP)
{
	mShader.Use();
	glUniformMatrix4fv(mShader("MVP"), 1, GL_FALSE, MVP);
	glBindVertexArray(mVaoID);
	glDrawElements(primitiveType(), mTotalIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	mShader.UnUse();
}