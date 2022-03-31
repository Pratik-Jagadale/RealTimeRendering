/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <math.h>
#include <winbase.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

/* OpenGL libraries */
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#pragma comment(lib, "winmm.lib")

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

float AnglePyramid = 0.0f;
float AngleCube = 0.0f;

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

    resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

    // Play Sound
    PlaySound(TEXT("GOT.wav"), NULL, SND_FILENAME | SND_ASYNC);

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

float pos_P = -9.0f; // for x
float pos_R = -4.5f; // for x, z
float pos_A = 10.5f; // for x
float pos_T = 1.5f;  // for x, z
float pos_I = 4.6f;  // for x, y
float pos_K = 7.0f;  // for x, y

float pos_I_Y = 7.0f;
float pos_K_Y = -7.0f;

float color_R = 0.0f; // z
float color_T = 0.0f;

int flagT = 0;

float zTranslate = -12.0f;

void display(void)
{
    // function prototype
    void drawBar(void);
    // void drawPRATIK(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f + pos_P, 0.0f, zTranslate);

    // drawPATRIK();

    glColor3f(1.0f, 0.0f, 0.0f);
    // p
    drawBar();

    glScalef(0.75f, 0.75f, 0.0f);
    glTranslatef(1.0f, 1.5f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(1.5f + pos_P, 0.45f, zTranslate);
    glScalef(0.75f, 0.60f, 0.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(1.0f + pos_P, -0.2f, zTranslate);
    glScalef(0.60f, 0.75f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    // R
    glColor3f(color_R, 0.0f, 0.0f);
    glLoadIdentity();
    glTranslatef(0.0f + pos_R, 0.0f, zTranslate);
    drawBar();

    glScalef(0.75f, 0.75f, 0.0f);
    glTranslatef(-3.5f - pos_R, 1.5f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(1.5f + pos_R, 0.4f, zTranslate);
    glScalef(0.75f, 0.75f, 0.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(1.0f + pos_R, -0.5f, zTranslate);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.75f, 0.75f, 0.0f);
    drawBar();

    // A
    glLoadIdentity();
    glTranslatef(-0.5f + pos_A, 0.0f, zTranslate);
    glRotatef(-33.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.1f, 1.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(0.6f + pos_A, 0.0f, zTranslate);
    glRotatef(35.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.2f, 1.0f);
    drawBar();

    // T
    if (flagT == 1)
    {
        glColor3f(color_T, 0.0f, 0.0f);
        glLoadIdentity();
        glTranslatef(0.0f + pos_T, -0.01f, zTranslate);
        drawBar();

        glTranslatef(0.0f, 1.0f, -0.01f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        drawBar();
    }
    // I
    glLoadIdentity();
    glTranslatef(0.0f + pos_I, 0.0f + pos_I_Y, zTranslate);
    drawBar();

    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    glLoadIdentity();
    glTranslatef(0.0f + pos_I, -1.0f + pos_I_Y, zTranslate);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    // K
    glLoadIdentity();
    glTranslatef(0.0f + pos_K, 0.0f + pos_K_Y, zTranslate);
    glScalef(1.3f, 1.0f, 1.0f);
    drawBar();

    glTranslatef(0.5f, -0.5f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.75f, 0.75f, 0.0f);
    drawBar();

    glTranslatef(0.5f, 0.5f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    drawBar();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    if (pos_P < -7.2)
    {
        pos_P = pos_P + 0.001f;
    }
    else if (color_R < 1.0f)
    {
        color_R = color_R + 0.0002f;
    }
    else if (pos_A > -1.0)
    {
        pos_A = pos_A - 0.002f;
        if (pos_A < -0.7f)
            flagT = 1;
    }
    else if (color_T < 1.0f)
    {
        color_T = color_T + 0.0002f;
    }
    else if (pos_I_Y > 0.0f)
    {
        pos_I_Y = pos_I_Y - 0.001f;
    }
    else if (pos_K_Y < 0.0f)
    {
        pos_K_Y = pos_K_Y + 0.001f;
    }
    else
    {
        zTranslate = zTranslate + 0.001f;
    }

    if (zTranslate > -0.2f)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    }
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

void drawBar(void)
{

    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.5f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);
    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    glEnd();
}
