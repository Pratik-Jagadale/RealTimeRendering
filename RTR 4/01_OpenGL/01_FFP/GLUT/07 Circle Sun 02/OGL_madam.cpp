//Header Files
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

//Global Variables - for fullscreen - bool
bool bFullScreen = false;

#define YELLOW 0
#define LAVENDER 1

//Entry-Point Function
int main(int argc, char *argv[])
{
    //Local functions declarations
    void initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void mouse(int, int, int, int);
    void uninitialize(void);

    //code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("RamaVijayGokhale");

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
    //code
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
    //code
    glViewport(0, 0, width, height);
}

void display(void)
{
    //constant declaration
    const float radius = 0.5f;

    static int fInner = 0; //flag for inner vertex
    static int fOuter = 0; //flag for outer vertex

    static float yellow[] = {1.0f, 1.0f, 0.0f};
    static float lavender[] = {0.5f, 0.5f, 1.0f};

    //code
    glClear(GL_COLOR_BUFFER_BIT);

    //for Inner Vertex ->
    if (fInner == YELLOW) // If Inner is Yellow Then Converting yellow[] array to Lavender
    {
        if (yellow[0] > 0.5f)
        {
            yellow[0] = yellow[0] - 0.01f;
        }
        if (yellow[1] > 0.5f)
        {
            yellow[1] = yellow[1] - 0.01f;
        }
        if (yellow[2] <= 1.0f)
        {
            yellow[2] = yellow[2] + 0.01f;

            if (yellow[2] >= 1.0f)
            {
                fInner = LAVENDER; // we are changing the flag here...because this if  will interate longer than other two
            }
        }
    }
    else // If yellow[] array is Lavender... converting yellow to lavender
    {
        if (yellow[0] < 1.0f)
        {
            yellow[0] = yellow[0] + 0.01f;
        }
        if (yellow[1] < 1.0f)
        {
            yellow[1] = yellow[1] + 0.01f;
        }
        if (yellow[2] >= 0.0f)
        {
            yellow[2] = yellow[2] - 0.01f;
            if (yellow[2] <= 0.0f)
            {
                fInner = YELLOW;
            }
        }
    }

    //for Outer Coordinate ->
    if (fOuter == LAVENDER) // If outer is lavender then converting lavender[] array to Yellow
    {
        if (lavender[0] < 1.0f)
        {
            lavender[0] = lavender[0] + 0.01f;
        }
        if (lavender[1] < 1.0f)
        {
            lavender[1] = lavender[1] + 0.01f;
        }
        if (lavender[2] >= 0.0f)
        {
            lavender[2] = lavender[2] - 0.01f;
            if (lavender[2] <= 0.0f)
            {
                fOuter = YELLOW;
            }
        }
    }
    else //if Outer is Yellow then converting Lavender[] array to -> lavender
    {
        if (lavender[0] > 0.5f)
        {
            lavender[0] = lavender[0] - 0.01f;
        }
        if (lavender[1] > 0.5f)
        {
            lavender[1] = lavender[1] - 0.01f;
        }
        if (lavender[2] <= 1.0f)
        {
            lavender[2] = lavender[2] + 0.01f;

            if (lavender[2] >= 1.0f)
            {
                fOuter = LAVENDER;
            }
        }
    }

    glBegin(GL_LINES);
    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.05f)
    {
        //glColor3f(245.0f / 255.0f, 176.0f / 255.0f, 73.0f / 255.0f);
        glColor3f(yellow[0], yellow[1], yellow[2]);
        glVertex3f(0.0f, 0.0f, 0.0f); //center of circle

        float x, y;
        x = radius * cos(angle);
        y = radius * sin(angle);

        glColor3f(lavender[0], lavender[1], lavender[2]);
        //glColor3f(114.0f / 255.0f, 181.0f / 255.0f, 254.0f / 255.0f);
        glVertex3f(x, y, 0.0f);
    }

    glEnd();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    //code
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
    //code
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
    //code
}