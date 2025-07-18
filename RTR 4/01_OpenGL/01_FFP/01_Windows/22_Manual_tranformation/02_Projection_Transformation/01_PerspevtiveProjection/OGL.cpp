/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
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
float AngleCube = 0.0f;

GLfloat identityMatrix[16];
GLfloat translationMatrix[16];
GLfloat scaleMatrix[16] = {0.75f, 0.75f, 0.75f};

GLfloat rotationMatrixX[16];
GLfloat rotationMatrixY[16];
GLfloat rotationMatrixZ[16];

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Intialization of matrix Arrays

    // Identity Matrix
    identityMatrix[0] = 1.0f;
    identityMatrix[1] = 0.0f;
    identityMatrix[2] = 0.0f;
    identityMatrix[3] = 0.0f;
    identityMatrix[4] = 0.0f;
    identityMatrix[5] = 1.0f;
    identityMatrix[6] = 0.0f;
    identityMatrix[7] = 0.0f;
    identityMatrix[8] = 0.0f;
    identityMatrix[9] = 0.0f;
    identityMatrix[10] = 1.0f;
    identityMatrix[11] = 0.0f;
    identityMatrix[12] = 0.0f;
    identityMatrix[13] = 0.0f;
    identityMatrix[14] = 0.0f;
    identityMatrix[15] = 1.0f;

    // Transalation matrix
    translationMatrix[0] = 1.0f;
    translationMatrix[1] = 0.0f;
    translationMatrix[2] = 0.0f;
    translationMatrix[3] = 0.0f;
    translationMatrix[4] = 0.0f;
    translationMatrix[5] = 1.0f;
    translationMatrix[6] = 0.0f;
    translationMatrix[7] = 0.0f;
    translationMatrix[8] = 0.0f;
    translationMatrix[9] = 0.0f;
    translationMatrix[10] = 1.0f;
    translationMatrix[11] = 0.0f;
    translationMatrix[12] = 0.0f;
    translationMatrix[13] = 0.0f;
    translationMatrix[14] = -6.0f;
    translationMatrix[15] = 1.0f;

    // scale Matrix

    scaleMatrix[0] = 0.75f;
    scaleMatrix[1] = 0.0f;
    scaleMatrix[2] = 0.0f;
    scaleMatrix[3] = 0.0f;
    scaleMatrix[4] = 0.0f;
    scaleMatrix[5] = 0.75f;
    scaleMatrix[6] = 0.0f;
    scaleMatrix[7] = 0.0f;
    scaleMatrix[8] = 0.0f;
    scaleMatrix[9] = 0.0f;
    scaleMatrix[10] = 0.75f;
    scaleMatrix[11] = 0.0f;
    scaleMatrix[12] = 0.0f;
    scaleMatrix[13] = 0.0f;
    scaleMatrix[14] = 0.0f;
    scaleMatrix[15] = 1.0f;

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

// GLOBAL VARIABLE DECLARATIONS

void display(void)
{
    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    // Trangle *****
    // glLoadIdentity();
    glLoadMatrixf(identityMatrix);

    // glTranslatef(0.0f, 0.0f, -4.0f);
    glMultMatrixf(translationMatrix);

    // glRotatef(AngleCube, 1.0f, 1.0f, 1.0f); // Rolling
    glMultMatrixf(scaleMatrix);

    float angle = (AngleCube * M_PI) / 180.0f;
    // xrotation matrix
    rotationMatrixX[0] = 1.0f;
    rotationMatrixX[1] = 0.0f;
    rotationMatrixX[1] = 0.0f;
    rotationMatrixX[3] = 0.0f;
    rotationMatrixX[4] = 0.0f;
    rotationMatrixX[5] = cos(angle);
    rotationMatrixX[6] = sin(angle);
    rotationMatrixX[7] = 0.0f;
    rotationMatrixX[8] = 0.0f;
    rotationMatrixX[9] = -sin(angle);
    rotationMatrixX[10] = cos(angle);
    rotationMatrixX[11] = 0.0f;
    rotationMatrixX[12] = 0.0f;
    rotationMatrixX[13] = 0.0f;
    rotationMatrixX[14] = 0.0f;
    rotationMatrixX[15] = 1.0f;

    // y rotation matrix
    rotationMatrixY[0] = cos(angle);
    rotationMatrixY[1] = 0.0f;
    rotationMatrixY[2] = -sin(angle);
    rotationMatrixY[3] = 0.0f;
    rotationMatrixY[4] = 0.0f;
    rotationMatrixY[5] = 1.0f;
    rotationMatrixY[6] = 0.0f;
    rotationMatrixY[7] = 0.0f;
    rotationMatrixY[8] = sin(angle);
    rotationMatrixY[9] = 0.0f;
    rotationMatrixY[10] = cos(angle);
    rotationMatrixY[11] = 0.0f;
    rotationMatrixY[12] = 0.0f;
    rotationMatrixY[13] = 0.0f;
    rotationMatrixY[14] = 0.0f;
    rotationMatrixY[15] = 1.0f;

    // z rotation matrox
    rotationMatrixZ[0] = cos(angle);
    rotationMatrixZ[1] = sin(angle);
    rotationMatrixZ[2] = 0.0f;
    rotationMatrixZ[3] = 0.0f;
    rotationMatrixZ[4] = -sin(angle);
    rotationMatrixZ[5] = cos(angle);
    rotationMatrixZ[6] = 0.0f;
    rotationMatrixZ[7] = 0.0f;
    rotationMatrixZ[8] = 0.0f;
    rotationMatrixZ[9] = 0.0f;
    rotationMatrixZ[10] = 1.0f;
    rotationMatrixZ[11] = 0.0f;
    rotationMatrixZ[12] = 0.0f;
    rotationMatrixZ[13] = 0.0f;
    rotationMatrixZ[14] = 0.0f;
    rotationMatrixZ[15] = 1.0f;

    glMultMatrixf(rotationMatrixX);
    glMultMatrixf(rotationMatrixY);
    glMultMatrixf(rotationMatrixZ);

    glBegin(GL_QUADS);

    // FRONT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // TOP FACE
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // BOTTOM FACE
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */

    AngleCube = AngleCube + 0.05f;
    if (AngleCube >= 360.0f)
        AngleCube = -360.0f;
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
}
