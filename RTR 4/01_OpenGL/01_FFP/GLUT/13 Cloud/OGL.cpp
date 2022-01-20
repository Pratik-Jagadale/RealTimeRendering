/* header files */
#include <GL/freeglut.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

/* entry-point function */
int main(int argc, char *argv[]) {//argc = argument count ,argv = argument vector

	/* local function declarations */
	void initialize(void);
	void resize(int,int);
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
	glClearColor(0.1f, 1.0f, 1.0f, 1.0f);
}

void resize(int width, int height) 
{
	/* code */
	glViewport(0, 0, width, height);
}

void display(void) {

	//function prototype
	void cloud(float, float,float,float);
	float r = 0.5;

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(0.0f + 0.5f,0.0f,0.0f);
		float x, y;
		x = (r * cos(angle)) + 0.6;
		y =  r * sin(angle);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(x,y,0.0f);
	}
	glEnd();

	r = 0.4;
	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		float x, y;
		x = (r * cos(angle)) + 0.4;
		y =  r * sin(angle);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(x,y,0.0f);
	}
	glEnd();

	r = 0.3;
	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(-0.2f,0.0f,0.0f);
		float x, y;
		x = (r * cos(angle)) + 0.5;
		y =  r * sin(angle);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(x,y,0.0f);
	}
	glEnd();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

	/* code */
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':

		if (bFullScreen == false){
			
			glutFullScreen();
			bFullScreen = true;
		}
		else {
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int  button, int state, int x, int y) {

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

void uninitialize(void) {

	/* code */
	if (1)
	{

	}
}

void cloud(float r, float centerX, float centerY , float curve)
{
	centerX = centerX + curve;

	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(0.1f,1.0f,1.0f);
		glVertex3f(centerX,centerY,0.0f);
		float x, y;
		x = (r * cos(angle)) + curve;
		y =  r * sin(angle);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(x,y,0.0f);
	}
	glEnd();
}