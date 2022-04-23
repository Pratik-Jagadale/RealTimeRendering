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
BOOL bTraingleShow = TRUE;

FILE *gpFile = NULL; // FILE* -> #include<stdio.h>

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setColor4f(float r, float g, float b, float alpha);
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
        case 't':
        case 'T':
            if (bTraingleShow == TRUE)
                bTraingleShow = FALSE;
            else
                bTraingleShow = TRUE;
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

float y = 0.67f;
float y_one = 0.67f;
float y_TWO = 0.300;

float jetA[2] = {-9.0f, -9.0f};
float jetAngleA = 45.0f;

float jetB = -9.0f;

float jetC[2] = {-9.0f, -9.0f};
float jetAngleC = 45.0f;

void display(void)
{
    void fighterJet(void);
    void drawIAF(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -12.0f);

    fighterJet();

    glTranslatef(-4.0f, 0.0f, 0.0f);
    glScalef(0.50f, 0.50f, 0.50f);
    drawIAF();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    y = y - 0.05;
    if (y < 0.300f)
        y = 0.67f;

    y_one = y_one - 0.01;
    if (y_one < 0.300f)
        y_one = 0.67f;

    y_TWO = y_TWO + 0.01;
    if (y_TWO > 0.67f)
        y_TWO = 0.300f;
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

void setColor(float r, float g, float b)
{
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void fighterJet(void)
{

    // top nitro
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(3);
    glBegin(GL_LINES);
    setColor4f(255, 153, 51, 255.0f);
    glVertex3f(-4.20, y, 0.0f);
    setColor4f(255, 153, 51, 0.5f);
    glVertex3f(-8.0, y, 0.0f);

    setColor4f(255, 153, 51, 255.5f);
    glVertex3f(-4.200, y_one, 0.0f);
    setColor4f(255, 153, 51, 0.5f);
    glVertex3f(-8.0, y_one, 0.0f);

    setColor4f(19, 136, 8, 255);
    glVertex3f(-4.200, y_TWO, 0.0f);
    setColor4f(19, 136, 8, 0.5f);
    glVertex3f(-8.0, y_TWO, 0.0f);

    glEnd();

    // bottom nitro
    glBegin(GL_LINES);
    setColor4f(255, 153, 51, 255.5f);
    glVertex3f(-4.200, -y_one, 0.0f);
    setColor4f(255, 153, 51, 0.5f);
    glVertex3f(-8.0, -y_one, 0.0f);

    setColor4f(19, 136, 8, 255);
    glVertex3f(-4.20, y - 0.9f, 0.0f);
    setColor4f(255, 153, 51, 0.5f);
    glVertex3f(-8.0, y - 0.9f, 0.0f);

    setColor4f(19, 136, 8, 255);
    glVertex3f(-4.20, y_one - 0.9f, 0.0f);
    setColor4f(255, 153, 51, 0.5f);
    glVertex3f(-8.0, y_one - 0.9f, 0.0f);

    glEnd();

    glDisable(GL_BLEND);

    ///////////////// top wing
    glBegin(GL_POLYGON);
    setColor(186, 226, 238);
    // setColor(0, 212, 250);
    glVertex3f(1.36f, 0.760f, 0.0f);
    glVertex3f(0.6f, 1.597f, 0.0f);
    glVertex3f(0.24f, 1.613f, 0.0f);
    glVertex3f(0.56f, 0.818f, 0.0f);
    glEnd();

    // bottom wing
    glBegin(GL_POLYGON);
    glVertex3f(1.36, -0.733, 0.0f);
    glVertex3f(0.443, -0.9, 0.0f);
    glVertex3f(0.251, -1.565, 0.0f);
    glVertex3f(0.568, -1.5664, 0.0f);
    glEnd();

    ////////////////
    // tip
    glBegin(GL_POLYGON);
    glVertex3f(4.394, 0.014, 0.0f);
    glVertex3f(3.618, 0.224, 0.0f);
    glVertex3f(2.962, 0.331, 0.0f);
    glVertex3f(2.175, 0.5325, 0.0f);
    glVertex3f(1.512, 0.640, 0.0f);
    glVertex3f(1.138, 0.650, 0.0f);
    glVertex3f(1.138, -0.650, 0.0f);
    glVertex3f(1.437, -0.640, 0.0f);
    glVertex3f(1.726, -0.600, 0.0f);
    glVertex3f(2.423, -0.404, 0.0f);
    glVertex3f(2.823, -0.312, 0.0f);
    glVertex3f(3.513, -0.206, 0.0f);
    glVertex3f(3.931, -0.125, 0.0f);
    glVertex3f(4.39, 0.008000, 0.0f);
    glEnd();

    // back
    glBegin(GL_POLYGON);
    glVertex3f(-0.256, 1.155, 0.0f);
    glVertex3f(-2.242, 3.127, 0.0f);
    glVertex3f(-2.279, 3.1546, 0.0f);
    glVertex3f(-3.194, 3.183, 0.0f);
    glVertex3f(-3.351, 0.93, 0.0f);
    glVertex3f(-3.7883, 0.9295, 0.0f);
    glVertex3f(-3.7955, 0.7366, 0.0f);
    glVertex3f(-4.1017, 0.672, 0.0f);
    glVertex3f(-4.098, 0.2597, 0.0f);
    glVertex3f(-3.808, 0.211, 0.0f);
    glVertex3f(-4.091, 0.193, 0.0f);
    glVertex3f(-4.093, 0.141, 0.0f);
    glVertex3f(-4.248, 0.1155, 0.0f);
    glVertex3f(-4.258, 0.078, 0.0f);
    glVertex3f(-4.732, 0.0725, 0.0f);
    glVertex3f(-4.74, -0.026, 0.0f);
    glVertex3f(-4.267, -0.0294, 0.0f);
    glVertex3f(-4.2404, -0.0754, 0.0f);
    glVertex3f(-4.0995, -0.092, 0.0f);
    glVertex3f(-4.0966, -0.1646, 0.0f);
    glVertex3f(-3.8896, -0.1804, 0.0f);
    glVertex3f(-4.096, -0.229, 0.0f);
    glVertex3f(-4.0976, -0.642, 0.0f);
    glVertex3f(-3.798, -0.6916, 0.0f);
    glVertex3f(-3.7935, -0.884, 0.0f);
    glVertex3f(-3.348, -0.887, 0.0f);
    glVertex3f(-3.189, -3.0945, 0.0f);
    glVertex3f(-2.2505, -3.109, 0.0f);
    glVertex3f(-0.256, -1.11300, 0.0f);
    glEnd();

    // middle
    glBegin(GL_POLYGON);
    glVertex3f(1.39, 0.76, 0.0f);
    glVertex3f(-0.256, 1.155, 0.0f);
    glVertex3f(-0.256, -1.11300, 0.0f);
    glVertex3f(1.39, -0.733, 0.0f);

    glEnd();

    glLineWidth(1);

    glBegin(GL_LINE_LOOP);
    setColor(0, 0, 0);
    glVertex3f(2.972, 0.15500, 0.0f);
    glVertex3f(2.276, 0.283, 0.0f);
    glVertex3f(0.772, 0.298, 0.0f);
    glVertex3f(0.651, 0.21, 0.0f);
    glVertex3f(0.563, 0.112, 0.0f);
    glVertex3f(0.552, -0.034, 0.0f);
    glVertex3f(0.627, -0.16, 0.0f);
    glVertex3f(0.778, -0.264, 0.0f);
    glVertex3f(2.21, -0.253, 0.0f);
    glVertex3f(2.979, -0.121, 0.0f);
    glVertex3f(3.007, 0, 0.0f);
    glEnd();

    setColor(0, 0, 0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(0.556, 0.803, 0.0f);
    glVertex3f(-0.26, 1.15, 0.0f);
    glVertex3f(-2.826, 1.143, 0.0f);
    glVertex3f(-2.82, 0.94, 0.0f);
    glVertex3f(-3.788, 0.918, 0.0f);
    glVertex3f(-3.788, -0.87, 0.0f);
    glVertex3f(-2.81, -0.888, 0.0f);
    glVertex3f(-2.804, -1.109, 0.0f);
    glVertex3f(-0.242, -1.106, 0.0f);
    glVertex3f(0.569, -0.77, 0.0f);

    glEnd();

    glLineWidth(10);
    glBegin(GL_LINES);
    setColor(186, 226, 238);
    glVertex3f(-1.754, 3.2, 0.0f);
    glVertex3f(-3.379, 3.2, 0.0f);

    glVertex3f(-1.754 + 1.0f, 3.2 - 1.0f, 0.0f);
    glVertex3f(-3.379 + 1.0f, 3.2 - 1.0f, 0.0f);

    glVertex3f(-1.754, -3.1, 0.0f);
    glVertex3f(-3.379, -3.1, 0.0f);

    glVertex3f(-1.754 + 1.0f, -3.1 + 1.0f, 0.0f);
    glVertex3f(-3.379 + 1.0f, -3.1 + 1.0f, 0.0f);

    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);

    glVertex3f(-2.803, 3.128, 0.0f);
    glVertex3f(-3.14, 3.132, 0.0f);
    glVertex3f(-3.308, 0.926, 0.0f);
    glVertex3f(-2.808, 0.932, 0.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);

    glVertex3f(-2.803, -3.128, 0.0f);
    glVertex3f(-3.14, -3.132, 0.0f);
    glVertex3f(-3.308, -0.926, 0.0f);
    glVertex3f(-2.808, -0.932, 0.0f);
    glEnd();

    glScalef(0.75f, 0.75f, 0.75f);
    glTranslatef(0.3f, 0.0f, 0.0f);

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    setColor(0, 212, 250);
    glVertex3f(2.972, 0.15500, 0.0f);
    glVertex3f(2.276, 0.283, 0.0f);
    glVertex3f(0.772, 0.298, 0.0f);
    glVertex3f(0.651, 0.21, 0.0f);
    glVertex3f(0.563, 0.112, 0.0f);
    glVertex3f(0.552, -0.034, 0.0f);
    glVertex3f(0.627, -0.16, 0.0f);
    glVertex3f(0.778, -0.264, 0.0f);
    glVertex3f(2.21, -0.253, 0.0f);
    glVertex3f(2.979, -0.121, 0.0f);
    glVertex3f(3.007, 0, 0.0f);
    glEnd();
}

void setColor4f(float r, float g, float b, float alpha)
{
    glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f);
}

void drawIAF(void)
{
    void drawI(void);
    void drawA(void);
    void drawF(void);

    glLineWidth(3);
    drawI();
    glTranslatef(2.2f, 0.0f, 0.0f);
    drawA();

    glTranslatef(2.5f, 0.0f, 0.0f);
    drawF();
}

void drawI(void)
{
    glBegin(GL_LINES);
    setColor(255, 153, 51);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
}
void drawA(void)
{
    glBegin(GL_LINES);
    setColor(255, 255, 255);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
}

void drawF(void)
{
    glBegin(GL_LINES);
    setColor(19, 136, 8);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
}