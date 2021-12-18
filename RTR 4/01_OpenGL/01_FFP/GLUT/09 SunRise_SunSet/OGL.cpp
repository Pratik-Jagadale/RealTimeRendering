/* header files */
#include <GL/freeglut.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

typedef int O_FLAG = 1;
typedef int W_FLAG = 0;
typedef int B_FLAG = 0;
typedef int Y_FLAG = 0;

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

	static float orange[] = {1.0f,0.2f,0.2f};
	static float yellow[] = { 1.0f,1.0f,0.0f};
	static float white[] = {1.0f,1.0f,1.0f};
	static float black[] = {0.0f,0.0f,0.0f};

	static float lavender[] = { 0.5f,0.5f,1.0f};

	static float Outer_Yellow[] = {};
	static float Outer_Orange[] = {};
	static float Outer_white[] = {};
	static float Outer_black[] = {};



	/* code */
	glClear(GL_COLOR_BUFFER_BIT);


	/* orange To Yellow & orange */
	if(orange[1] <= 1.0f && O_FLAG == 1)
	{
		orange[1] = orange[1] + 0.0001f;
		if(orange[1] == 1.0f){
			Y_FLAG = 1;
			O_FLAG = 0;
		}
	}
	if(orange[2] >= 0.0f && O_FLAG == 1)
	{
		orange[2] = orange[1] - 0.0001f;	
	}

	/* orange To White & Orange */
	if(orange[2] < 1.0f && W_FLAG == 1)
	{

	}


	/* White to Yellow & orange */

	/* Yellow to Orange */

	/* Orange To Black */

	
	glBegin(GL_QUADS);
	glColor3f(white[0],white[1],white[2]);
	glVertex3f(-1.0f, 1.0f, -0.5f);
	glColor3f(yellow[0],yellow[1],yellow[2]);
	glVertex3f(1.0f, 1.0f, -0.5f);
	glColor3f(white[0],white[1],white[2]);
	glVertex3f(1.0f, -1.0f, -0.5f);
	glColor3f(lavender[0],lavender[1],lavender[2]);
	glVertex3f(-1.0f, -1.0f, -0.5f);
	glEnd();

	glBegin(GL_LINES);
	for(float angle =  0.0f; angle < 360.0f ; angle = angle + 0.01f)
	{
		glColor3f(yellow[0],yellow[1],yellow[2]);
		glVertex3f(0.0f,0.0f,0.0f);
		float x, y;
		x = r * cos(angle);
		y =  r * sin(angle);
		glColor3f(organge[0],organge[1],organge[2]);
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
