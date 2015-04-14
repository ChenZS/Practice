#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLSLShader.h"

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")

using namespace std;

const int WIDTH = 512;
const int HEIGHT = 512;

GLSLShader shader;

GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

GLuint textureID;

glm::vec2 vertices[4];
GLushort indices[6];

const string filename = "media/Lenna.png";

void OnInit()
{
	GL_CHECK_ERRORS

	shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
	shader.CreateAndLinkProgram();
	shader.Use();
	shader.AddAttribute("vVertex");
	shader.AddUniform("textureMap");
	glUniform1i(shader("textureMap"), 0);
	shader.UnUse();

	GL_CHECK_ERRORS

	vertices[0] = glm::vec2(0.0, 0.0);
	vertices[1] = glm::vec2(1.0, 0.0);
	vertices[2] = glm::vec2(1.0, 1.0);
	vertices[3] = glm::vec2(0.0, 1.0);

	GLushort* id = &indices[0];
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;

	GL_CHECK_ERRORS

	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS
	
	// Enable vertex attribute array for position
	glEnableVertexAttribArray(shader["vVertex"]);
	glVertexAttribPointer(shader["vVertex"], 2, GL_FLOAT, GL_FALSE, 0, 0);

	GL_CHECK_ERRORS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	GL_CHECK_ERRORS

	// load the image using SOIL
	int texture_width = 0, texture_height = 0, channels = 0;
	GLubyte* pData = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_AUTO);
	if (pData == NULL) 
	{
		cerr << "Cannot load image: " << filename.c_str() << endl;
		exit(EXIT_FAILURE);
	}

	// vertically flip the image on Y axis since it is inverted
	int i, j;
	for (j = 0; j * 2 < texture_height; ++j)
	{
		int index1 = j * texture_width * channels;
		int index2 = (texture_height - 1 - j) * texture_width * channels;
		for (i = texture_width * channels; i > 0; --i)
		{
			GLubyte temp = pData[index1];
			pData[index1] = pData[index2];
			pData[index2] = temp;
			++index1;
			++index2;
		}
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	SOIL_free_image_data(pData);
}

void OnShutdown()
{
	shader.DeleteShaderProgram();

	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	glDeleteTextures(1, &textureID);
	cout << "Shutdown successfull" << endl;
}

void OnResize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	shader.UnUse();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Simple image loader - OpenGL 3.3");

	// glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	else
	{
		if (GLEW_VERSION_3_3)
		{
			cout << "Driver supports OpenGL 3.3\nDetails:" << endl;
		}
	}

	err = glGetError();
	GL_CHECK_ERRORS

	cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	GL_CHECK_ERRORS

	OnInit();

	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);

	glutMainLoop();
	return 0;
}