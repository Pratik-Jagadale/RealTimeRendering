
/* header files */
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
/* global variables */
bool bFullScreen = false;

#define ON 0
#define OFF 1

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
    // function prototype
    void star2(void);

    // varibale declartions
    static int flag = ON;

    static int i = 0; // flag  variable for inner start white black transforming
    i = i + 1;

    static float innerColor[3] = {0.0f, 0.0f, 0.0f};
    // x[] for outer star
    static float x1[3] = {-0.42f, 1.0f, 0};
    x1[0] = x1[0] + 0.0001;
    x1[1] = x1[1] - 0.0001;

    static float x2[3] = {-0.47f, 0.92f, 0};
    x2[0] = x2[0] + 0.0001;
    x2[1] = x2[1] - 0.0001;

    static float x3[3] = {-0.55f, 0.92f, 0};
    x3[0] = x3[0] + 0.0001;
    x3[1] = x3[1] - 0.0001;

    static float x4[3] = {-0.48f, 0.86f, 0};
    x4[0] = x4[0] + 0.0001;
    x4[1] = x4[1] - 0.0001;

    static float x5[3] = {-0.51f, 0.76f, 0};
    x5[0] = x5[0] + 0.0001;
    x5[1] = x5[1] - 0.0001;

    static float x6[3] = {-0.43f, 0.82f, 0};
    x6[0] = x6[0] + 0.0001;
    x6[1] = x6[1] - 0.0001;

    static float x7[3] = {-0.35f, 0.76f, 0};
    x7[0] = x7[0] + 0.0001;
    x7[1] = x7[1] - 0.0001;

    static float x8[3] = {-0.38f, 0.86f, 0};
    x8[0] = x8[0] + 0.0001;
    x8[1] = x8[1] - 0.0001;

    static float x9[3] = {-0.3f, 0.92f, 0};
    x9[0] = x9[0] + 0.0001;
    x9[1] = x9[1] - 0.0001;

    static float x10[3] = {-0.39f, 0.92f, 0};
    x10[0] = x10[0] + 0.0001;
    x10[1] = x10[1] - 0.0001;

    //y[] inner loop
    static float y1[3] = {-0.42f, 0.94f, 0.0f};
    y1[0] = y1[0] + 0.0001;
    y1[1] = y1[1] - 0.0001;

    static float y2[3] = {-0.445f, 0.895f, 0.0f};
    y2[0] = y2[0] + 0.0001;
    y2[1] = y2[1] - 0.0001;

    static float y3[3] = {-0.495f, 0.895f, 0.0f};
    y3[0] = y3[0] + 0.0001;
    y3[1] = y3[1] - 0.0001;

    static float y4[3] = {-0.45f, 0.865f, 0.0f};
    y4[0] = y4[0] + 0.0001;
    y4[1] = y4[1] - 0.0001;

    static float y5[3] = {-0.47f, 0.81f, 0.0f};
    y5[0] = y5[0] + 0.0001;
    y5[1] = y5[1] - 0.0001;

    static float y6[3] = {-0.43f, 0.845f, 0.0f};
    y6[0] = y6[0] + 0.0001;
    y6[1] = y6[1] - 0.0001;

    static float y7[3] = {-0.385f, 0.815f, 0.0f};
    y7[0] = y7[0] + 0.0001;
    y7[1] = y7[1] - 0.0001;

    static float y8[3] = {-0.4f, 0.86f, 0.0};
    y8[0] = y8[0] + 0.0001;
    y8[1] = y8[1] - 0.0001;

    static float y9[3] = {-0.36f, 0.895f, 0.0f};
    y9[0] = x9[0] + 0.0001;
    y9[1] = x9[1] - 0.0001;

    static float y10[3] = {-0.41f, 0.895f, 0.0f};
    y10[0] = y10[0] + 0.0001;
    y10[1] = y10[1] - 0.0001;

    /* code */

    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(9);

    if (flag == ON) // For Close Transforming
    {
        glBegin(GL_LINE_STRIP);
        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x1[0], x1[1], x1[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x2[0], x2[1], x2[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x3[0], x3[1], x3[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x4[0], x4[1], x4[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x5[0], x5[1], x5[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x6[0], x6[1], x6[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x7[0], x7[1], x7[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x8[0], x8[1], x8[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x9[0], x9[1], x9[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x10[0], x10[1], x10[2]);

        glColor3f(0.94f, 0.79f, 0.01f);
        glVertex3f(x1[0], x1[1], x1[2]);
        glEnd();
        if (x1[0] > 1.0f && x1[1] < -1.0f)
        {
            flag = OFF;
        }
    }

    //Inner Star

    if (i % 50 == 0)
    {
        innerColor[0] = 1.0f;
        innerColor[1] = 1.0f;
        innerColor[2] = 1.0f;
    }
    else
    {
        innerColor[0] = 0.0f;
        innerColor[1] = 0.0f;
        innerColor[2] = 0.0f;
    }
    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y1[0], y1[1], y1[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y2[0], y2[1], y2[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y3[0], y3[1], y3[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y4[0], y4[1], y4[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y5[0], y5[1], y5[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y6[0], y6[1], y6[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y7[0], y7[1], y7[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y8[0], y8[1], y8[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y9[0], y9[1], y9[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y10[0], y10[1], y10[2]);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(y1[0], y1[1], y1[2]);

    glEnd();

    // STAR 2
    star2();

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

void star2(void)
{
    // variable declartions
    static int i = 0; // flag  variable for inner start white black transforming
    i = i + 1;

    static float innerColor[3] = {0.0f, 0.0f, 0.0f};
    static float flag = 0;

    // code
    if (i % 100 == 0)
    {
        innerColor[0] = 1.0f;
        innerColor[1] = 1.0f;
        innerColor[2] = 1.0f;
    }
    else
    {
        innerColor[0] = 0.0f;
        innerColor[1] = 0.0f;
        innerColor[2] = 0.0f;
    }

    // STAR 2***********************
    glLineWidth(7);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.42f + 0.3f, 1.0f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.47f + 0.3f, 0.92f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.55f + 0.3f, 0.92f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.48f + 0.3f, 0.86f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.51f + 0.3f, 0.76f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.43f + 0.3f, 0.82f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.35f + 0.3f, 0.76f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.38f + 0.3f, 0.86f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.3f + 0.3f, 0.92f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.39f + 0.3f, 0.92f, 0);

    glColor3f(0.94f, 0.79f, 0.01f);
    glVertex3f(-0.42f + 0.3f, 1.0f, 0);
    glEnd();

    //Inner Star
    glLineWidth(2);

    glBegin(GL_LINE_STRIP);
    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.42f + 0.3f, 0.94f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.445f + 0.3f, 0.895f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.495f + 0.3f, 0.895f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.45f + 0.3f, 0.865f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.47f + 0.3f, 0.81f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.43f + 0.3f, 0.845f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.385f + 0.3f, 0.815f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.4f + 0.3f, 0.86f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.36f + 0.3f, 0.895f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.41f + 0.3f, 0.895f, 0.0f);

    glColor3f(innerColor[0], innerColor[1], innerColor[2]);
    glVertex3f(-0.42f + 0.3f, 0.94f, 0.0f);

    glEnd();
}