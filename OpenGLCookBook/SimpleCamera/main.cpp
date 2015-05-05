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

int state = 0, oldX = 0, oldY = 0;
float rX = 0, rY = 135, dist = 0;

const int VK_W = 0x57;
const int VK_S = 0x53;
const int VK_A = 0x41;
const int VK_D = 0x44;
const int VK_Q = 0x51;
const int VK_Z = 0x5a;

const float MOVE_SPEED = 0.125f;

const float MOUSE_FILTER_WEIGHT = 0.75f;
const int MOUSE_HISTORY_BUFFER_SIZE = 10;
glm::vec2 mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];

float mouseX = 0, mouseY = 0; 
bool useFiltering = true;
void filterMouseMoves(float dx, float dy)
{
	for (int i = MOUSE_HISTORY_BUFFER_SIZE - 1; i > 0; --i) 
	{
		mouseHistory[i] = mouseHistory[i - 1];
	}

	// Store current mouse entry at front of array.
	mouseHistory[0] = glm::vec2(dx, dy);

	float averageX = 0.0f;
	float averageY = 0.0f;
	float averageTotal = 0.0f;
	float currentWeight = 1.0f;

	// Filter the mouse.
	for (int i = 0; i < MOUSE_HISTORY_BUFFER_SIZE; ++i)
	{
		glm::vec2 tmp = mouseHistory[i];
		averageX += tmp.x * currentWeight;
		averageY += tmp.y * currentWeight;
		averageTotal += 1.0f * currentWeight;
		currentWeight *= MOUSE_FILTER_WEIGHT;
	}

	mouseX = averageX / averageTotal;
	mouseY = averageY / averageTotal;
}

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		oldX = x;
		oldY = y;
	}

	if (button == GLUT_MIDDLE_BUTTON)
	{
		state = 0;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		state = 2;
	}
	else
	{
		state = 1;
	}
}

void OnMouseMove(int x, int y)
{
	if (state == 0)
	{
		dist = (y - oldY) / 60.0f;
		camera.zoom(dist);
	}
	else if (state == 2)
	{
		float dy = float(y - oldY) / 100.0f;
		float dx = float(oldX - x) / 100.0f;

		if (useFiltering)
		{
			filterMouseMoves(dx, dy);
		}
		else
		{
			mouseX = dx;
			mouseY = dy;
		}

		camera.pan(mouseX, mouseY);
	}
	else
	{
		rY = float(y - oldY);
		rX = float(oldX - x);

		if (useFiltering)
		{
			filterMouseMoves(rX, rY);
		}
		else
		{
			mouseX = rX;
			mouseY = rY;
		}

		camera.rotate(mouseX, mouseY, 0);
	}

	oldX = x;
	oldY = y;

	glutPostRedisplay();
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
	bool bPressed = false;
	float dx = 0, dy = 0;
	if (GetAsyncKeyState(VK_W) & 0x8000) 
	{
		dy += (MOVE_SPEED);
		bPressed = true;
	}

	if (GetAsyncKeyState(VK_S) & 0x8000) 
	{
		dy -= (MOVE_SPEED);
		bPressed = true;
	}

	if (GetAsyncKeyState(VK_A) & 0x8000) 
	{
		dx -= (MOVE_SPEED);
		bPressed = true;
	}

	if (GetAsyncKeyState(VK_D) & 0x8000) 
	{
		dx += (MOVE_SPEED);
		bPressed = true;
	}

	if (bPressed)
		camera.move(dx, dy);

	glutPostRedisplay();
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
	switch (key) 
	{
	case ' ':
		useFiltering = !useFiltering;
		break;
	}
	glutPostRedisplay();
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