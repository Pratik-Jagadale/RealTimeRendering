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
int iHeightOfScreen;
int iWidthOfscreen;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>

float AngleTangle = 0.0f;
float x, y;

float loacationCameraX = 0.0f;
float loacationCameraY = 0.0f;
float loacationCameraZ = 0.0f;

float directionOfCameraX = 0.0f;
float directionOfCameraY = 0.0f;
float directionOfCameraZ = 0.0f;

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;
void setColor(float r, float g, float b);

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

    iHeightOfScreen = GetSystemMetrics(SM_CYSCREEN); // Height of Window Screen
    iWidthOfscreen = GetSystemMetrics(SM_CXSCREEN);  // Width Of Window Screen

    /* Create Window */
    hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
                          TEXT("OpenGL Window - Pratik Jagadale"),
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                          (iWidthOfscreen - WINWIDTH) / 2,
                          (iHeightOfScreen - WINHEIGHT) / 2,
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
        fprintf(gpFile, "Makeing OpnGL as current Context Failed...\n");
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

        case 'w':
            loacationCameraZ = loacationCameraZ - 0.1f;
            // directionOfCameraZ = directionOfCameraZ - 0.1f;
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
    float SCALERATION = -0.25f;

    void drawPiller(void);
    void drawWall(void);
    void drawBigDoor(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // front bottom base

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -9.0f);

    glBegin(GL_POLYGON);
    setColor(211, 211, 211);
    glVertex3f(-10.0f, -4.0f, 0.0f);
    glVertex3f(10.0f, -4.0f, 0.0f);
    setColor(0, 0, 0);
    // setColor(47, 79, 79);
    glVertex3f(10.0f, -4.0f, -9.0f);
    glVertex3f(-10.0f, -4.0f, -9.0f);
    glEnd();

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -9.0f);
    glScalef(1.0f + SCALERATION, 1.0f + SCALERATION, 1.0f + SCALERATION);

    // background wall
    drawWall();
    glLoadIdentity();
    glTranslatef(-6.6, -1.0f, -9.0f);
    glScalef(0.75f, 0.80f + SCALERATION, 0.75f + SCALERATION);

    // small pillers
    for (int i = 0; i <= 7; i++)
    {
        glTranslatef(2.0f, 0.0f, 0.0f);
        drawPiller();
    }

    glLoadIdentity();

    glTranslatef(-5.5f, 0.0f, -9.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);
    // door
    drawBigDoor();

    glLoadIdentity();
    glTranslatef(5.5f, 0.0f, -9.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);
    // door
    drawBigDoor();

    gluLookAt(loacationCameraX, loacationCameraY, loacationCameraZ, directionOfCameraX, directionOfCameraY, directionOfCameraZ, 0.0f, 0.0f, 1.0f);

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

void drawBigDoor(void)
{
    void drawGlass(void);

    // UPPER TRAINGLE
    glBegin(GL_TRIANGLES);
    setColor(120.0f, 49.0f, 58.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);
    glVertex3f(-2.0f, 2.0f, 0.0f);
    glVertex3f(2.0f, 2.0f, 0.0f);

    setColor(171.0f, 110.0f, 80.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.95f, 0.0f);
    glVertex3f(-1.8f, 2.05f, 0.0f);
    glVertex3f(1.8f, 2.05f, 0.0f);
    glEnd();

    // MIDDLE SQAD
    glBegin(GL_POLYGON);
    setColor(51, 29, 12);
    glVertex3f(1.8f, 2.05f, 0.0f);
    glVertex3f(-1.8f, 2.05f, 0.0f);
    glVertex3f(-1.8f, 1.5f, 0.0f);
    glVertex3f(1.8f, 1.5f, 0.0f);
    glEnd();

    // MIDDLE LINES
    glLineWidth(5);
    glBegin(GL_LINES);

    setColor(102, 58, 23);
    glVertex3f(1.8f, 2.04f, 0.0f);
    glVertex3f(-1.8f, 2.0f, 0.0f);

    glVertex3f(1.8f, 1.6f, 0.0f);
    glVertex3f(-1.8f, 1.6f, 0.0f);
    glEnd();

    // bottom QUAD
    glBegin(GL_POLYGON);
    glVertex3f(1.7f, 1.5f, 0.0f);
    glVertex3f(-1.7f, 1.5f, 0.0f);
    glVertex3f(-1.7f, -3.0f, 0.0f);
    glVertex3f(1.7f, -3.0f, 0.0f);
    glEnd();

    // bottom(INNER) QUAD
    glBegin(GL_POLYGON);
    //    setColor(164.0f, 96.0f, 65.0f);
    setColor(51, 29, 12);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -3.0f, 0.0f);
    glVertex3f(1.0f, -3.0f, 0.0f);
    glEnd();

    // glass
    glScalef(0.50f, 0.50f, 0.50f);
    drawGlass();
}

void drawGlass(void)
{
    float x, y;

    // border
    glPointSize(5);
    glBegin(GL_POINTS);
    setColor(51, 29, 12);
    for (float angle = 0.0f; angle <= M_PI; angle = angle + 0.0001f)
    {
        x = 2.01f * cos(angle);
        y = 2.01f * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // glass
    glBegin(GL_LINES);

    for (float angle = 0.0f; angle <= M_PI; angle = angle + 0.008f)
    {
        x = 2.0f * cos(angle);
        y = 2.0f * sin(angle);
        glColor3f(0.015f, 0.850f, 0.850f);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glColor3f(0.835f, 0.898f, 0.949f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // vertical lines
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.349f, 0.211f, 0.180f);
    for (float angle = 0.0f; angle <= M_PI; angle = angle + 0.1f)
    {
        x = 2.0f * cos(angle);
        y = 2.0f * sin(angle);
        glVertex3f(x, 0.0f, 0.0f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // horizontal lines
    glBegin(GL_LINES);
    glColor3f(0.349f, 0.211f, 0.180f);
    y = 0.0f;
    for (float angle = 0.0f; angle <= M_PI / 2; angle = angle + 0.1f)
    {
        x = 2.0f * cos(angle);
        if (y < 2.0f)
            y = y + 0.1f;
        glVertex3f(-x, y, 0.0f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // bootom border
    glLineWidth(5);
    glBegin(GL_LINES);
    setColor(51, 29, 12);
    glVertex3f(2.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 0.0f, 0.0f);
    glEnd();
}

void drawWall(void)
{
    // BACKGRUNW WALL
    glBegin(GL_POLYGON);
    setColor(51, 29, 12);
    glVertex3f(6.5f, 3.0f, -0.5f);
    glVertex3f(-6.5f, 3.0f, -0.5f);
    glVertex3f(-6.5f, -3.5f, -0.5f);
    glVertex3f(6.5f, -3.5f, -0.5f);
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
    setColor(139, 0, 0);
    //    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(6.0f, 3.0f, 0.0f);
    glVertex3f(-6.0f, 3.0f, 0.0f);
    glVertex3f(-6.0f, 2.0f, 0.0f);
    glVertex3f(6.0f, 2.0f, 0.0f);

    glEnd();

    // Line
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(6.0f, 2.9f, 0.0f);
    glVertex3f(-6.0f, 2.9f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(6.0f, 2.8f, 0.0f);
    glVertex3f(-6.0f, 2.8f, 0.0f);
    glEnd();

    // bottom box
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(6.0f, 1.9f, 0.0f);
    glVertex3f(-6.0f, 1.9f, 0.0f);
    glVertex3f(-6.0f, 1.5f, 0.0f);
    glVertex3f(6.0f, 1.5f, 0.0f);
    glEnd();
}

void drawPiller(void)
{
    // TOP BOX
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 3.0f, 0.0f);
    glVertex3f(-0.6f, 3.0f, 0.0f);
    glVertex3f(-0.4f, 2.1f, 0.0f);
    glVertex3f(0.4f, 2.1f, 0.0f);

    glEnd();

    // MIDDLE BOX
    glBegin(GL_POLYGON);
    setColor(102, 58, 23);
    glVertex3f(0.4f, 2.0f, 0.0f);
    glVertex3f(-0.4f, 2.0f, 0.0f);
    glVertex3f(-0.4f, -2.0f, 0.0f);
    glVertex3f(0.4f, -2.0f, 0.0f);
    glEnd();

    // BOTTOM BOX
    glBegin(GL_POLYGON);
    setColor(102, 58, 23);
    glVertex3f(-0.5f, -2.0f, 0.0f);
    glVertex3f(-0.5f, -3.0f, 0.0f);
    glVertex3f(0.5f, -3.0f, 0.0f);
    glVertex3f(0.5f, -2.0f, 0.0f);

    glEnd();
}

void setColor(float r, float g, float b)
{
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

/*
dark dark brown - 51 , 29 , 12
dark brown      - 77 43 17
brown           - 102 58 23
light brown     - 128 72 28
light brown     - 153 87 34

DarkSlateGray   - 47, 79, 79
*/
