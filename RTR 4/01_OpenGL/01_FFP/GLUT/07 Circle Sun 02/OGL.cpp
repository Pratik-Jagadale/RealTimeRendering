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
	const float r =  0.5f;
	static float yellow[] = { 1.0f,1.0f,0.0f};
	static float lavender[] = { 0.5f,0.5f,1.0f};

	/* code */
	glClear(GL_COLOR_BUFFER_BIT);
	/* Yello to Lavender */
	if(yellow[0] >= 0.5f)
		yellow[0] = yellow[0] - 0.01f;

	if(yellow[1] >= 0.5f)
		yellow[1] = yellow[1] - 0.01f;

	if(yellow[2] <= 1.0f)
		yellow[2] = yellow[2] + 0.01f;
	
	// lavendor to yellow
	if(lavender[0] <= 1.0f)
		lavender[0] = lavender[0] + 0.01f;

	if(lavender[1] <= 1.0f)
		lavender[1] = lavender[1] + 0.01f;
	
	if(lavender[2] > 0.0f)
		lavender[2] = lavender[2] - 0.01f;
	

	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(yellow[0],yellow[1],yellow[2]);
		glVertex3f(0.0f,0.0f,0.0f);
		float x, y;
		x = r * cos(angle);
		y =  r * sin(angle);
		glColor3f(lavender[0],lavender[1],lavender[2]);
		glVertex3f(x,y,0.0f);
	}
	glEnd();
	
	glutSwapBuffers();

	glutPostRedisplay();
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
