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

	/* code */
/*Center star*/
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);

	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-0.5f, 0.4f, -0.1f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.4f, -0.1f);
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(0.0f, -0.5f, -0.1f);
	

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.6f, -0.2f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.3f, -0.2f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -0.3f, -0.2f);


/* Top Left star */
	/* Top left start back trangle */	
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-1.0f, 0.95f, -0.1f);//top left vertex
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(-0.8f, 0.95f, -0.1f);//right vertex
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(-0.9f, 0.75f, -0.1f);//bottom vertex
	

	/* Top left start front trangle */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, 1.0f, -0.2f);//top vertex
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.8f, -0.2f);//left vertex
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.8f, 0.8f, -0.2f);//right vertex


/* Bottom left star */
	/* back trangle */	
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(-1.0f, -0.8f, -0.1f);//top left vertex
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(-0.8f, -0.8f, -0.1f);//right vertex
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(-0.9f, -1.0f, -0.1f);//bottom vertex
	

	/* front trangle */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.9f, -0.75f, -0.2f);//top vertex
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -0.95f, -0.2f);//left vertex
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.8f, -0.95f, -0.2f);//right vertex

/* Bottom Right star */
	/* back trangle */	
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.8f, -0.8f, -0.1f);//top left vertex
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f, -0.8f, -0.1f);//right vertex
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(0.9f, -1.0f, -0.1f);//bottom vertex
	

	/* front trangle */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, -0.75f, -0.2f);//top vertex
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, -0.95f, -0.2f);//left vertex
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -0.95f, -0.2f);//right vertex


/* Top Right star */
	/* back trangle */	
	glColor3f(0.0f,1.0f,1.0f);
	glVertex3f(0.8f, 0.95f, -0.1f);//top left vertex
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f, 0.95f, -0.1f);//right vertex
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(0.9f, 0.75f, -0.1f);//bottom vertex
	

	/* front trangle */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.9f, 1.0f, -0.2f);//top vertex
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 0.8f, -0.2f);//left vertex
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.8f, -0.2f);//right vertex



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
