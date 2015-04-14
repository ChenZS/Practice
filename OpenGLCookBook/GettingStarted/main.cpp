#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "GLSLShader.h"



using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 960;

void OnInit()
{
	glClearColor(1, 0, 0, 0);
	cout << "Initialization successful" << endl;
}

void OnShutdown()
{
	cout << "Shutdown successful" << endl;
}

void OnResize(int nWidth, int nHeight)
{

}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(1280, 960);
	glutCreateWindow("Getting started with OpenGL 4.3");

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
			cout << "Driver supports OpenGL 3.3\nDetails:" << endl;
		}
	}

	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRender: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << endl;
	cout << "\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	GL_CHECK_ERRORS

	OnInit();

	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutMainLoop();

	return 0;
}