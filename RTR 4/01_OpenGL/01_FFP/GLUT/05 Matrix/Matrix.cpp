// ------------------ || Shree || ------------------ 
// header files
#include <windows.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>  
#define MATRIX_COUNT 50

// global variables
bool bFullScreen = false;
float yPos = 1.0f;
float zPosArr[MATRIX_COUNT];
float xPosArr[MATRIX_COUNT];
float yPosArr[MATRIX_COUNT];
double zoom=1.0;
double matrixX = -0.2;
double matrixZoom = 0.0002;
const double scene2Start = -1.5;

// animate
void timerFunction(int timerID) {
	yPos -= 0.02f;
	zoom += 0.00009;
	if(yPos < scene2Start)
	{
		matrixX -= 0.0025;
		matrixZoom += 0.00001;
	}
	glutTimerFunc(65, timerFunction, 0);
	glutPostRedisplay();
}

// Entry-point function
int main(int argc, char* argv[])
{
	// local function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	// Code
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Aishwarya Sanjay Kendle");
	
	initialize();
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);
	glutFullScreen();
	glutTimerFunc(30, timerFunction, 0);
	glutMainLoop();

	return(0);
}

// DrawText
void DrawText(float x, float y, float z)
{  
  glColor3f(0.0f, 1.0f, 0.0f); 
  char ch[] = "a123123^%#$124665sd123fqawjh)(&^#$*^%!(^*&$KJHVAJFSDKVD1232435123JHABSdcjh";

  for(int i = 0 ; ch[i] != '\0'; i++ )
  {
	unsigned char dispChar = ch[i];
	dispChar += rand() % 30 + 1;
  	glRasterPos3f(x, y, z);
  	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dispChar);
	y = y - 0.07;
  }
}
void drawMatrix()
{
	glColor3f(0.0f, 1.0f, 0.0f); 
	unsigned char matrix[] = "Matrix Group";
	// Matrix
	glTranslatef(matrixX, 0.0f, 0.0f);
	glScalef(matrixZoom, matrixZoom, 0.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, matrix);

}

float randF() {
	float a = -1.5f;
	float b = 1.5f;
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
void initialize(void)
{
	// Code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	for(int  i = 0; i< MATRIX_COUNT; i++)
	{
		xPosArr[i] = randF();
		zPosArr[i] = randF();
		yPosArr[i] = randF();
	}
	glTranslatef(matrixX, 0.0f, 0.0f);
	PlaySound("matrix.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
}

void resize(int width, int height)
{
	// Code
	glViewport(0, 0, width, height);
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT );
	
	for(int  i = 0; i< MATRIX_COUNT; i++)
	{
		DrawText( xPosArr[i], yPos+yPosArr[i], 0.0f);
	}
	glScalef(zoom, zoom, 0.0f);

	if(yPos < scene2Start)
	{
		drawMatrix();
		glLoadIdentity();
	}

	glEnd();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	// code
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
	// case 'N':
	// case 'n':
	// 	yPos -= 0.01f;
	// 	zoom += 0.00009;
	// 	if(yPos < -1.0f)
	// 	{
	// 		matrixX -= 0.0025;
	// 		matrixZoom += 0.00001;
	// 	}
	// 	glutPostRedisplay();
		// break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	// code
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
	// code
}
