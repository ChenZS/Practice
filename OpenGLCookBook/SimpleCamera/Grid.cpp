#include "Grid.h"
#include <glm/glm.hpp>

Grid::Grid(int width, int depth) : RenderableObject()
{
	mShader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
	mShader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
	mShader.CreateAndLinkProgram();
	mShader.Use();
		mShader.AddAttribute("vVertex");
		mShader.AddUniform("MVP");
	mShader.UnUse();

	glm::vec3* vertices = new glm::vec3[((width + 1) + (depth + 1)) * 2];
	mTotalIndices = width * depth;
	GLushort* indices = new GLushort[mTotalIndices];

	int count = 0;
	int width_2 = width / 2;
	int depth_2 = depth / 2;

	for (int i = -width_2; i <= width_2; i++)
	{
		vertices[count++] = glm::vec3(i, 0, -depth_2);
		vertices[count++] = glm::vec3(i, 0, depth_2);

		vertices[count++] = glm::vec3(-width_2, 0, i);
		vertices[count++] = glm::vec3(width_2, 0, i);
	}

	GLushort* id = &indices[0];
	for (int i = 0; i < width*depth; i += 4) 
	{
		*id++ = i;
		*id++ = i + 1;
		*id++ = i + 2;
		*id++ = i + 3;
	}

	glGenVertexArrays(1, &mVaoID);
	glGenBuffers(1, &mVboVerticesID);
	glGenBuffers(1, &mVboIndicesID);

	glBindVertexArray(mVaoID);

	glBindBuffer(GL_ARRAY_BUFFER, mVboVerticesID);
	glBufferData(GL_ARRAY_BUFFER, ((width + 1) + (depth + 1)) * 2 * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(mShader["vVertex"]);
	glVertexAttribPointer(mShader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*mTotalIndices, &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	delete[] indices;
	delete[] vertices;
}


Grid::~Grid()
{
	mShader.DeleteShaderProgram();

	glDeleteBuffers(1, &mVboVerticesID);
	glDeleteBuffers(1, &mVboIndicesID);
	glDeleteVertexArrays(1, &mVaoID);
}

void Grid::render(const float* MVP)
{
	mShader.Use();
	glUniformMatrix4fv(mShader("MVP"), 1, GL_FALSE, MVP);
	glBindVertexArray(mVaoID);
	glDrawElements(GL_LINES, mTotalIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	mShader.UnUse();
}
