
/* header files */
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

SYSTEMTIME st;

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void resize(int width, int height)
{

    /* code */
    glViewport(0, 0, width, height);
}

void display(void)
{
    float x1[3] = {-0.42f, 1.0f, 0};
    if (x1[0] > -0.42f)
        x1[1] = x1[1] - 0.001f;

    float x2[3] = {-0.47f, 0.92f, 0};
    if (x2[0] > -0.445f)
        x2[0] = x2[0] - 0.001f;
    if (x2[1] f > 0.0895f)
        x2[1] = x2[1] - 0.001f;

    float x3[3] = {-0.55f, 0.92f, 0};
    if (x3[0] > 0.495f)
        x3[0] = x[0] - 0.001f;
    if (x3[1] < 0.89)
        x[1] = x[1] + 0.001f;

    float x4[3] = {-0.48f, 0.86f, 0};
    float x5[3] = {-0.51f, 0.76f, 0};
    float x6[3] = {-0.43f, 0.82f, 0};
    float x7[3] = {-0.35f, 0.76f, 0};
    float x8[3] = {-0.38f, 0.86f, 0};
    float x9[3] = {-0.3f, 0.92f, 0};
    float x10[3] = {-0.39f, 0.92f, 0};

    /* code */
    GetSystemTime(&st);

    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x1[0], x1[1], x1[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x2[0], x2[1], x2[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x3[0], x3[1], x3[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x4[0], x4[1], x4[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x5[0], x5[1], x5[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x6[0], x6[1], x6[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x7[0], x7[1], x7[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x8[0], x8[1], x8[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x9[0], x9[1], x9[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x10[0], x10[1], x10[2]);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x1[0], x1[1], x1[2]);

    glEnd();

    //Inner
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.42f, 0.94f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.445f, 0.895f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.495f, 0.895f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.45f, 0.865f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.47f, 0.81f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.43f, 0.845f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.385f, 0.815f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.4f, 0.86f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.36f, 0.895f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.41f, 0.895f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.42f, 0.94f, 0.0f);

    glEnd();

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
