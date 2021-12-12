/* header files */
#include <GL/freeglut.h>

/* global variables */
bool bFullScreen = false;

/* entry-point function */
int main(int argc, char *argv[]) {//argc = argument count ,argv = argument vector

	/* local function declarations */
	void initialize(void);
	void resize(int,int);
	void displayP(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	/* code */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("\"P\" ");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(displayP);
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

void displayP(void) {

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glColor3f(0.215f, 0.650f, 0.650f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(0.305f, 0.749f, 0.705f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glColor3f(0.305f, 0.749f, 0.705f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.305f, 0.749f, 0.705f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glColor3f(0.305f, 0.749f, 0.705f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(-0.2f, -0.4f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.0f, -0.4f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(-0.2f, 0.4f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(-0.2f, 0.6f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.0f, 0.6f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(-0.2f, 0.4f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.0f, 0.6f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.6f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.4f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.0f, 0.4f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.6f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.4f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.4f, 0.4f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.2f, 0.4f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glColor3f(0.050f, 0.411f, 0.450f);
	glVertex3f(0.4f, 0.4f, 0.0f);
	glEnd();


	glBegin(GL_TRIANGLES);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glColor3f(0.175f, 0.247f, 0.349f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glColor3f(0.835f, 0.949f, 0.929f);
	glVertex3f(0.2f, 0.2f, 0.0f);
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
