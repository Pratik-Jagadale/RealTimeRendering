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
int day = 0;
int year = 0;
int moon = 0;
GLUquadric *quadric = NULL;

BOOL bLight = FALSE;
GLfloat lightAmbiantZero[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDiffuseZero[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecularZero[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositionZero[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat light_Model_Ambiant[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_Model_Local_Viewer[] = {0.0f};

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLint keyPressed = 0;

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

        case 'L':
        case 'l':
            if (bLight == FALSE)
            {
                glEnable(GL_LIGHTING);
                bLight = TRUE;
            }
            else
            {
                glDisable(GL_LIGHTING);
                bLight = FALSE;
            }

            break;

        case 'x':
        case 'X':
            keyPressed = 1;
            angleForXRotation = 0.0f; // reset
            break;

        case 'y':
        case 'Y':
            keyPressed = 2;
            angleForYRotation = 0.0f; // reset
            break;

        case 'z':
        case 'Z':
            keyPressed = 3;
            angleForZRotation = 0.0f; // reset
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
            keyPressed = 0;
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

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // light related changes
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_Model_Ambiant);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, light_Model_Local_Viewer);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbiantZero);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseZero);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularZero);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

    glEnable(GL_LIGHT0);

    /* Clear the  screen using black color */
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // quadric intialliza
    // create quadric
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
    // function prototype
    void colorSetcolor(int r, int g, int b);
    void draw24Sphere(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat x, y;
    GLfloat angle;

    if (keyPressed == 1)
    {
        angle = (angleForXRotation * M_PI) / 180.0f;
        x = 45.0f * cos(angle);
        y = 45.0f * sin(angle);

        lightPositionZero[1] = x;
        lightPositionZero[2] = y;
    }
    else if (keyPressed == 2)
    {
        angle = angleForYRotation * (M_PI / 180.0f);
        x = 55.0f * cos(angle);
        y = 55.0f * sin(angle);

        lightPositionZero[0] = x;
        lightPositionZero[2] = y;
    }
    else if (keyPressed == 3)
    {
        angle = angleForZRotation * (M_PI / 180.0f);
        x = 45.0f * cos(angle);
        y = 45.0f * sin(angle);

        lightPositionZero[1] = x;
        lightPositionZero[0] = y;
    }
    else
    {
        lightPositionZero[0] = 0.0f;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //    glTranslatef(0.0f, 0.0f, -4.0f);

    draw24Sphere();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    if (keyPressed == 1)
    {
        angleForXRotation = angleForXRotation + 1.0f;
        if (angleForXRotation > 360.0f)
            angleForXRotation = angleForXRotation - 360.0f;
    }

    if (keyPressed == 2)
    {
        angleForYRotation = angleForYRotation + 1.00f;
        if (angleForYRotation > 360.0f)
            angleForYRotation = angleForYRotation - 360.0f;
    }

    if (keyPressed == 3)
    {
        angleForZRotation = angleForZRotation + 1.0f;
        if (angleForZRotation > 360.0f)
            angleForZRotation = angleForZRotation - 360.0f;
    }
}

void draw24Sphere(void)
{
    // variable declartions
    GLfloat materialAmbiant[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShinniness;

    glTranslatef(0.0f, 0.0f, -19.0f);
    glPushMatrix();

    // ***** 1st sphere on 1st column, emerald *****
    // ambient material
    materialAmbiant[0] = 0.0215; // r
    materialAmbiant[1] = 0.1745; // g
    materialAmbiant[2] = 0.0215; // b
    materialAmbiant[3] = 1.0f;   // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.07568; // r
    materialDiffuse[1] = 0.61424; // g
    materialDiffuse[2] = 0.07568; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.633;    // r
    materialSpecular[1] = 0.727811; // g
    materialSpecular[2] = 0.633;    // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.6 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-9.0f, 6.0f, 0.0f);
    // glTranslatef(1.5f, 14.0f, 0.0f); // x + 6  ,
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 2nd sphere on 1st column, jade *****
    // ambient material
    materialAmbiant[0] = 0.135;  // r
    materialAmbiant[1] = 0.2225; // g
    materialAmbiant[2] = 0.1575; // b
    materialAmbiant[3] = 1.0f;   // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.54; // r
    materialDiffuse[1] = 0.89; // g
    materialDiffuse[2] = 0.63; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.316228; // r
    materialSpecular[1] = 0.316228; // g
    materialSpecular[2] = 0.316228; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.1 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-9.0f, 3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 3rd sphere on 1st column, obsidian *****
    // ambient material
    materialAmbiant[0] = 0.05375; // r
    materialAmbiant[1] = 0.05;    // g
    materialAmbiant[2] = 0.06625; // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.18275; // r
    materialDiffuse[1] = 0.17;    // g
    materialDiffuse[2] = 0.22525; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.332741; // r
    materialSpecular[1] = 0.328634; // g
    materialSpecular[2] = 0.346435; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.3 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-9.0f, 1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 4th sphere on 1st column, pearl *****
    // ambient material
    materialAmbiant[0] = 0.25;    // r
    materialAmbiant[1] = 0.20725; // g
    materialAmbiant[2] = 0.20725; // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 1.0;   // r
    materialDiffuse[1] = 0.829; // g
    materialDiffuse[2] = 0.829; // b
    materialDiffuse[3] = 1.0f;  // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.296648; // r
    materialSpecular[1] = 0.296648; // g
    materialSpecular[2] = 0.296648; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.088 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-9.0f, -1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 5th sphere on 1st column, ruby *****
    // ambient material
    materialAmbiant[0] = 0.1745;  // r
    materialAmbiant[1] = 0.01175; // g
    materialAmbiant[2] = 0.01175; // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.61424; // r
    materialDiffuse[1] = 0.04136; // g
    materialDiffuse[2] = 0.04136; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.727811; // r
    materialSpecular[1] = 0.626959; // g
    materialSpecular[2] = 0.626959; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.6 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-9.0f, -3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 6th sphere on 1st column, turquoise *****
    // ambient material
    materialAmbiant[0] = 0.1;     // r
    materialAmbiant[1] = 0.18725; // g
    materialAmbiant[2] = 0.1745;  // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.396;   // r
    materialDiffuse[1] = 0.74151; // g
    materialDiffuse[2] = 0.69102; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.297254; // r
    materialSpecular[1] = 0.30829;  // g
    materialSpecular[2] = 0.306678; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.1 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-9.0f, -6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 2nd column, brass *****
    // ambient material
    materialAmbiant[0] = 0.329412; // r
    materialAmbiant[1] = 0.223529; // g
    materialAmbiant[2] = 0.027451; // b
    materialAmbiant[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.780392; // r
    materialDiffuse[1] = 0.568627; // g
    materialDiffuse[2] = 0.113725; // b
    materialDiffuse[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.992157; // r
    materialSpecular[1] = 0.941176; // g
    materialSpecular[2] = 0.807843; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.21794872 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, 6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 2nd sphere on 2nd column, bronze *****
    // ambient material
    materialAmbiant[0] = 0.2125; // r
    materialAmbiant[1] = 0.1275; // g
    materialAmbiant[2] = 0.054;  // b
    materialAmbiant[3] = 1.0f;   // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.714;   // r
    materialDiffuse[1] = 0.4284;  // g
    materialDiffuse[2] = 0.18144; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.393548; // r
    materialSpecular[1] = 0.271906; // g
    materialSpecular[2] = 0.166721; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.2 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, 3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 3rd sphere on 2nd column, chrome *****
    // ambient material
    materialAmbiant[0] = 0.25; // r
    materialAmbiant[1] = 0.25; // g
    materialAmbiant[2] = 0.25; // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.4;  // r
    materialDiffuse[1] = 0.4;  // g
    materialDiffuse[2] = 0.4;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.774597; // r
    materialSpecular[1] = 0.774597; // g
    materialSpecular[2] = 0.774597; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.6 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, 1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 4th sphere on 2nd column, copper *****
    // ambient material
    materialAmbiant[0] = 0.19125; // r
    materialAmbiant[1] = 0.0735;  // g
    materialAmbiant[2] = 0.0225;  // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.7038;  // r
    materialDiffuse[1] = 0.27048; // g
    materialDiffuse[2] = 0.0828;  // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.256777; // r
    materialSpecular[1] = 0.137622; // g
    materialSpecular[2] = 0.086014; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.1 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, -1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 5th sphere on 2nd column, gold *****
    // ambient material
    materialAmbiant[0] = 0.24725; // r
    materialAmbiant[1] = 0.1995;  // g
    materialAmbiant[2] = 0.0745;  // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.75164; // r
    materialDiffuse[1] = 0.60648; // g
    materialDiffuse[2] = 0.22648; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.628281; // r
    materialSpecular[1] = 0.555802; // g
    materialSpecular[2] = 0.366065; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.4 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, -3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 6th sphere on 2nd column, silver *****
    // ambient material
    materialAmbiant[0] = 0.19225; // r
    materialAmbiant[1] = 0.19225; // g
    materialAmbiant[2] = 0.19225; // b
    materialAmbiant[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.50754; // r
    materialDiffuse[1] = 0.50754; // g
    materialDiffuse[2] = 0.50754; // b
    materialDiffuse[3] = 1.0f;    // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.508273; // r
    materialSpecular[1] = 0.508273; // g
    materialSpecular[2] = 0.508273; // b
    materialSpecular[3] = 1.0f;     // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.4 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-3.0f, -6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 3rd column, black *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.01; // r
    materialDiffuse[1] = 0.01; // g
    materialDiffuse[2] = 0.01; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.50; // r
    materialSpecular[1] = 0.50; // g
    materialSpecular[2] = 0.50; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, 6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 2nd sphere on 3rd column, cyan *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.1;  // g
    materialAmbiant[2] = 0.06; // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.0;        // r
    materialDiffuse[1] = 0.50980392; // g
    materialDiffuse[2] = 0.50980392; // b
    materialDiffuse[3] = 1.0f;       // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.50196078; // r
    materialSpecular[1] = 0.50196078; // g
    materialSpecular[2] = 0.50196078; // b
    materialSpecular[3] = 1.0f;       // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, 3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 3rd sphere on 2nd column, green *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.1;  // r
    materialDiffuse[1] = 0.35; // g
    materialDiffuse[2] = 0.1;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.45; // r
    materialSpecular[1] = 0.55; // g
    materialSpecular[2] = 0.45; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, 1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 4th sphere on 3rd column, red *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.5;  // r
    materialDiffuse[1] = 0.0;  // g
    materialDiffuse[2] = 0.0;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.7;  // r
    materialSpecular[1] = 0.6;  // g
    materialSpecular[2] = 0.6;  // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, -1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 5th sphere on 3rd column, white *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.55; // r
    materialDiffuse[1] = 0.55; // g
    materialDiffuse[2] = 0.55; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.70; // r
    materialSpecular[1] = 0.70; // g
    materialSpecular[2] = 0.70; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, -3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 6th sphere on 3rd column, yellow plastic *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.5;  // r
    materialDiffuse[1] = 0.5;  // g
    materialDiffuse[2] = 0.0;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.60; // r
    materialSpecular[1] = 0.60; // g
    materialSpecular[2] = 0.50; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.25 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(3.0f, -6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 4th column, black *****
    // ambient material
    materialAmbiant[0] = 0.02; // r
    materialAmbiant[1] = 0.02; // g
    materialAmbiant[2] = 0.02; // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.01; // r
    materialDiffuse[1] = 0.01; // g
    materialDiffuse[2] = 0.01; // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.4;  // r
    materialSpecular[1] = 0.4;  // g
    materialSpecular[2] = 0.4;  // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, 6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 2nd sphere on 4th column, cyan *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.05; // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.4;  // r
    materialDiffuse[1] = 0.5;  // g
    materialDiffuse[2] = 0.5;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.04; // r
    materialSpecular[1] = 0.7;  // g
    materialSpecular[2] = 0.7;  // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, 3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 3rd sphere on 4th column, green *****
    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.4;  // r
    materialDiffuse[1] = 0.5;  // g
    materialDiffuse[2] = 0.4;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.04; // r
    materialSpecular[1] = 0.7;  // g
    materialSpecular[2] = 0.04; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, 1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 4th sphere on 4th column, red *****
    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.5;  // r
    materialDiffuse[1] = 0.4;  // g
    materialDiffuse[2] = 0.4;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.7;  // r
    materialSpecular[1] = 0.04; // g
    materialSpecular[2] = 0.04; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, -1.2f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 5th sphere on 4th column, white *****
    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.05; // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.5;  // r
    materialDiffuse[1] = 0.5;  // g
    materialDiffuse[2] = 0.5;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.7;  // r
    materialSpecular[1] = 0.7;  // g
    materialSpecular[2] = 0.7;  // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, -3.6f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************

    // ***** 6th sphere on 4th column, yellow rubber *****
    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.0;  // b
    materialAmbiant[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

    // diffuse material
    materialDiffuse[0] = 0.5;  // r
    materialDiffuse[1] = 0.5;  // g
    materialDiffuse[2] = 0.4;  // b
    materialDiffuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    // specular material
    materialSpecular[0] = 0.7;  // r
    materialSpecular[1] = 0.7;  // g
    materialSpecular[2] = 0.04; // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    // shininess
    materialShinniness = 0.078125 * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

    // geometry
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(9.0f, -6.0f, 0.0f);
    gluSphere(quadric, 1.0f, 30, 30);
    // *******************************************************
    // *******************************************************
    // *******************************************************
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
}

void colorSetcolor(int r, int g, int b)
{
    glColor3f(r / 255, g / 255, b / 255);
}