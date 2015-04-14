#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSLShader.h"

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 960;

GLSLShader shader;
int sub_divisions = 1;

//vertex array and vertex buffer object ids
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

//mesh vertices and indices
glm::vec3 vertices[4];
GLushort indices[6];

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

//camera transformation variables
int state = 0, oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -35;

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		oldX = x;
		oldY = y;
	}

	if (button == GLUT_MIDDLE_BUTTON)
		state = 0;
	else
		state = 1;
}

void OnMouseMove(int x, int y)
{
	if (state == 0)
	{
		dist *= (1 + (y - oldY) / 60.0f);
	}
	else
	{
		rY += (x - oldX) / 5.0f;
		rX += (y - oldY) / 5.0f;
	}

	oldX = x;
	oldY = y;

	glutPostRedisplay();
}

void OnKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ',': sub_divisions--; break;
	case '.': sub_divisions++; break;
	}

	sub_divisions = std::max(1, min(8, sub_divisions));

	glutPostRedisplay();
}

void OnInit()
{
	GL_CHECK_ERRORS
	
	shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
	shader.LoadFromFile(GL_GEOMETRY_SHADER, "shaders/shader.geom");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");

	shader.CreateAndLinkProgram();
	shader.Use();
	shader.AddAttribute("vVertex");
	shader.AddUniform("MVP");
	shader.AddUniform("sub_divisions");
	glUniform1i(shader("sub_divisions"), sub_divisions);
	shader.UnUse();

	GL_CHECK_ERRORS

	vertices[0] = glm::vec3(-5, 0, -5);
	vertices[1] = glm::vec3(-5, 0, 5);
	vertices[2] = glm::vec3(5, 0, 5);
	vertices[3] = glm::vec3(5, 0, -5);

	GLushort* id = &indices[0];
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;

	*id++ = 0;
	*id++ = 2;
	*id++ = 3;

	GL_CHECK_ERRORS

	//setup quad vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	GL_CHECK_ERRORS

	glEnableVertexAttribArray(shader["vVertex"]);
	glVertexAttribPointer(shader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

	GL_CHECK_ERRORS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GL_CHECK_ERRORS

	cout << "Initialization successfull" << endl;
}

void OnShutdown()
{
	shader.DeleteShaderProgram();

	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);



	cout << "Shutdown successfull" << endl;
}

void OnResize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//setup the projection matrix
	P = glm::perspective(45.0f, (GLfloat)w / h, 0.01f, 10000.f);
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
	glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
	MV = glm::translate(MV, glm::vec3(-5, 0, -5));

	shader.Use();
	glUniform1i(shader("sub_divisions"), sub_divisions);
	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	MV = glm::translate(MV, glm::vec3(10, 0, 0));
	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	MV = glm::translate(MV, glm::vec3(0, 0, 10));
	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	MV = glm::translate(MV, glm::vec3(-10, 0, 0));
	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV));
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
	glutCreateWindow("Simple plane subdivision using geometry shader - OpenGL 3.3");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	else
	{
		if (GLEW_VERSION_3_3)
		{
			cout << "Driver supports OpenGL 3.3\nDetails: " << endl;
		}
	}
	err = glGetError();
	GL_CHECK_ERRORS;

	cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	OnInit();

	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);

	glutMainLoop();

	return 0;
}