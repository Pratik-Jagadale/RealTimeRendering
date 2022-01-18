/* header files */
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

SYSTEMTIME st;

//#define macros
#define PI 3.1459265

const float r1 = 0.42f;
const float r2 = 0.39f;
const float r3 = 0.37f;

const float clockTime = -(3.1459265 / 5) / 6;

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
	void DrawCircle(void);
	void DrawInnerBackgroundCircle(void);
	void DrawPoints(void);
	void DrawHrHand(void);
	void DrawMinutrHand(void);
	void DrawSecondHand(void);
	void DrawSubLines(void);
	void DrawCenter(void);

	/* code */
	GetSystemTime(&st);

	glClear(GL_COLOR_BUFFER_BIT);

	DrawCircle();
	DrawInnerBackgroundCircle();
	DrawPoints();
	DrawHrHand();
	DrawMinutrHand();
	DrawSecondHand();
	DrawSubLines();
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
void DrawCircle(void)
{
	// Outer Color
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
	{
		float x, y;
		x = r1 * cos(angle);
		y = r1 * sin(angle);
		glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void DrawInnerBackgroundCircle(void)
{
	// Inner Back Ground Circle
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
	{

		glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		float x, y;
		x = r2 * cos(angle);
		y = r2 * sin(angle);
		glColor3f(0.184313725490196f, 0.2705882352f, 0.3803921568627f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}
// Points

void DrawPoints(void)
{
	glPointSize(10);
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + (PI / 2))
	{
		float x, y;
		x = r3 * cos(angle);
		y = r3 * sin(angle);
		glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
		glVertex3f(x, y, 0.1f);
	}
	glEnd();
}

void DrawHrHand(void)
{
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(0.4f, 0.33725f, 0.494117f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	float x = 0.0, y = 0.0;
	x = 0.17 * cos(clockTime * st.wHour);
	y = 0.17 * sin(clockTime * st.wHour);
	glColor3f(0.4f, 0.33725f, 0.494117f);
	glVertex3f(x, y, 0.0f);
	glEnd();
}

void DrawMinutrHand(void)
{
	// Minute Hand
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(0.4f, 0.33725f, 0.494117f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	float x = 0.0, y = 0.0;
	x = 0.24 * cos(clockTime * st.wMinute);
	y = 0.24 * sin(clockTime * st.wMinute);
	glColor3f(0.4f, 0.33725f, 0.494117f);
	glVertex3f(x, y, 0.0f);

	glEnd();
}
void DrawSecondHand(void)
{
	// Second Hand
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	float x = 0.0, y = 0.0;
	x = 0.25 * cos(clockTime * st.wSecond);
	y = 0.25 * sin(clockTime * st.wSecond);
	glColor3f(1.0f, 0.0f, 0.1f);
	glVertex3f(x, y, 0.5f);
	glEnd();
}

void DrawSubLines(void)
{
	// SubPoins
	glPointSize(1);
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + (PI / 6.0f))
	{
		float x, y;
		x = r3 * cos(angle);
		y = r3 * sin(angle);
		glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
		glVertex3f(x, y, 0.1f);

		
		glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
		if (x > 0 && y > 0)
		{
			glVertex3f(x - 0.0001, y - 0.0001, 0.2f);
		}
		else if (x > 0 && y < 0)
		{
			glVertex3f(x - 0.0001, y + 0.0001, 0.1f);
		}
		else if (x < 0 && y < 0)
		{
			glVertex3f(x + 0.0001, y + 0.0001, 0.1f);
		}
		else if (x < 0 && y > 0)
		{
			glVertex3f(x + 0.0001, y - 0.0001, 0.1f);
		}
	}
	glEnd();
}

void DrawCenter(void)
{
	// Center
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}