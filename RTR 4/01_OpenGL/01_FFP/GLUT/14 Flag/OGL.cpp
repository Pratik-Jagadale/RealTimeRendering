/* header files */
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

//#define macros
#define PI 3.1459265


/* entry-point function */
int main(int argc, char *argv[])
{ //argc = argument count ,argv = argument vector

	/* local function declarations */
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	/* code */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pratik Rajendra Jagadale");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return (0);
}

void initialize(void)
{

	/* code */
	glClearColor(1.0f, 1.0f,1.0f, 0.0f);
}

void resize(int width, int height)
{

	/* code */
	glViewport(0, 0, width, height);
}

void display(void)
{
	//function prototypes
	void DrawOrangeQuad(void);
	void DrawWhiteQuad(void);
	void DrawGreenQuad(void);
	
	void DrawCircle(void);
	void DrawLines(void);
	void DrawCenter(void);

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);

	DrawOrangeQuad();
	DrawWhiteQuad();
	DrawGreenQuad();
	DrawCircle();
	DrawLines();
	DrawCenter();

	glutPostRedisplay();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{

	/* code */
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':

		if (bFullScreen == false)
		{

			glutFullScreen();
			bFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	/* code */
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void)
{
	/* code */
}

/* DRAW FUCNTIONS */
void DrawOrangeQuad(void)
{
	glBegin(GL_QUADS);
	
	glColor3f(1.0f, 0.6f, 0.2f);
	glVertex3f(-0.59f, 0.39f, 0.0f);

	glColor3f(1.0f, 0.6f, 0.2f);
	glVertex3f(-0.59f, 0.12f, 0.0f);
	
	glColor3f(1.0f, 0.6f, 0.2f);
	glVertex3f(0.59f, 0.12f, 0.0f);
	
	glColor3f(1.0f, 0.6f, 0.2f);
	glVertex3f(0.59f, 0.39f, 0.0f);
	
	glEnd();
}

void DrawWhiteQuad(void)
{
	glBegin(GL_QUADS);

	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	
	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glEnd();
}

void DrawGreenQuad(void)
{
	glBegin(GL_QUADS);

	glColor3f(0.074f, 0.533f, 0.031f);
	glVertex3f(-0.59f, -0.12f, 0.0f);

	glColor3f(0.074f, 0.533f, 0.031f);
	glVertex3f(-0.59f, -0.39f, 0.0f);
	
	glColor3f(0.074f, 0.533f, 0.031f);
	glVertex3f(0.59f, -0.39f, 0.0f);

	glColor3f(0.074f, 0.533f, 0.031f);
	glVertex3f(0.59f, -0.12f, 0.0f);

	glEnd();
}


void DrawCircle(void)
{
	// Outer Color
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
	{
		float x, y;
		x = 0.1f * cos(angle);
		y = 0.1f * sin(angle);
		glColor3f(0.0f, 0.0f, 0.50f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void DrawLines(void)
{
	glLineWidth(2);
	glBegin(GL_LINES);
		for (float angle = 0.0f; angle < PI * 2; angle = angle + (PI / 12))
		{
			glColor3f(0.0f, 0.0f, 0.50f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			float x,y;
			x = 0.1f * cos(angle);
			y = 0.1f * sin(angle);
			glColor3f(0.0f, 0.0f, 0.50f);
			glVertex3f(x, y, 0.0f);
			
		}
	glEnd();
}


void DrawCenter(void)
{
	// Center
	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.50f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}