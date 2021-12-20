/* header files */
#include <GL/freeglut.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

int I_O_FLAG = 1;
int I_B_FLAG = 0;
int I_Y_FLAG = 1;

int O_O_FLAG = 0;
int O_B_FLAG = 0;
int O_Y_FLAG = 1;
int O_W_FLAG = 0;

int BLACK = 0;
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

	glutInitWindowSize(800, 600);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{

	/* code */
	glViewport(0, 0, width, height);
}

void display(void)
{

	//const declartion
	const float r = 0.5f;

	static float Inner[] = {1.0f, 0.2f, 0.2f};
	static float Outer[] = {0.0f, 0.0f, 0.0f};

	static float white[] = {1.0f, 1.0f, 1.0f};
	static float orange[] = {1.0f, 0.2f, 0.2f};
	static float yellow[] = {1.0f, 1.0f, 0.0f};
	static float black[] = {0.0f, 0.0f, 0.0f};

	static float lavender[] = {0.5f, 0.5f, 1.0f};

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);

	/* INNER -> orange To Yellow & orange */
	if (Inner[1] <= 1.0f && I_Y_FLAG == 1)
	{
		Inner[1] = Inner[1] + 0.0001f;
		if (Inner[1] == 1.0f)
		{
			I_Y_FLAG = 0;
			I_B_FLAG = 1;
		}
	}
	if (Inner[2] >= 0.0f)
	{
		Inner[2] = Inner[2] - 0.00001f;
		if (Inner[2] == 0.0f)
		{
			I_Y_FLAG = 0;
			I_B_FLAG = 1;
		}
	}

	/*Inner Yellow To Black */
	if (Inner[0] >= 0.0 && I_B_FLAG == 1)
	{
		Inner[0] = Inner[0] - 0.00001f;
	}

	if (Inner[1] >= 0.0f && I_B_FLAG == 1)
	{
		Inner[1] = Inner[1] - 0.00001f;
		if (Inner[1] == 0.0f)
			BLACK = 1;
	}

	/* Outer Black to Yellow */
	if (Outer[0] <= 1.0f && O_Y_FLAG == 1)
	{
		Outer[0] = Outer[0] + 0.00001f;
		if (Outer[0] == 1.0f)
		{
			O_Y_FLAG = 0;
			O_O_FLAG = 1;
		}
	}

	if (Outer[1] <= 1.0f && O_Y_FLAG == 1)
	{
		Outer[1] = Outer[1] + 0.00001f;
		if (Outer[1] == 1.0f)
		{
			O_Y_FLAG = 0;
			O_O_FLAG = 1;
		}
	}

	/* Outer Yellow to Orange */
	if (Outer[1] >= 0.2f && O_O_FLAG == 1)
	{
		Outer[1] = Outer[1] - 0.0001f;
		if (Outer[1] == 0.2f)
		{
			O_O_FLAG = 0;
			O_W_FLAG = 1;
		}
	}
	if (Outer[2] <= 0.2f && O_O_FLAG == 1)
	{
		Outer[2] = Outer[2] - 0.000001f;
		if (Outer[2] == 0.2f)
		{
			O_O_FLAG = 0;
			O_W_FLAG = 1;
		}
	}

	/* Outer Orange To Black */
	if (Outer[0] >= 0.0f)
	{
		Outer[0] = Outer[0] - 0.00001f;
	}

	if (Outer[1] >= 0.0f)
	{
		Outer[1] = Outer[1] - 0.000001f;
	}

	if (Outer[2] >= 0.0f)
	{
		Outer[2] = Outer[2] - 0.000001f;
	}

	//*** Background Quads White, Yellow , Lavender to BALCK
	if (BLACK == 1)
	{
		//white to black

		//lavender to black

		//yellow to black
		if (yellow[0] >= 0.0)
		{
			yellow[0] = yellow[0] - 0.00001f;
		}

		if (yellow[1] >= 0.0f)
		{
			yellow[1] = yellow[1] - 0.00001f;
		}
	}

	glBegin(GL_QUADS);
	glColor3f(white[0], white[1], white[2]);
	glVertex3f(-1.0f, 1.0f, -0.5f);
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(1.0f, 1.0f, -0.5f);
	glColor3f(white[0], white[1], white[2]);
	glVertex3f(1.0f, -1.0f, -0.5f);
	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-1.0f, -1.0f, -0.5f);
	glEnd();

	glBegin(GL_LINES);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
	{
		glColor3f(Inner[0], Inner[1], Inner[2]);
		glVertex3f(0.0f, 0.0f, 0.0f);
		float x, y;
		x = r * cos(angle);
		y = r * sin(angle);
		glColor3f(Outer[0], Outer[1], Outer[2]);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glutSwapBuffers();

	glutPostRedisplay();
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
	if (1)
	{
	}
}
