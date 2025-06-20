/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <winbase.h>
#include <mmsystem.h>

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

/* OpenGL libraries */
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#pragma comment(lib, "winmm.lib")

#define WINWIDTH 800
#define WINHEIGHT 600
#define PI 3.14159

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
int iHeightOfWindow;
int iWidthOfWindow;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>
GLUquadric *gluQuadric;
float AnglePyramid = 0.0f;
char *soundName = "WAVE c:\\sounds\\bells.wav";
int TIMER = 0;

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
    glClearColor(0.862f, 0.862f, 0.862f, 0.0f);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

    // for draw sphere
    gluQuadric = gluNewQuadric();

    // Play Sound
    PlaySound(TEXT("Anari-Woh Chand Khila Woh.wav"), NULL, SND_FILENAME | SND_ASYNC);

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

// Global Arrays
float BodyTransalation[3] = {-3.0f, 0.0f, 0.0f};
float BodyRotation[3] = {0.0f, 1.0f, 0.0f};
float x, z;

float RightArmRotationAngle = 0.0f;

float BodyTransalation1[3] = {3.0f, 0.0f, 0.0f};

float xArmRotationAngle = 0.0f, yArmRotationAngle = 0.0f;

/* DISPLAY    */
void display(void)
{
    void drawFirstModel(void);
    void drawSecondModel(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawFirstModel();
    drawSecondModel();

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    TIMER = TIMER + 1;
    if (TIMER < 450)
    {
    }
    else
    {
        if (BodyTransalation[0] < 1.0f)

            BodyTransalation[0] = BodyTransalation[0] + 0.01f;
        else
        {
            if (RightArmRotationAngle < 50.0f)
            {
                RightArmRotationAngle = RightArmRotationAngle + 1.0f;
                if (xArmRotationAngle < 0.3f)
                {
                    xArmRotationAngle = xArmRotationAngle + 0.009f;
                    yArmRotationAngle = yArmRotationAngle + 0.009f;
                }
            }
        }
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

// GIRL
void drawFirstModel(void)
{

    // Function Prototypes
    void drawHeadSphere(void);
    void drawUpperBodyQuad(void);
    void drawDownBodySphere(void);
    void drawLeftLeg(float TranslateX);
    void drawRightLeg(float TranslateX);
    void drawLeftArm(void);
    void drawRigthArm(void);
    void drawLeftHand(void);
    void drawRightHand(void);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* DRAW MIDDLE SQUARE */
    glTranslatef(BodyTransalation[0] + 0.0f, 0.0f, (-6.0f));
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.25f, 0.25f);

    drawUpperBodyQuad();

    /* DRAW HEAD SPHERE */
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + 0.0f, 0.7f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    glColor3f(0.9647f, 0.901f, 0.850f);
    drawHeadSphere();

    // draw body sphere
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + 0.0f, -0.5f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    drawDownBodySphere();

    // draw left leg
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + 0.25f, -0.8f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.14f, 0.25f, 0.15f);

    drawRightLeg(BodyTransalation[0]);

    // draw left leg
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + -0.25f, -0.8f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.14f, 0.25f, 0.15f);

    drawLeftLeg(BodyTransalation[0]);

    // draw right arm
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + 0.4f, 0.0f, -6.0f);
    glRotatef(RightArmRotationAngle, 0.0f, 0.0f, 1.0f);
    glScalef(0.17f, 0.20f, 0.20f);

    drawRigthArm();

    // draw Right Hand
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + 0.45f + xArmRotationAngle, -0.5f + yArmRotationAngle, -6.0f);
    glRotatef(RightArmRotationAngle, 0.0f, 0.0f, 1.0f);
    glColor3f(0.9647f, 0.901f, 0.850f);
    glRotatef(0.0, 1.0f, 1.0f, 0.0f);
    glScalef(0.17f, 0.20f, 0.20f);

    drawRightHand();

    // draw left arm
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + (-0.4f), 0.0f, -6.0f);
    glRotatef(0.0f, 1.0f, 1.0f, 0.0f);
    glScalef(0.17f, 0.20f, 0.20f);

    drawLeftArm();

    // draw Left Hand
    glLoadIdentity();
    glTranslatef(BodyTransalation[0] + (-0.45f), -0.5f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.17f, 0.20f, 0.20f);

    drawLeftHand();
}

// Male
void drawSecondModel(void)
{
    // Function Prototypes
    void drawHeadSphere(void);
    void drawUpperBodyQuad(void);
    void drawDownBodySphere(void);
    void drawLeftLeg(float TranslateX);
    void drawRightLeg(float TranslateX);
    void drawLeftArm(void);
    void drawRigthArm(void);
    void drawLeftHand(void);
    void drawRightHand(void);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* DRAW MIDDLE SQUARE */
    glTranslatef(BodyTransalation1[0] + 0.0f, 0.0f, (-6.0f));
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    drawUpperBodyQuad();

    /* DRAW HEAD SPHERE */
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + 0.0f, 0.7f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    drawHeadSphere();

    // draw body sphere
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + 0.0f, -0.5f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);

    drawDownBodySphere();

    // draw left leg
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + 0.25f, -0.8f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.25f, 0.15f);

    drawRightLeg(BodyTransalation1[0]);

    // draw left leg
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + -0.25f, -0.8f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.25f, 0.15f);

    drawLeftLeg(BodyTransalation1[0]);

    // draw right arm
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + 0.5f, 0.0f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.20f, 0.20f, 0.20f);

    drawRigthArm();

    // draw Right Hand
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + 0.6f, -0.5f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.20f, 0.20f, 0.20f);

    glColor3f(0.815f, 0.686f, 0.611f);
    drawRightHand();

    // draw left arm
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + (-0.5f), 0.0f, -6.0f);
    glRotatef(RightArmRotationAngle, 0.0f, 0.0f, -1.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.20f, 0.20f, 0.20f);

    drawLeftArm();

    // draw Left Hand
    glLoadIdentity();
    glTranslatef(BodyTransalation1[0] + (-0.6f) - xArmRotationAngle, -0.5f + yArmRotationAngle, -6.0f);
    glRotatef(-RightArmRotationAngle, 0.0f, 0.0f, 1.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.20f, 0.20f, 0.20f);

    drawLeftHand();
}

void drawHeadSphere(void)
{
    glColor3f(1.0f, 1.0f, 0.0f);
    // glPolygonMode(GL_FRONT_AND_BACK,GL_POINT );
    gluSphere(gluQuadric, 1.2f, 70, 70);
}

void drawUpperBodyQuad(void)
{
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
    glVertex3f(-1.0f, 1.0f, -1.0f);
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
}

void drawDownBodySphere(void)
{
    glColor3f(1.0f, 1.0f, 0.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    gluSphere(gluQuadric, 0.8f, 70, 70);
}

void drawRightLeg(float TranslateX)
{
    glBegin(GL_TRIANGLES);

    // FRONT FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    // bottom face
    glBegin(GL_QUADS);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // bottom pryramid

    glLoadIdentity();
    glTranslatef(TranslateX + 0.25f, -1.3f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.25f, 0.15f);

    glBegin(GL_TRIANGLES);
    // FRONT FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // BACK FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // LEFT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();
}

void drawLeftLeg(float TranslateX)
{
    glBegin(GL_TRIANGLES);

    // FRONT FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    // bottom face
    glBegin(GL_QUADS);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // bottom pryramid
    glLoadIdentity();

    glTranslatef(TranslateX + (-0.25f), -1.3f, -6.0f);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.25f, 0.15f);

    // FRONT FACE
    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // RIGHT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // BACK FACE
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // LEFT FACE
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();
}

void drawLeftArm(void)
{
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
    glVertex3f(-1.0f, 1.0f, -1.0f);
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
}
void drawLeftHand(void)
{
    float x, y, z;
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(5);
    glBegin(GL_LINES);
    for (y = 1.0f; y > -1.0f; y = y - 0.01f)
    {
        for (float angle = 0.0f; angle < PI * 2; angle = angle = angle + 0.01f)
        {
            x = 0.5f * cos(angle);
            z = 0.5f * sin(angle);
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}

void drawRigthArm(void)
{
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
    glVertex3f(-1.0f, 1.0f, -1.0f);
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
}

void drawRightHand(void)
{
    float x, y, z;
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(5);
    glBegin(GL_LINES);
    for (y = 1.0f; y > -1.0f; y = y - 0.01f)
    {
        for (float angle = 0.0f; angle < PI * 2; angle = angle = angle + 0.01f)
        {
            glVertex3f(0.0f, y, 0.0f);
            x = 0.5f * cos(angle);
            z = 0.5f * sin(angle);
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}
