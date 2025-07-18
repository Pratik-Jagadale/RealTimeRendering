/* header files */
#include <GL/freeglut.h>

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

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

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
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

  	//Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}


void resize(int width, int height) {

	/* code */
	if(height < 0)
		height = 1.0f;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1.0f,1.0f,-1.0f,1.0f,1.5f,25.0f);
}

void display(void) {

	/* code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0f,0.0f,5.0f, 0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f);
	
	glScalef(1.0f,2.0f,-1.0f);
	
	glutWireCube(1.0f);

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
