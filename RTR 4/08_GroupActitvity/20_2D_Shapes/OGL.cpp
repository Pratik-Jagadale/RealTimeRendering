/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
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

int iShapeFlag = 0;

#define PI 3.1459265

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
                          TEXT("2D Shape Assignment"),
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

        case 'a':
        case 'A':
            iShapeFlag = 1;
            break;

        case 'b':
        case 'B':
            iShapeFlag = 2;
            break;

        case 'c':
        case 'C':
            iShapeFlag = 3;
            break;

        case 'd':
        case 'D':
            iShapeFlag = 4;
            break;

        case 'e':
        case 'E':
            iShapeFlag = 5;
            break;

        case 'g':
        case 'G':
            iShapeFlag = 6;
            break;

        case 'h':
        case 'H':
            iShapeFlag = 7;
            break;

        case 'i':
        case 'I':
            iShapeFlag = 8;
            break;

        case 'j':
        case 'J':
            iShapeFlag = 9;
            break;

        case 'k':
        case 'K':
            iShapeFlag = 10;
            break;

        case 'l':
        case 'L':
            iShapeFlag = 11;
            break;

        default:
            iShapeFlag = 0;
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
    /* Code */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);

    /* LINE */

    glLineWidth(7);
    switch (iShapeFlag)
    {
    case 0:
        glPointSize(15);
        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        break;
    case 1:
        /* LINE */
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5, 0.0f, 0.0f);
        glVertex3f(0.5, 0.0f, 0.0f);
        glEnd();
        break;

    case 2:
        /* TRANGEL */
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glEnd();
        break;

    case 3:
        /* SQUARE */
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);
        glVertex3f(-0.4f, -0.4f, 0.0f);
        glVertex3f(0.4f, -0.4f, 0.0f);
        glVertex3f(0.4f, 0.4f, 0.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);

        glEnd();
        break;

    case 4:
        /* Pentagon */
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);
        glVertex3f(-0.4f, -0.4f, 0.0f);
        glVertex3f(0.4f, -0.4f, 0.0f);
        glVertex3f(0.4f, 0.4f, 0.0f);
        glVertex3f(0.0f, 0.8f, 0.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);

        glEnd();
        break;

    case 5:
        /* hexagon */
        glBegin(GL_LINE_STRIP);

        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);
        glVertex3f(-0.4f, -0.4f, 0.0f);
        glVertex3f(0.0f, -0.8f, 0.0f);
        glVertex3f(0.4f, -0.4f, 0.0f);
        glVertex3f(0.4f, 0.4f, 0.0f);
        glVertex3f(0.4f, 0.4f, 0.0f);
        glVertex3f(0.0f, 0.8f, 0.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);

        glEnd();
        break;

    case 6:
        /* Heptagon */
        glBegin(GL_LINE_STRIP);

        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.458f, 0.0f);
        glVertex3f(-0.373f, 0.279f, 0.0f);
        glVertex3f(-0.469f, -0.129f, 0.0f);
        glVertex3f(-0.2084f, -0.456f, 0.0f);
        glVertex3f(0.208f, -0.454f, 0.0f);
        glVertex3f(0.468f, -0.125f, 0.0f);
        glVertex3f(0.371f, 0.276f, 0.0f);
        glVertex3f(0.0f, 0.458f, 0.0f);
        glEnd();
        break;

    case 7:
        /* Octagon */
        glBegin(GL_LINE_STRIP);

        glColor3f(0.5f, 0.5f, 1.0f);
        glVertex3f(-0.203f, 0.463f, 0.0f);
        glVertex3f(-0.471f, 0.197f, 0.0f);
        glVertex3f(-0.471f, -0.1875f, 0.0f);
        glVertex3f(-0.2f, -0.458f, 0.0f);
        glVertex3f(0.17f, -0.459f, 0.0f);
        glVertex3f(0.444f, -0.185f, 0.0f);
        glVertex3f(0.442f, 0.191f, 0.0f);
        glVertex3f(0.174, 0.46f, 0.0f);
        glVertex3f(-0.203f, 0.463f, 0.0f);
        glEnd();

        break;

    case 8:
        // Nonagone
        glBegin(GL_LINE_STRIP);

        glColor3f(0.5f, 0.5f, 0.0f);
        glVertex3f(-0.0055f, 0.466f, 0.0f);
        glVertex3f(-0.308f, 0.353f, 0.0f);
        glVertex3f(-0.474f, 0.074f, 0.0f);
        glVertex3f(-0.417f, -0.252f, 0.0f);
        glVertex3f(-0.164f, -0.462f, 0.0f);
        glVertex3f(0.167f, -0.46f, 0.0f);
        glVertex3f(0.416f, -0.251f, 0.0f);
        glVertex3f(0.475f, 0.0675f, 0.0f);
        glVertex3f(0.306f, 0.349f, 0.0f);
        glVertex3f(-0.0055f, 0.466f, 0.0f);
        glEnd();

        break;

    case 9:
        // Decagon
        glBegin(GL_LINE_STRIP);

        glColor3f(0.5f, 0.5f, 5.0f);
        glVertex3f(-0.149, 0.4555f, 0.0f);
        glVertex3f(-0.39, 0.282f, 0.0f);
        glVertex3f(-0.482, -0.001f, 0.0f);
        glVertex3f(-0.388, -0.28f, 0.0f);
        glVertex3f(-0.148, -0.452f, 0.0f);
        glVertex3f(0.145, -0.453f, 0.0f);
        glVertex3f(0.384, -0.28f, 0.0f);
        glVertex3f(0.478, 0.0f, 0.0f);
        glVertex3f(0.384, 0.2815f, 0.0f);
        glVertex3f(0.148, 0.455f, 0.0f);
        glVertex3f(-0.149, 0.4555f, 0.0f);
        glEnd();
        break;

    case 10:
        // circle
        glBegin(GL_LINES);
        for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.001f)
        {
            glVertex3f(0.0f, 0.0f, 0.0f);
            float x, y;
            x = 0.4544f * cos(angle);
            y = 0.4544f * sin(angle);
            glVertex3f(x, y, 0.50f);
        }
        glEnd();

        break;

    case 11:
        // Ellipse
        glBegin(GL_LINES);
        for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.001f)
        {
            glVertex3f(0.0f, 0.0f, 0.0f);
            float x, y;
            x = (0.4544f + 0.3f) * cos(angle);
            y = (0.4544f * sin(angle));
            glVertex3f(x, y, 0.50f);
        }
        glEnd();

    default:
        glPointSize(10);
        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        break;
    }

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
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
