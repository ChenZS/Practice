#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Grid.h"
#include "TargetCamera.h"

TargetCamera camera;
Grid *grid;

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

//#pragma comment(lib, "glew32.lib")

using namespace std;

const int WIDTH		= 1280;
const int HEIGHT	= 960;

void OnMouseDown(int button, int s, int x, int y)
{

}

void OnMouseMove(int x, int y)
{

}

void OnInit()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GL_CHECK_ERRORS

	grid = new Grid(10, 10);

	camera.setPosition(glm::vec3(5, 5, 5));
	camera.setTarget(glm::vec3(0, 0, 0));

	cout << "Initialization successfull" << endl;
}

void OnShutdown()
{
	delete grid;
	cout << "Shutdown successfull" << endl;
}

void OnResize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	camera.setupProjection(45, (GLfloat)w / h);
}

void OnIdle()
{
}

void OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 MV = camera.viewMatrix();
	glm::mat4 P = camera.projectionMatrix();
	glm::mat4 MVP = P * MV;

	grid->render(glm::value_ptr(MVP));

	glutSwapBuffers();
}

void OnKey(unsigned char key, int x, int y)
{
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Target Camera - OpenGL 3.3");

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

	err = glGetError();
	GL_CHECK_ERRORS

	cout << "\tUsing GLEW" << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GL_CHECK_ERRORS
	OnInit();
	glutCloseFunc(OnShutdown);

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutKeyboardFunc(OnKey);
	glutIdleFunc(OnIdle);
	glutMainLoop();

	return 0;
}