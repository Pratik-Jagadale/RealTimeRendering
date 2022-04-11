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
                          TEXT("Railway Station"),
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
    /* Function declartiona */
    void drawRailwayStation(void);
    void drawStation(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);

    // drawRailwayStation();
    drawStation();

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

/*
    Draw Railway Station
*/
#define PI 3.1459265

const float r1 = 0.42f;
const float r2 = 0.39f;
const float r3 = 0.37f;

const float clockTime = -(3.1459265 / 5) / 6;
SYSTEMTIME st;

void drawRailwayStation(void)
{
    /* Function Prototyoes */
    void drawStation(void);

    /* Variable Declarations */
    drawStation();
}

void drawStation(void)
{
    // fucntion prototype
    void drawClock(void);
    drawClock();
    // Center Vertical

    /* Center sqaur main */
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);

    glVertex3f(-1.4025f, 0.132f, 0.50f);
    glVertex3f(-1.4f, -0.800f, 0.50f);
    glVertex3f(1.399f, -0.800f, 0.50f);
    glVertex3f(1.3925, 0.1445f, 0.50f);

    /* Center  swaur terece */
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(-1.226f, 0.4325f, 0.50f);
    glVertex3f(-1.4025, 0.132f, 0.50f);
    glVertex3f(1.3925, 0.132f, 0.50f);
    glVertex3f(1.2195, 0.4325f, 0.50f);

    glEnd();

    // front middle wall
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-0.759f, 0.4357f, 0.50f);
    glVertex3f(-0.7566f, -0.800f, 0.50f);
    glVertex3f(0.7495f, -0.800f, 0.50f);
    glVertex3f(0.745f, 0.4357f, 0.50f);

    glEnd();

    // circle - top
    glColor3f(0.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    for (float angle = 0.0f; angle < PI; angle = angle + 0.001f)
    {
        glVertex3f(0.0f, 0.422f, 0.50f);
        float x, y;
        x = 0.4544f * cos(angle);
        y = (0.4544f * sin(angle)) + 0.4357;
        glVertex3f(x, y, 0.50f);
    }
    glEnd();

    // DRAW CLOCK

    // glScalef(0.5f, 0.5f, 0.0f);
}

//
//
//
//
//
//
//
// CLOCK
void drawClock(void)
{
    // function prototypes
    void DrawCircle(void);
    void DrawInnerBackgroundCircle(void);
    void DrawPoints(void);
    void DrawHrHand(void);
    void DrawMinutrHand(void);
    void DrawSecondHand(void);
    void DrawSubLines(void);
    void DrawCenter(void);

    /* code */
    GetSystemTime(&st);

    glClear(GL_COLOR_BUFFER_BIT);

    DrawCircle();
    DrawInnerBackgroundCircle();
    DrawPoints();
    DrawHrHand();
    DrawMinutrHand();
    DrawSecondHand();
    DrawSubLines();
    DrawCenter();
}

/* DRAW FUCNTIONS */
void DrawCircle(void)
{
    // Outer Color
    glBegin(GL_POINTS);
    for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
    {
        float x, y;
        x = r1 * cos(angle);
        y = r1 * sin(angle);
        glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
        glVertex3f(x, y, 1.0f);
    }
    glEnd();
}

void DrawInnerBackgroundCircle(void)
{
    // Inner Back Ground Circle
    glBegin(GL_LINES);
    for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
    {

        glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
        glVertex3f(0.0f, 0.0f, -0.60f);
        float x, y;
        x = r2 * cos(angle);
        y = r2 * sin(angle);
        glColor3f(0.184313725490196f, 0.2705882352f, 0.3803921568627f);
        glVertex3f(x, y, -0.60f);
    }
    glEnd();
}
// Points

void DrawPoints(void)
{
    glPointSize(10);
    glBegin(GL_POINTS);
    for (float angle = 0.0f; angle < PI * 2; angle = angle + (PI / 2))
    {
        float x, y;
        x = r3 * cos(angle);
        y = r3 * sin(angle);
        glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
        glVertex3f(x, y, 0.50f);
    }
    glEnd();
}

void DrawHrHand(void)
{
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0.4f, 0.33725f, 0.494117f);
    glVertex3f(0.0f, 0.0f, 0.50f);
    float x = 0.0, y = 0.0;
    x = 0.17 * cos(clockTime * st.wHour);
    y = 0.17 * sin(clockTime * st.wHour);
    glColor3f(0.4f, 0.33725f, 0.494117f);
    glVertex3f(x, y, 0.50f);
    glEnd();
}

void DrawMinutrHand(void)
{
    // Minute Hand
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(0.4f, 0.33725f, 0.494117f);
    glVertex3f(0.0f, 0.0f, 0.50f);
    float x = 0.0, y = 0.0;
    x = 0.24 * cos(clockTime * st.wMinute);
    y = 0.24 * sin(clockTime * st.wMinute);
    glColor3f(0.4f, 0.33725f, 0.494117f);
    glVertex3f(x, y, 0.50f);

    glEnd();
}
void DrawSecondHand(void)
{
    // Second Hand
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
    glVertex3f(0.0f, 0.0f, 0.50f);
    float x = 0.0, y = 0.0;
    x = 0.25 * cos(clockTime * st.wSecond);
    y = 0.25 * sin(clockTime * st.wSecond);
    glColor3f(1.0f, 0.0f, 0.1f);
    glVertex3f(x, y, 0.50f);
    glEnd();
}

void DrawSubLines(void)
{
    // SubPoins
    glPointSize(1);
    glBegin(GL_LINES);
    for (float angle = 0.0f; angle < PI * 2; angle = angle + (PI / 6.0f))
    {
        float x, y;
        x = r3 * cos(angle);
        y = r3 * sin(angle);
        glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
        glVertex3f(x, y, 0.50f);

        glColor3f(0.894117647058823529f, 0.87058823529f, 0.8235294117647f);
        if (x > 0 && y > 0)
        {
            glVertex3f(x - 0.0001, y - 0.0001, 0.50f);
        }
        else if (x > 0 && y < 0)
        {
            glVertex3f(x - 0.0001, y + 0.0001, 0.50f);
        }
        else if (x < 0 && y < 0)
        {
            glVertex3f(x + 0.0001, y + 0.0001, 0.50f);
        }
        else if (x < 0 && y > 0)
        {
            glVertex3f(x + 0.0001, y - 0.0001, 0.50f);
        }
    }
    glEnd();
}

void DrawCenter(void)
{
    // Center
    glPointSize(8);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.50f);
    glEnd();
}