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

GLuint texture_GaneshWallpaper;

GLfloat gfLightAmbiant[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightDeffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gflightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightPositions[] = {0.0f, 5.0f, -6.0f, 0.0f};
GLfloat gfLightSpotLightDirection[] = {0.0f, -1.0f, 0.0f, 1.0};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDefuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.f, 1.0f, 1.0f};
GLfloat materialShininess = 120.0f;

BOOL bLight = FALSE;
GLfloat angle1 = 70.0f;
GLfloat angle2 = 10.0f;

float AngleCube = 90.0f;
GLfloat radius = 8.0f;
float yEyeVector = 0;

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
        MessageBox(NULL, TEXT("Creation of Log File Faile..!!! Exiting...\n"), TEXT("File I/O Error"), MB_OK);
        exit(0);
    }
    else
    {
        fprintf(gpFile, "Log File is Successfuly Created\n");
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
        fprintf(gpFile, "LoadGLTexture() failed...\n");
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
    BOOL LoadGLTexture(GLuint *, TCHAR[]);

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

    // Ligth related

    glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbiant);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDeffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, gfLightPositions);
    glLightfv(GL_LIGHT1, GL_SPECULAR, gflightSpecular);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, gfLightSpotLightDirection);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angle1);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, angle2);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    glEnable(GL_LIGHT1);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /* Clear the  screen using black color */
    glClearColor(0.25f, 0.25f, 0.25f, 0.0f);

    if (LoadGLTexture(&texture_GaneshWallpaper, MAKEINTRESOURCE(IDBITMAP_GANESHWALLPAPER)) == FALSE)
        return -5; // write log in wndproc

    // Enabaling the texture
    glEnable(GL_TEXTURE_2D);

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
    void DrawBox(void);

    void drawGaneshPoster(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float angle;

    angle = AngleCube * M_PI / 180.0f;
    glLightfv(GL_LIGHT1, GL_POSITION, gfLightPositions);

    gluLookAt(radius * cos(angle), yEyeVector, radius * sin(angle),
              0.0f, 0.0f, -6.0f,
              0.0f, 1.0f, 0.0f);

    //    gluLookAt(0.0f, 1.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(0.0f, 0.0f, -6.0f);

    // Table
    drawGaneshPoster();

    SwapBuffers(ghdc);
}

void drawGaneshPoster(void)
{
    // variable declartions
    void DrawBox(void);

    // code
    glPushMatrix();
    // First column
    materialDefuse[0] = 1.0f; // r
    materialDefuse[1] = 1.0f; // g
    materialDefuse[2] = 1.0f; // b
    materialDefuse[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDefuse);

    materialSpecular[0] = 1.0;  // r
    materialSpecular[1] = 1.0;  // g
    materialSpecular[2] = 1.0;  // b
    materialSpecular[3] = 1.0f; // a
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    // RamSita Frame
    glScalef(2.0f, 2.0f, 0.05f);
    glBindTexture(GL_TEXTURE_2D, texture_GaneshWallpaper);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
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

    if (texture_GaneshWallpaper)
    {
        glDeleteTextures(1, &texture_GaneshWallpaper);
    }
}

void colorSetcolor(int r, int g, int b)
{
    glColor3f(r / 255, g / 255, b / 255);
}

void DrawBox(void)
{
    glBegin(GL_QUADS);
    // FRONT FACE
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // RIGHT FACE
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // BACK FACE
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // LEFT FACE
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // TOP
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // BOTTOM FACE

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void drawQuad(void)
{
    glBegin(GL_QUADS);
    // FRONT FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();
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
        fprintf(gpFile, "1\n");
        bResult = TRUE;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        fprintf(gpFile, "2\n");
        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);
        fprintf(gpFile, "3\n");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        fprintf(gpFile, "4\n");

        // create the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
        fprintf(gpFile, "5\n");

        // DELETE Object
        DeleteObject(hBitmap);
        fprintf(gpFile, "6\n");
    }
    return bResult;
}
