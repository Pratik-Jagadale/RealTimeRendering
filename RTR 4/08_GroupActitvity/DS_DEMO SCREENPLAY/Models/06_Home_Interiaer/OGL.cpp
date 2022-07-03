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

BOOL bLight = FALSE;
GLfloat lightAmbinatZero[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDefuseZero[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightSpecularZero[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPositionZero[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat lightAmbinatOne[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDefuseOne[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat lightSpecularOne[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat lightPositionOne[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat lightAmbinatTwo[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDefuseTwo[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat lightSpecularTwo[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat lightPositionTwo[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDefuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.f, 1.0f, 1.0f};
GLfloat materialShininess = 120.0f;

GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 0.0f;
GLfloat lightAngleZero = 0.0f;

float AngleCube = 90.0f;
GLfloat radius = 8.0f;
float yEyeVector = 0;

GLuint texture_home_BricksWallpaper;
GLuint texture_home_WhiteWallpaper;
GLuint texture_home_Roof;
GLuint texture_home_window;
GLuint texture_home_door;

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
    else if (iRetVal == -5)
    {
        fprintf(gpFile, "LoadGLTexture() Failed...\n");
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

        case 'W':
        case 'w':
            radius = radius - 1.0f;

            break;

        case 'S':
        case 's':
            radius = radius + 1.0f;
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
            yEyeVector = yEyeVector + 1.0f;
            break;

        case 'E':
        case 'e':
            yEyeVector = yEyeVector - 1.0f;
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
    BOOL LoadGLTexture(GLuint * texture, TCHAR ImageResourceID[]);

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

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbinatZero);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDefuseZero);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularZero);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbinatOne);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDefuseOne);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecularOne);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPositionOne);

    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbinatTwo);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDefuseTwo);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecularTwo);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPositionTwo);

    glEnable(GL_LIGHT2);

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if (LoadGLTexture(&texture_home_BricksWallpaper, MAKEINTRESOURCE(IDBITMAP_BRICKSWALLPAPER)) == FALSE)
        return -5; // write log in wndproc

    if (LoadGLTexture(&texture_home_WhiteWallpaper, MAKEINTRESOURCE(IDBITMAP_WHITEWALLPAPER)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_home_Roof, MAKEINTRESOURCE(IDBITMAP_ROOFWALLPAPER)) == FALSE)
        return -7; // write log in wndproc

    if (LoadGLTexture(&texture_home_door, MAKEINTRESOURCE(IDBITMAP_HOME_DOORTEXTURE)) == FALSE)
        return -7; // write log in wndproc

    if (LoadGLTexture(&texture_home_window, MAKEINTRESOURCE(IDBITMAP_HOME_WINDOWTEXTURE)) == FALSE)
        return -7; // write log in wndproc

    glEnable(GL_TEXTURE_2D);

    /* Clear the  screen using black color */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // quadric intialliza
    // create quadric.
    quadric = gluNewQuadric();

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
    void drawRoom(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float angle = AngleCube * M_PI / 180.0f;

    gluLookAt(radius * cos(angle), yEyeVector, radius * sin(angle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawRoom();
    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    lightAngleZero = lightAngleZero + 0.1f;
    if (lightAngleZero > 360.0f)
        lightAngleZero = lightAngleZero - 360.0f;

    lightAngleOne = lightAngleOne + 0.2f;
    if (lightAngleOne > 360.0f)
        lightAngleOne = lightAngleOne - 360.0f;

    lightAngleTwo = lightAngleTwo + 0.2f;
    if (lightAngleTwo > 360.0f)
        lightAngleTwo = lightAngleTwo - 360.0f;
}

void uninitialize(void)
{
    /* function declarations */
    void ToggleFullScreen(void);

    /* code */
    if (texture_home_door)
    {
        glDeleteTextures(1, &texture_home_door);
        texture_home_door = 0;
    }

    if (texture_home_window)
    {
        glDeleteTextures(1, &texture_home_window);
        texture_home_window = 0;
    }

    if (texture_home_BricksWallpaper)
    {
        glDeleteTextures(1, &texture_home_BricksWallpaper);
        texture_home_BricksWallpaper = 0;
    }

    if (texture_home_WhiteWallpaper)
    {
        glDeleteTextures(1, &texture_home_WhiteWallpaper);
        texture_home_WhiteWallpaper = 0;
    }

    if (texture_home_Roof)
    {
        glDeleteTextures(1, &texture_home_Roof);
        texture_home_Roof = 0;
    }

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

void drawRoom(void)
{
    // funnction declartions
    void drawHome(void);

    // code
    glScalef(8.0f, 4.0f, 6.0f);
    drawHome();
}

void colorSetcolor(int r, int g, int b)
{
    glColor3f(r / 255, g / 255, b / 255);
}

void drawHome(void)
{

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    // FRONT FACE
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPushMatrix();
    // window Right
    glBindTexture(GL_TEXTURE_2D, texture_home_window);
    glTranslatef(0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.0001f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // window Left
    glTranslatef(-0.65f, 0.0f, 0.001f);
    glScalef(0.15f, 0.4f, 1.00001f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // Dooor
    glBindTexture(GL_TEXTURE_2D, texture_home_door);
    glTranslatef(0.0f, -0.2f, 0.001f);
    glScalef(0.2f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);

    // RIGHT FACE
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // RIGHT FACE Traigle
    glPushMatrix();

    glTranslatef(1.0001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    // LEFT FACE Traigle
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1.001f, 1.2f, 0.0);
    glScalef(1.0f, 0.25f, 1.1f);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    // BACK FACE
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // LEFT FACE
    glBindTexture(GL_TEXTURE_2D, texture_home_BricksWallpaper);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    // TOP
    // front side
    glPushMatrix();

    glTranslatef(0.0f, 0.19f, 0.45f);
    glScalef(1.0f, 1.0f, 0.8f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    // back side
    glTranslatef(0.0f, 0.19f, -0.45f);
    glScalef(1.0f, 1.0f, 0.8f);
    glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_home_Roof);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, texture_home_WhiteWallpaper);
    // BOTTOM FACE
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
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
