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

void initialize(void){
	
	/* code */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height) {

	/* code */
	glViewport(0, 0, width, height);
}

void display(void) {

	//const declartion
	const float r =  0.35f;

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	for(float i = 0 ; i < 0.58 ; i = i + 0.001)
	{
		glColor3f(0.294f,0.0f,0.509f);
		glVertex3f(0.0f, -0.65f, -0.01f);
		glColor3f(0.294f,0.0f,0.509f);
		glVertex3f(0.29 - i,-0.20,0.0f);
	}
	glEnd();

	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(0.1f,1.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		float x, y;
		x = r * cos(angle);
		y =  r * sin(angle);
		glColor3f(0.294f,0.0f,0.509f);
		glVertex3f(x,y,0.0f);
	}
	glEnd();


	glBegin(GL_TRIANGLES);
	glColor3f(0.294f,0.0f,0.509f);
	glVertex3f(-0.06f, -0.33f, 0.0f);
	glColor3f(0.294f,0.0f,0.509f);
	glVertex3f(-0.10f, -0.33f, 0.0f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.08f, -0.35f, 0.0f);
	glEnd();



	glBegin(GL_TRIANGLES);
	glColor3f(0.294f,0.0f,0.509f);
	glVertex3f(0.06f, -0.45f, 0.0f);
	glColor3f(0.294f,0.0f,0.509f);
	glVertex3f(0.10f, -0.45f, 0.0f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.08f, -0.47f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.0f, -0.65f, -0.01f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.0f,-0.80,0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.001f, -0.65f, -0.01f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.001f,-0.80,0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.002f, -0.65f, -0.01f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.002f,-0.80,0.0f);
	glEnd();

	// Ribbins


	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.15f, -0.35f, -0.01f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(0.15f,-0.80,0.0f);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(-0.15f, -0.50f, -0.01f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(-0.15f,-0.80,0.0f);
	glEnd();
	

	glBegin(GL_LINES);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.05f, -0.60f, -0.01f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.05f,-0.80,0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.5f, -0.05f, -0.01f);
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.5f,-0.80,0.0f);
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
