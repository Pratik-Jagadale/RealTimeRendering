/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

/* OpenGL libraries */
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

#define WINWIDTH 800
#define WINHEIGHT 600

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
int iHeightOfWindow;
int iWidthOfWindow;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>
GLUquadric *quadric = NULL;

GLfloat radius = 8.0f;
float AngleCube = 270.0f;
float yEyeVector = 0;
GLuint texture_Root;

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;
BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[]);

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    /* function declartions */
    int initialize(void);
    void uninitialize(void);
    void display(void);
    void update(void);
    void uninitialize(void);

    /* variable declarations */
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyWindow");
    BOOL bDone = FALSE;
    int iRetVal = 0;

    // Code
    if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
    {
        MessageBox(NULL, TEXT("Creation of Log File Faile..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
        exit(0);
    }
    else
    {
        fprintf(gpFile, "Log File is Successfuly Created");
    }

    // Initializaion of wndclassex structure
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

    /* Register Above wndclass */
    RegisterClassEx(&wndclass);

    iHeightOfWindow = GetSystemMetrics(SM_CYSCREEN); // Height of Window Screen
    iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN);  // Width Of Window Screen

    /* Create Window */
    hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
                          TEXT("OpenGL - Pratik Jagadale"),
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                          (iWidthOfWindow - WINWIDTH) / 2,
                          (iHeightOfWindow - WINHEIGHT) / 2,
                          WINWIDTH,
                          WINHEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ghwnd = hwnd;

    // initizalize
    iRetVal = initialize();

    if (iRetVal == -1)
    {
        fprintf(gpFile, "Choose Pixel format Failed...\n");
        uninitialize();
    }
    else if (iRetVal == -2)
    {
        fprintf(gpFile, "Set Pixel format Failed...\n");
        uninitialize();
    }
    else if (iRetVal == -3)
    {
        fprintf(gpFile, "Crete OpenGl Context Failed...\n");
        uninitialize();
    }
    else if (iRetVal == -4)
    {
        fprintf(gpFile, "Making OpenGL as current Context Failed...\n");
        uninitialize();
    }
    else if (iRetVal == -6)
    {
        fprintf(gpFile, "LoadGlTexture Failed..\n");
        uninitialize();
    }
    else
    {
        fprintf(gpFile, "Initialize Successfull...\n");
    }

    ShowWindow(hwnd, iCmdShow);

    /* fore grounding and focusing window */
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    /* Game Loop */
    while (bDone == FALSE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                bDone = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (gbActiveWindow == TRUE)
            {
                /* Render the seen */
                display();
                // updatetheseen
                update();
            }
        }
    }

    uninitialize();
    return ((int)msg.wParam);
}

/* CALLBACK Function */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    /* fucntion declarations */
    void resize(int, int);

    // code
    switch (iMsg)
    {
    case WM_SETFOCUS:
        gbActiveWindow = TRUE;
        break;

    case WM_KILLFOCUS:
        gbActiveWindow = FALSE;
        break;

    case WM_ERASEBKGND:
        return (0);

    case WM_CHAR:
        switch (wParam)
        {
        case 'f':
        case 'F':
            ToggleFullScreen();
            break;
        case 'W':
        case 'w':
            radius = radius - 0.1f;

            break;

        case 'S':
        case 's':
            radius = radius + 0.1f;
            break;

        case 'A':
        case 'a':
            AngleCube = AngleCube - 1.0f;
            break;

        case 'D':
        case 'd':
            AngleCube = AngleCube + 1.00f;
            break;

        case 'Q':
        case 'q':
            yEyeVector = yEyeVector + 0.01f;
            break;

        case 'E':
        case 'e':
            yEyeVector = yEyeVector - 0.01f;
            break;

        case 27:
            if (gpFile)
            {
                fprintf(gpFile, "Log File Successfully Closes");
                fclose(gpFile);
                gpFile = NULL;
            }
            PostQuitMessage(0);
            break;

        default:
            break;
        }
        break;

    case WM_SIZE:
        resize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_CLOSE:

        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
    // variable declartions
    static DWORD dwStyle;
    static WINDOWPLACEMENT wp;
    MONITORINFO mi;

    //	code
    wp.length = sizeof(WINDOWPLACEMENT);
    if (gbFullScreen == FALSE)
    {
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
        if (dwStyle & WS_OVERLAPPEDWINDOW)
        {
            mi.cbSize = sizeof(MONITORINFO);

            if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
            {
                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

                SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // nccalksize
            }

            ShowCursor(FALSE);
            gbFullScreen = TRUE;
        }
    }
    else
    {
        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

        SetWindowPlacement(ghwnd, &wp);
        SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

        ShowCursor(TRUE);
        gbFullScreen = FALSE;
    }
}

int initialize(void)
{
    /* fucntion delcations */
    void resize(int, int);

    /* variable declartions */
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;

    /* code */
    /* initialization of pixelformatdesciptor structure */
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32; // 24 also can done

    /* GetDC */
    ghdc = GetDC(ghwnd);

    /* Choose Pixel Format */
    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

    if (iPixelFormatIndex == 0)
        return -1;

    /* Set The choosen Puxel Format */
    if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
        return -2;

    /* briding API */
    /* Create OpenGL Rendering Context */

    ghrc = wglCreateContext(ghdc);
    if (ghrc == NULL)
        return -3;

    /* make the rendering as current cintext */
    if (wglMakeCurrent(ghdc, ghrc) == FALSE)
        return -4;

    /* Here start OpeGL Code */
    /* Clear the  screen using black color */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //

    if (LoadGLTexture(&texture_Root, MAKEINTRESOURCE(IDBITMAP_ROOT_TEXTURE)) == FALSE)
        return -6; // write log in wndproc

    quadric = gluNewQuadric();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

    return (0);
}

void resize(int width, int height)
{
    /* code */
    if (height == 0) // To AVOID DEVIDED BY ZERO
        height = 1;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
    // fucntion declartions
    void drawRoots(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float angle = AngleCube * M_PI / 180.0f;

    gluLookAt(radius * cos(angle), yEyeVector, radius * sin(angle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glTranslatef(0.0f, 0.0f, -3.0f);

    glEnable(GL_TEXTURE_2D);

    drawRoots();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    // AngleCube = AngleCube + 0.05f;
    if (AngleCube >= 360.0f)
        AngleCube = 0.0f;
}

void uninitialize(void)
{
    /* function declarations */
    void ToggleFullScreen(void);

    /* code */
    if (gbFullScreen)
        ToggleFullScreen();

    /*  */
    if (wglGetCurrentContext() == ghrc)
    {
        wglMakeCurrent(NULL, NULL);
    }

    if (ghrc)
    {
        wglDeleteContext(ghrc);
        ghrc = NULL;
    }

    if (ghdc)
    {
        ReleaseDC(ghwnd, ghdc);
        ghwnd = NULL;
        ghdc = NULL;
    }

    if (ghwnd)
    {
        DestroyWindow(ghwnd);
    }

    if (gpFile)
    {
        fprintf(gpFile, "Log File Successfully Closes");
        fclose(gpFile);
        gpFile = NULL;
    }

    if (quadric)
    {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

    if (texture_Root)
    {
        glDeleteTextures(1, &texture_Root);
        texture_Root = 0;
    }
}

float fRootIncline[] = {0.0f, 0.0f, 0.0f};
float fRootTranslate = 0.0f;
BOOL fMainRoot = TRUE;

void drawRoots()
{
    // function declartions
    void drawLeftSideCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);
    void drawRightSideCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);
    void drawFrontRightSideCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);
    void drawBackSideCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);

    void drawBackCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);
    void drawBackNewCurvedRoot(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height);
    // code
    // BASE
    glPushMatrix();
    glScalef(1.0f, 5.0f, 1.0f);
    glBegin(GL_POLYGON);
    glColor3f(0.15f, 0.15f, 0.15f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glEnd();
    glPopMatrix();

    // main root
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(quadric, 0.4f, 0.01f, 4.5f, 100, 100);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    // ROOT 1
    glPushMatrix();
    glScalef(0.60f, 1.0f, 0.60f);
    // rotation around x zxis
    glTranslatef(-0.7f, 0.0f, 0.0f);
    glRotatef(-90.0, 0.60f, 0.0f, 0.0f);
    // slightly rotation around z
    glRotatef(90.0, 0.0f, 0.0f, 1.0f);
    drawLeftSideCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    // rotation around x zxis
    glTranslatef(-0.7f, 0.0f, 0.0f);
    glRotatef(-120.0, 1.0f, 0.0f, 0.0f);
    // slightly rotation around z
    glRotatef(90.0, 0.0f, 0.0f, 1.0f);
    drawLeftSideCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    // ROOT 2
    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    glTranslatef(0.7f, 0.0f, 0.0f);
    // slightly rotation around z
    glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
    drawRightSideCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.6f, 0.60f);
    glTranslatef(0.7f, 0.0f, 0.0f);
    // slightly rotation around z
    glRotatef(-45.0, 1.0f, 0.0f, 0.0f);
    drawFrontRightSideCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    glTranslatef(-0.7f, 0.0f, 0.0f);
    // slightly rotation around z
    glRotatef(-90.0, 1.0f, 0.0f, 1.0f);
    drawBackSideCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    glTranslatef(0.0f, 0.0f, 0.7f);
    // slightly rotation around z
    glRotatef(-60.0, 1.0f, 0.0f, 0.0f);
    drawBackCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    glTranslatef(0.0f, 0.0f, 0.7f);
    // slightly rotation around z
    glRotatef(-120.0, 1.0f, 0.0f, 0.0f);
    drawBackCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.60f, 0.60f, 0.60f);
    glTranslatef(-0.7f, 0.0f, 0.7f);
    // slightly rotation around z
    glRotatef(-120.0, 1.0f, 0.0f, 0.0f);
    drawBackNewCurvedRoot(quadric, 0.3f, 0.4f, 2.0f);
    glPopMatrix();
}

void drawLeftSideCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius <= 0.0f)
        return;

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    // Translation for draw Root
    glTranslatef(0.0f, 0.5f, -height);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

    // draw small roots
    glPushMatrix();
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    // actual root
    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawLeftSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawRightSideCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius <= 0.0f)
        return;

    glTranslatef(0.50f, 0.0f, -height);
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glRotatef(-50.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius - 0.2);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawRightSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);
}

void drawFrontRightSideCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius < 0.0f)
        return;

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    glTranslatef(0.50f, 0.0f, -height);
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // draw small roots
    glPushMatrix();
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    // actual root
    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawRightSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawBackSideCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius < 0.0f)
        return;

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    glTranslatef(0.50f, 0.0f, -height);
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // draw small roots
    glPushMatrix();
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    // actual root
    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawRightSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawBackCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius < 0.0f)
        return;

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    glTranslatef(0.0f, 0.50f, -height);
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);

    // draw small roots
    glPushMatrix();
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    // actual root
    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawRightSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawBackNewCurvedRoot(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height)
{
    if (baseRadius < 0.0f)
        return;

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);

    glTranslatef(0.0f, 0.50f, -height);
    glRotatef(25.0f, 0.0f, 1.0f, 0.0f);

    // draw small roots
    glPushMatrix();
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -topRadius);
    if (!(baseRadius <= 0.1f))
        gluCylinder(qobj, 0.03, 0.05, 0.3, 10, 10);
    glPopMatrix();

    gluQuadricTexture(qobj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, texture_Root);
    // actual root
    gluCylinder(qobj, baseRadius, topRadius, height, 30, 30);
    drawRightSideCurvedRoot(qobj, baseRadius - 0.1f, baseRadius, height - 0.1f);

    glBindTexture(GL_TEXTURE_2D, 0);
}

BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[])
{
    // variable declartions
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    BOOL bResult = FALSE;

    // code
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hBitmap)
    {
        bResult = TRUE;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // create the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        DeleteObject(hBitmap);
    }
    return bResult;
}
