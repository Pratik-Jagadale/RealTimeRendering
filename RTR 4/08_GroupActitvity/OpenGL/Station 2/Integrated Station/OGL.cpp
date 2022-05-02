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
#define PI 3.1415
#define ID_STATION_SCENE_TIMER 221

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
int iHeightOfScreen;
int iWidthOfscreen;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>

// Texture
GLuint texture_sky;
GLuint texture_road;
GLuint texture_bricks;
GLuint texture_b_bricks;

// Station
int stationScene = 4;
float tapriScalex = 0.5f;
float tapriTranslatex = 6.5f;
float tapriTranslatey = -2.5f;
float tapriScaley = 0.5f;
// Chaha Tapri Global Variables
BOOL bGoBig = TRUE;

float fScaleX = 0.10f, fScaleY = 0.10f, fScaleZ = 0.10f;

float fScalex = 0.06f, fScaley = 0.06f, fScalez = 0.06f;

BOOL goVapour = TRUE;

float fVx = 0.0f;
float fVy = 0.0f;

// Chaha Cup - Madam
float gfAlphaBlend = 0.0f;
BOOL gbIfTransparent = TRUE;
// -----------------------------------------

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;
// void setColor(float r, float g, float b);

//------------------------CHAHA TAPRI-------------------------------
void drawStation(void);

//------------------------CHAHA TAPRI-------------------------------

void DrawFlame(void);
void Wheel(void);
void CookingPot(void);
void DrawFlameThree(void);
void TeaKettle(void);
void setColor(GLfloat, GLfloat, GLfloat);
void drawChai();
void Vapour(void);
void DrawTapri_PartOne(void);
//------------------------RTR BOARD-------------------------------
void DrawWhiteRectangle(void);
void BackCircle(void);
void FrontCircle(void);
void Rect(void);
void Text_R1(void);
void Text_R2(void);
void Text_T(void);

//---------------------------------------------------------------------------

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    /* function declartions */
    int initialize(void);
    void uninitialize(void);
    void display(void);
    void update(void);
    void uninitialize(void);
    void ToggleFullScreen();

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
    ToggleFullScreen();
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
    case WM_CREATE:
        SetTimer(hwnd, ID_STATION_SCENE_TIMER, 5000, NULL);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case ID_STATION_SCENE_TIMER:
            if (stationScene < 4)
                stationScene++;
            break;
        }
        break;
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

            // ShowCursor(FALSE);
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
    glEnable(GL_BLEND | GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Loading the texture
    if (LoadGLTexture(&texture_sky, MAKEINTRESOURCE(IDBITMAP_SKY)) == FALSE)
    {
        return -5;
    }
    if (LoadGLTexture(&texture_road, MAKEINTRESOURCE(IDBITMAP_ROAD)) == FALSE)
    {
        return -5;
    }
    if (LoadGLTexture(&texture_bricks, MAKEINTRESOURCE(IDBITMAP_BRICKS)) == FALSE)
    {
        return -5;
    }
    if (LoadGLTexture(&texture_b_bricks, MAKEINTRESOURCE(IDBITMAP_BRICKS)) == FALSE)
    {
        return -5;
    }
    // Enabling the texture
    glEnable(GL_TEXTURE_2D);

    resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

    return (0);
}

BOOL LoadGLTexture(GLuint *texture, TCHAR imageResourceId[])
{
    // variable declaration
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    BOOL bResult = FALSE;

    // Code
    // Load the image
    hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hBitmap)
    {
        // get image data from the handle
        bResult = TRUE;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        // Pixel Packing & Unpacking
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        // Generate Texture
        glGenTextures(1, texture);

        // Bind
        glBindTexture(GL_TEXTURE_2D, *texture);

        // texture Parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Craete the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
        DeleteObject(hBitmap);
    }

    return bResult;
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

    float cameraScale = 2.5f;
    // Background Texture
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glBindTexture(GL_TEXTURE_2D, texture_sky);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0.8);
        glVertex3f(-3.0f, 2.5f, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(-3.0f, -2.5f, 0.0f);
        glTexCoord2f(0.8, 0);
        glVertex3f(3.0f, -2.5f, 0.0f);
        glTexCoord2f(0.8, 0.8);
        glVertex3f(3.0f, 2.5f, 0.0f);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // // ===================================== TRAIN STATION
    {
        glLoadIdentity();
        // Edit this
        if (stationScene == 0)
        {
            cameraScale = 9.5f;

            glTranslatef(-40.0f, 5.0f, -12.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 1)
        {
            cameraScale = 9.5f;

            glTranslatef(-38.0f, 5.0f, -12.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 2)
        {
            cameraScale = 3.5f;

            glTranslatef(-21.0f, 7.0f, -12.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 3)
        {
            cameraScale = 10.5f;

            glTranslatef(19.5f, 5.0f, -12.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 4)
        {
            glTranslatef(0.0f, 0.0f, -12.0f);
        }

        glPushMatrix();
        drawStation();
        glPopMatrix();
    }
    // ================================== Railway Board
    {
        glLoadIdentity();
        if (stationScene == 3)
            glTranslatef(0.0f, 0.0f, -2.5f);
        else if (stationScene == 4)
            glTranslatef(-6.2f, -1.2f, -11.5f);
        DrawWhiteRectangle();

        BackCircle();
        FrontCircle();

        if (stationScene == 3)
            glTranslatef(0.0f, 0.0f, -9.0f);
        else if (stationScene == 4)
            glTranslatef(-18.8f, -3.5f, -35.0f);

        Rect();

        if (stationScene == 3)
            glTranslatef(-2.0f, 0.0f, -15.0f);
        else if (stationScene == 4)
            glTranslatef(-31.0f, -5.1f, -53.0f);
        Text_R1();

        if (stationScene == 3)
            glTranslatef(2.0f, 0.0f, 0.0f);
        else if (stationScene == 4)
            glTranslatef(+2.5f, -0.1f, 0.0f);
        Text_T();

        if (stationScene == 3)
            glTranslatef(2.0f, 0.0f, 0.0f);
        else if (stationScene == 4)
            glTranslatef(+2.5f, 0.0f, 0.0f);
        Text_R2();
    }
    // ===================================== CHAHA TAPRI
    {

        glLoadIdentity();
        if (stationScene == 0)
        {
            glTranslatef(-33.8f, 8.5f, 0.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 1)
        {
            glTranslatef(-26.8f, 8.5f, 0.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 2)
        {
            glTranslatef(-10.9f, 3.5f, 0.0f);
            glScalef(cameraScale, cameraScale, 1.0f);
        }
        else if (stationScene == 3)
        {
            glTranslatef(17.0f, 5.0f, -12.0f);
        }
        else if (stationScene == 4)
        {
            // No Translations
        }
        // Edit this
        glPushMatrix();

        glScalef(tapriScalex, tapriScaley, 1.0f);
        glTranslatef(tapriTranslatex, tapriTranslatey, 0.0);
        glTranslatef(0.7f, 0.4f, -6.0f);
        glScalef(fScaleX, fScaleY, fScaleZ);
        DrawFlameThree(); // FLAME
        // CookingPot(); // COOKING POT
        // Vapour();

        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();
        if (stationScene == 0)
        {
            glTranslatef(tapriTranslatex - 2.85, tapriTranslatey + 1.52, 0.0 - 12.0f);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
        }
        else if (stationScene == 1)
        {
            glTranslatef(tapriTranslatex + 2.85, tapriTranslatey + 1.52, 0.0 - 12.0f);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
        }
        else if (stationScene == 2)
        {
            glTranslatef(tapriTranslatex - 2.18, tapriTranslatey + 1.6, 0.0 - 15.0f);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
        }
        else if (stationScene == 4)
        {
            glTranslatef(tapriTranslatex + 1.3, tapriTranslatey + 0.45, 0.0 - 13.0f);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
        }
        CookingPot(); // COOKING POT
        Vapour();

        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(tapriTranslatex - 3.15f, tapriTranslatey + 1.28f, 0.0);
        glScalef(tapriScalex, tapriScaley, 1.0f);

        glScalef(0.2f, 0.4f, 1.0f);
        glTranslatef(-1.7f, 1.27f, -6.0f);
        TeaKettle();

        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(tapriTranslatex - 3.2f, tapriTranslatey + 1.25f, 0.0);

        glScalef(tapriScalex, tapriScaley, 1.0f);

        glTranslatef(0.0f, 0.0f, -6.0f); // -Ve to show it

        DrawTapri_PartOne(); // TAPRI ONE

        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(tapriTranslatex - 3.2f, tapriTranslatey + 1.25f, 0.0);

        glScalef(tapriScalex, tapriScaley, 1.0f);

        glTranslatef(-0.75f, -0.6f, -6.0f);
        Wheel();

        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();

        if (stationScene == 1)
        {
            // glTranslatef(tapriTranslatex - 6.5f, tapriTranslatey - 2.5, 0.0);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
            glTranslatef(8.6f, -5.58f, -38.0f);
        }
        else if (stationScene == 2)
        {
            // glTranslatef(tapriTranslatex - 6.5f, tapriTranslatey - 2.5, 0.0);
            glScalef(tapriScalex - 0.2, tapriScaley - 0.2, 1.0f);
            glTranslatef(5.0f, -3.5f, -38.0f);
        }
        else if (stationScene == 4)
        {
            glTranslatef(tapriTranslatex + 18.0, tapriTranslatey - 6.8f, 0.0);
            glScalef(tapriScalex, tapriScaley, 1.0f);
            glTranslatef(-6.5f, 3.8f, -45.0f);
        }
        drawChai();
        glPopMatrix();
        glPopMatrix();
    }

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    // ========================================== Chai Tapri
    float cupSteamTransparentSpeed = 0.004f;
    if (gbIfTransparent == TRUE)
    {
        if (gfAlphaBlend <= 0.3f)
        {
            gfAlphaBlend = gfAlphaBlend + cupSteamTransparentSpeed;
        }
        else
            gbIfTransparent = FALSE;
    }
    else
    {
        if (gfAlphaBlend >= 0.0f)
        {
            gfAlphaBlend = gfAlphaBlend - cupSteamTransparentSpeed;
        }
        else
            gbIfTransparent = TRUE;
    }
    if (bGoBig == TRUE)
    {
        if (fScaleX <= 0.10f)
        {
            fScaleX += 0.005f;
            fScaleY += 0.005f;
            fScaleZ += 0.005f;
        }
        else
        {
            bGoBig = FALSE;
        }
    }
    else
    {
        if (fScaleX >= 0.06)
        {
            fScaleX -= 0.005f;
            fScaleY -= 0.005f;
            fScaleZ -= 0.005f;
        }
        else
        {
            bGoBig = TRUE;
        }
    }

    if (goVapour == TRUE)
    {
        if (fVy <= 0.5f)
        {
            fVy += 0.02f;
        }
        else
        {
            fVy = 0.0f;
        }
    }

    // ==========================================
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

// STATION CODE ROUTINES

void drawStation(void)
{
    // variable declarations
    float SCALERATION = -0.25f;

    void drawPiller(void);
    void drawWall(void);
    void drawBigDoor(void);
    void drawRAILWAYSTATION(void);
    void drawBigGlass(void);
    void Garden(void);
    void drawCompund(void);

    // code

    glBindTexture(GL_TEXTURE_2D, texture_road);
    glBegin(GL_POLYGON);
    setColor(211, 211, 211);
    glTexCoord2f(0, 0);
    glVertex3f(-20.0f, -4.0f, 2.5f);
    glTexCoord2f(0, 1);
    glVertex3f(20.0f, -4.0f, 2.5f);
    setColor(211, 211, 211);
    // setColor(47, 79, 79);
    glTexCoord2f(1, 1);
    glVertex3f(20.0f, -4.0f, -12.0f);
    glTexCoord2f(0, 1);
    glVertex3f(-20.0f, -4.0f, -12.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // big glass
    // glLoadIdentity();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(2.5f, 1.3f, 1.0f);

    drawBigGlass();

    glPopMatrix();
    glPushMatrix();

    // glLoadIdentity();
    // glTranslatef(0.0f, 0.0f, -12.0f);
    glScalef(1.0f + SCALERATION, 1.0f + SCALERATION, 1.0f + SCALERATION);
    // background wall
    drawWall();

    glPopMatrix();
    glPushMatrix();

    // glLoadIdentity();
    glTranslatef(-8.6, -1.0f, 0.0f);
    glScalef(0.75f, 0.80f + SCALERATION, 0.75f + SCALERATION);
    // small pillers
    for (int i = 0; i <= 10; i++)
    {
        glTranslatef(2.0f, 0.0f, 0.0f);
        drawPiller();
    }

    glPopMatrix();
    glPushMatrix();

    // RAILWAY STATION
    // glLoadIdentity();
    glScalef(0.46f + SCALERATION, 0.40f + SCALERATION, 1.0f + SCALERATION);

    glTranslatef(-17.5f, 12.0f, 0.0f);
    glLineWidth(5);

    drawRAILWAYSTATION();

    // garden
    for (float k = 0.0f; k < 2.5f; k = k + 0.5f)
    {
        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();

        glTranslatef(-4.8f, -3.0f, -1.5f - k);

        glScalef(0.45f, 0.45f, 0.0f);

        Garden();
    }

    // garden
    for (float k = 0.0f; k < 2.5f; k = k + 0.5f)
    {
        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();

        glTranslatef(-4.8f, -3.0f, -1.5f - k);

        glScalef(0.45f, 0.45f, 0.0f);

        Garden();
    }

    // garden
    for (float k = 0.0f; k < 2.5f; k = k + 0.5f)
    {
        // glLoadIdentity();
        glPopMatrix();
        glPushMatrix();

        glTranslatef(-4.8f, -3.45f, -1.5f - k);

        glScalef(0.45f, 0.45f, 0.0f);

        Garden();
    }

    // doors
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-6.5f, 0.0f, 0.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);

    // draw left wall
    glTranslatef(-1.5f, -1.5f, 0.0f);
    glBegin(GL_POLYGON);
    setColor(51, 29, 12);
    glVertex3f(0.7f, 0.5f, -1.0f);
    glVertex3f(-1.7f, 0.5f, -1.0f);
    glVertex3f(-1.7f, -1.8f, -1.0f);
    glVertex3f(0.7f, -1.8f, -1.0f);
    glEnd();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-6.5f, 0.0f, 0.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);

    drawBigDoor();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(11.5f, 0.0f, 0.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);

    // draw left wall
    glTranslatef(-1.5f, -1.5f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture_b_bricks);

    glBegin(GL_POLYGON);
    // setColor(51, 29, 12);
    // setColor(0, 0, 255);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.7f, 0.5f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.7f, 0.5f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.7f, -1.8f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, -1.8f, -1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(6.5f, 0.0f, 0.0f);
    glScalef(1.4f + SCALERATION, 1.4f + SCALERATION, 1.4f + SCALERATION);
    // door

    drawBigDoor();

    // draw right walls

    // compound
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-4.2f, -2.6f, 0.0f);
    glScalef(0.45f, 0.45f, 0.0f);

    drawCompund();
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

    setColor(102, 58, 32);
    glVertex3f(1.8f, 2.04f, 0.0f);
    glVertex3f(-1.8f, 2.0f, 0.0f);

    glVertex3f(1.8f, 1.6f, 0.0f);
    glVertex3f(-1.8f, 1.6f, 0.0f);
    glEnd();

    // bottom

    glBegin(GL_POLYGON);
    setColor(51, 29, 12);
    glVertex3f(-1.7f, 1.5f, 0.0f);
    glVertex3f(-1.7f, -3.0f, 0.0f);
    glVertex3f(-1.3f, -3.0f, -2.0f);
    glVertex3f(-1.3f, 1.5f, -2.0f);

    glVertex3f(1.7f, 1.5f, 0.0f);
    glVertex3f(1.3f, 1.5f, -2.0f);
    glVertex3f(1.3f, -3.0f, -2.0f);
    glVertex3f(1.7f, -3.0f, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture_bricks);

    glBegin(GL_POLYGON);
    setColor(102, 58, 23);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.7f, 1.5f, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.7f, 1.5f, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.7f, -3.0f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.7f, -3.0f, 0.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    setColor(102, 58, 23);
    //

    // bottom(INNER) QUAD
    glBegin(GL_POLYGON);
    //    setColor(164.0f, 96.0f, 65.0f);
    setColor(60, 29, 12);
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

void drawCompund(void)
{
    glLineWidth(1);
    for (int i = 0; i < 10; i++)
    {

        glBegin(GL_POLYGON);
        setColor(255, 222, 173);
        glVertex3f(0.0f, 2.0f, 0.0f);
        glVertex3f(-0.5f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -2.0f, 0.0f);
        glVertex3f(0.5f, -2.0f, 0.0f);
        glVertex3f(0.5f, 1.0f, 0.0f);
        glVertex3f(0.0f, 2.0f, 0.0f);
        glEnd();

        glBegin(GL_LINE_LOOP);
        setColor(77, 43, 17);
        glVertex3f(0.0f, 2.0f, 0.0f);
        glVertex3f(-0.5f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -2.0f, 0.0f);
        glVertex3f(0.5f, -2.0f, 0.0f);
        glVertex3f(0.5f, 1.0f, 0.0f);
        glVertex3f(0.0f, 2.0f, 0.0f);
        glEnd();

        // Horizontal TOP Bar
        glBegin(GL_POLYGON);
        setColor(255, 222, 173);

        glVertex3f(2.0f, 0.8f, 0.0f);
        glVertex3f(-2.0f, 0.8f, 0.0f);
        glVertex3f(-2.0f, 0.5f, 0.0f);
        glVertex3f(2.0f, 0.5f, 0.0f);

        glEnd();

        glBegin(GL_LINE_LOOP);
        setColor(77, 43, 17);
        glVertex3f(2.0f, 0.8f, 0.0f);
        glVertex3f(-2.0f, 0.8f, 0.0f);
        glVertex3f(-2.0f, 0.5f, 0.0f);
        glVertex3f(2.0f, 0.5f, 0.0f);
        glEnd();

        // Horizontal BOTTOM BAR
        glBegin(GL_POLYGON);
        setColor(255, 222, 173);

        glVertex3f(2.0f, -0.5f, 0.0f);
        glVertex3f(-2.0f, -0.5f, 0.0f);
        glVertex3f(-2.0f, -0.8f, 0.0f);
        glVertex3f(2.0f, -0.8f, 0.0f);

        glEnd();

        glBegin(GL_LINE_LOOP);
        setColor(77, 43, 17);
        glVertex3f(2.0f, -0.5f, 0.0f);
        glVertex3f(-2.0f, -0.5f, 0.0f);
        glVertex3f(-2.0f, -0.8f, 0.0f);
        glVertex3f(2.0f, -0.8f, 0.0f);
        glEnd();

        glTranslatef(2.0f, 0.0f, 0.0f);
    }
}

void Garden(void)
{

    void grass1(void);
    void grass2(void);

    glPushMatrix();
    for (int i = 0; i < 32; i++)
    {
        glTranslatef(0.5, 0.0f, 0.0f);
        grass1();
    }

    glPopMatrix();
    // glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 32; i++)
    {
        glTranslatef(0.7, 0.0f, 0.0f);
        grass2();
    }
    // glPopMatrix();
}

void drawRAILWAYSTATION(void)
{

    // R
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    glVertex3f(-1.0f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);
    glEnd();

    // A
    glTranslatef(2.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    glEnd();

    glTranslatef(2.0f, 0.0f, 0.0f);
    // I
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-0.5f, 1.0f, 0.0f);
    glVertex3f(0.5f, 1.0f, 0.0f);

    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);

    glEnd();

    // L
    glTranslatef(1.5f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

    // W
    glTranslatef(2.5f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);

    glVertex3f(-1.5f, 1.0f, 0.0f);
    glVertex3f(-0.7f, -1.0f, 0.0f);

    glVertex3f(-0.7f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.7f, -1.0f, 0.0f);

    glVertex3f(0.7f, -1.0f, 0.0f);
    glVertex3f(1.5f, 1.0f, 0.0f);
    glEnd();

    // A
    glTranslatef(2.5f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    glEnd();

    // Y
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);

    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.2f, 0.0f);

    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(-0.7f, 1.0f, 0.0f);

    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(0.7f, 1.0f, 0.0f);

    glEnd();

    // S
    glTranslatef(4.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);

    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();

    // T
    glTranslatef(2.5f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glEnd();

    // A
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    glEnd();

    // T
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glEnd();

    // I
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-0.5f, 1.0f, 0.0f);
    glVertex3f(0.5f, 1.0f, 0.0f);

    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);

    glEnd();

    // O
    glTranslatef(2.0f, 0.0f, 0.0f);
    float x;
    float y;

    glPointSize(4);

    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 1.0f);
    for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
    {

        x = 0.9f * cos(angle);
        y = 1.2f * sin(angle);

        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // N
    glTranslatef(2.5f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 1.0f);

    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glEnd();
}

void drawBigGlass(void)
{
    // code
    float x, y;

    // border
    glEnable(GL_POINT_SMOOTH);
    glPointSize(15);
    glBegin(GL_POINTS);
    glColor3f(0.349f, 0.211f, 0.180f);
    for (float angle = 0.0f; angle <= 180.0; angle = angle + 0.1f)
    {
        float rangle = (angle * M_PI) / 180.0f;
        x = 2.01f * cos(rangle);
        y = 2.01f * sin(rangle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    // glass
    glBegin(GL_LINES);

    for (float angle = 0.0f; angle <= 180.0f; angle = angle + 0.008f)
    {

        float rangle = (angle * M_PI) / 180.0f;
        x = 2.0f * cos(rangle);
        y = 2.0f * sin(rangle);
        glColor3f(0.015f, 0.850f, 0.850f);
        glVertex3f(0.0f, 0.0f, 0.0f);

        glColor3f(0.835f, 0.898f, 0.949f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // horizontal lines
    glLineWidth(2);
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
    glBegin(GL_LINES);
    glColor3f(0.349f, 0.211f, 0.180f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 0.0f, 0.0f);
    glEnd();
}

void grass1(void)
{
    // grass types 1
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.6989f, -0.9219f, 0.0f);
    glVertex3f(-1.6699f, -0.8391f, 0.0f);
    glVertex3f(-1.655f, -0.7827f, 0.0f);
    glVertex3f(-1.6448f, -0.7403f, 0.0f);
    glVertex3f(-1.6333f, -0.68965f, 0.0f);
    glVertex3f(-1.6211f, -0.638f, 0.0f);
    glVertex3f(-1.6135f, -0.5859f, 0.0f);
    glVertex3f(-1.6078f, -0.5452f, 0.0f);
    glVertex3f(-1.60285f, -0.505f, 0.0f);
    glVertex3f(-1.5993f, -0.45294f, 0.0f);
    glVertex3f(-1.59804f, -0.4144f, 0.0f);
    glVertex3f(-1.5972f, -0.3714f, 0.0f);
    glVertex3f(-1.60184f, -0.3097f, 0.0f);
    glVertex3f(-1.6095f, -0.2608f, 0.0f);
    glVertex3f(-1.6199f, -0.2206f, 0.0f);
    glVertex3f(-1.6348f, -0.1854f, 0.0f);
    glVertex3f(-1.65375f, -0.1472f, 0.0f);
    glVertex3f(-1.6737f, -0.11736f, 0.0f);
    glVertex3f(-1.6972f, -0.09146f, 0.0f);
    glVertex3f(-1.7145f, -0.0784f, 0.0f);
    glVertex3f(-1.7424f, -0.0601f, 0.0f);
    glVertex3f(-1.7887f, -0.0396f, 0.0f);
    glVertex3f(-1.7408f, -0.0505f, 0.0f);
    glVertex3f(-1.6947f, -0.0657f, 0.0f);
    glVertex3f(-1.6521f, -0.0842f, 0.0f);
    glVertex3f(-1.6148f, -0.1018f, 0.0f);
    glVertex3f(-1.5766f, -0.1238f, 0.0f);
    glVertex3f(-1.5525f, -0.1403f, 0.0f);
    glVertex3f(-1.52685f, -0.1582f, 0.0f);
    glVertex3f(-1.5017f, -0.1788f, 0.0f);
    glVertex3f(-1.473f, -0.2033f, 0.0f);
    glVertex3f(-1.4556f, -0.2211f, 0.0f);
    glVertex3f(-1.4348f, -0.245f, 0.0f);
    glVertex3f(-1.4132f, -0.27f, 0.0f);
    glVertex3f(-1.3845f, -0.3103f, 0.0f);
    glVertex3f(-1.3647f, -0.343f, 0.0f);
    glVertex3f(-1.3466f, -0.377f, 0.0f);
    glVertex3f(-1.3297f, -0.4153f, 0.0f);
    glVertex3f(-1.31444f, -0.45285f, 0.0f);
    glVertex3f(-1.32f, -0.8866f, 0.0f);
    glEnd();
}

void grass2(void)
{
    // grass type 2
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.6989f, -0.9219f, 0.0f);
    glVertex3f(-1.6699f, -0.8391f, 0.0f);
    glVertex3f(-1.655f, -0.7827f, 0.0f);
    glVertex3f(-1.6448f, -0.7403f, 0.0f);
    glVertex3f(-1.6333f, -0.68965f, 0.0f);
    glVertex3f(-1.6211f, -0.638f, 0.0f);
    glVertex3f(-1.6135f, -0.5859f, 0.0f);
    glVertex3f(-1.6078f, -0.5452f, 0.0f);
    glVertex3f(-1.60285f, -0.505f, 0.0f);
    glVertex3f(-1.5993f, -0.45294f, 0.0f);
    glVertex3f(-1.59804f, -0.4144f, 0.0f);
    glVertex3f(-1.5972f, -0.3714f, 0.0f);
    glVertex3f(-1.60184f, -0.3097f, 0.0f);
    glVertex3f(-1.6095f, -0.2608f, 0.0f);
    glVertex3f(-1.6199f, -0.2206f, 0.0f);
    glVertex3f(-1.6348f, -0.1854f, 0.0f);
    glVertex3f(-1.65375f, -0.1472f, 0.0f);

    setColor(173, 255, 47);
    glVertex3f(-1.6737f, -0.11736f, 0.0f);
    glVertex3f(-1.6972f, -0.09146f, 0.0f);
    glVertex3f(-1.7145f, -0.0784f, 0.0f);
    glVertex3f(-1.7424f, -0.0601f, 0.0f);
    glVertex3f(-1.7887f, -0.0396f, 0.0f);
    glVertex3f(-1.7408f, -0.0505f, 0.0f);
    glVertex3f(-1.6947f, -0.0657f, 0.0f);
    glVertex3f(-1.6521f, -0.0842f, 0.0f);
    glVertex3f(-1.6148f, -0.1018f, 0.0f);
    glVertex3f(-1.5766f, -0.1238f, 0.0f);
    glVertex3f(-1.5525f, -0.1403f, 0.0f);
    glVertex3f(-1.52685f, -0.1582f, 0.0f);
    glVertex3f(-1.5017f, -0.1788f, 0.0f);
    glVertex3f(-1.473f, -0.2033f, 0.0f);
    glVertex3f(-1.4556f, -0.2211f, 0.0f);
    glVertex3f(-1.4348f, -0.245f, 0.0f);
    glVertex3f(-1.4132f, -0.27f, 0.0f);
    glVertex3f(-1.3845f, -0.3103f, 0.0f);
    glVertex3f(-1.3647f, -0.343f, 0.0f);
    setColor(50, 205, 50);
    glVertex3f(-1.3466f, -0.377f, 0.0f);
    glVertex3f(-1.3297f, -0.4153f, 0.0f);
    glVertex3f(-1.31444f, -0.45285f, 0.0f);
    glVertex3f(-1.32f, -0.8866f, 0.0f);

    //----------------------

    glVertex3f(-1.3009f, -0.407f, 0.0f);
    glVertex3f(-1.2898f, -0.3549f, 0.0f);
    glVertex3f(-1.2747f, -0.31155f, 0.0f);
    glVertex3f(-1.2692f, -0.2935f, 0.0f);
    glVertex3f(-1.2557f, -0.26044f, 0.0f);
    glVertex3f(-1.242f, -0.231f, 0.0f);
    glVertex3f(-1.2275f, -0.2044f, 0.0f);
    setColor(46, 139, 87);
    glVertex3f(-1.2204f, -0.1913f, 0.0f);
    glVertex3f(-1.20647f, -0.1701f, 0.0f);
    glVertex3f(-1.1951f, -0.15376f, 0.0f);
    glVertex3f(-1.18205f, -0.1368f, 0.0f);
    glVertex3f(-1.16746f, -0.1207f, 0.0f);
    glVertex3f(-1.1508f, -0.10266f, 0.0f);
    glVertex3f(-1.1384f, -0.091f, 0.0f);
    glVertex3f(-1.12376f, -0.07756f, 0.0f);
    glVertex3f(-1.1024f, -0.0635f, 0.0f);
    glVertex3f(-1.0851f, -0.0537f, 0.0f);
    glVertex3f(-1.0744f, -0.0483f, 0.0f);
    glVertex3f(-1.0838f, -0.0638f, 0.0f);
    glVertex3f(-1.0951f, -0.0803f, 0.0f);
    glVertex3f(-1.1068f, -0.0964f, 0.0f);
    glVertex3f(-1.1183f, -0.1143f, 0.0f);
    glVertex3f(-1.1251f, -0.1297f, 0.0f);
    glVertex3f(-1.13095f, -0.1467f, 0.0f);
    setColor(34, 139, 34);
    glVertex3f(-1.1386f, -0.1744f, 0.0f);
    glVertex3f(-1.14464f, -0.20186f, 0.0f);
    glVertex3f(-1.15f, -0.22666f, 0.0f);
    glVertex3f(-1.15374f, -0.25597f, 0.0f);
    glVertex3f(-1.1556f, -0.2801f, 0.0f);
    glVertex3f(-1.1576f, -0.30516f, 0.0f);
    glVertex3f(-1.15753f, -0.31036f, 0.0f);
    glVertex3f(-1.1372f, -0.32056f, 0.0f);
    glVertex3f(-1.1227f, -0.32635f, 0.0f);

    setColor(0, 100, 0);
    glVertex3f(-1.0656f, -0.3597f, 0.0f);
    glVertex3f(-1.0334f, -0.3813f, 0.0f);
    glVertex3f(-1.007f, -0.3977f, 0.0f);
    glVertex3f(-0.9916f, -0.4093f, 0.0f);
    glVertex3f(-0.9701f, -0.4275f, 0.0f);
    glVertex3f(-0.9505f, -0.448f, 0.0f);
    glVertex3f(-0.9314f, -0.4678f, 0.0f);
    glVertex3f(-0.9143f, -0.4894f, 0.0f);
    glVertex3f(-0.8984f, -0.51036f, 0.0f);
    glVertex3f(-0.8854f, -0.532f, 0.0f);
    glVertex3f(-0.87375f, -0.552f, 0.0f);
    glVertex3f(-0.8676f, -0.56365f, 0.0f);
    glVertex3f(-0.86115f, -0.57455f, 0.0f);

    setColor(154, 205, 50);
    glVertex3f(-0.852f, -0.5435f, 0.0f);
    glVertex3f(-0.8391f, -0.5079f, 0.0f);
    glVertex3f(-0.8185f, -0.4626f, 0.0f);
    glVertex3f(-0.8007f, -0.4276f, 0.0f);
    glVertex3f(-0.7787f, -0.3937f, 0.0f);
    glVertex3f(-0.7554f, -0.3639f, 0.0f);
    glVertex3f(-0.7332f, -0.3406f, 0.0f);
    glVertex3f(-0.6993f, -0.3106f, 0.0f);
    glVertex3f(-0.6778f, -0.2958f, 0.0f);
    glVertex3f(-0.6534f, -0.2808f, 0.0f);
    glVertex3f(-0.631f, -0.2687f, 0.0f);
    glVertex3f(-0.61426f, -0.2614f, 0.0f);
    glVertex3f(-0.5966f, -0.25525f, 0.0f);
    glVertex3f(-0.61085f, -0.264f, 0.0f);
    glVertex3f(-0.62276f, -0.2738f, 0.0f);
    glVertex3f(-0.6391f, -0.29f, 0.0f);
    glVertex3f(-0.64976f, -0.30276f, 0.0f);
    glVertex3f(-0.6608f, -0.322f, 0.0f);
    glVertex3f(-0.6711f, -0.3434f, 0.0f);
    glVertex3f(-0.67945f, -0.367f, 0.0f);
    glVertex3f(-0.6855f, -0.38673f, 0.0f);
    glVertex3f(-0.69004f, -0.4093600f, 0.0f);
    glVertex3f(-0.6943f, -0.4351f, 0.0f);
    glVertex3f(-0.6986f, -0.4604f, 0.0f);
    glVertex3f(-0.69867f, -0.4893f, 0.0f);
    glVertex3f(-0.7006f, -0.5159f, 0.0f);
    glVertex3f(-0.7011f, -0.54004f, 0.0f);
    glVertex3f(-0.6993f, -0.5689f, 0.0f);
    glVertex3f(-0.6991f, -0.5896f, 0.0f);
    glVertex3f(-0.6979f, -0.6123f, 0.0f);
    glVertex3f(-0.6965f, -0.6398f, 0.0f);
    glVertex3f(-0.6933f, -0.6659f, 0.0f);
    glVertex3f(-0.69146f, -0.649f, 0.0f);
    setColor(107, 142, 35);
    glVertex3f(-0.6911f, -0.62176f, 0.0f);
    glVertex3f(-0.6899f, -0.5962f, 0.0f);
    glVertex3f(-0.6873f, -0.5717f, 0.0f);
    glVertex3f(-0.6842f, -0.5483f, 0.0f);
    glVertex3f(-0.6778f, -0.5205f, 0.0f);
    glVertex3f(-0.67084f, -0.4899f, 0.0f);
    glVertex3f(-0.6633f, -0.4661f, 0.0f);
    glVertex3f(-0.6591f, -0.4507f, 0.0f);
    glVertex3f(-0.6462f, -0.423f, 0.0f);
    glVertex3f(-0.63245f, -0.396f, 0.0f);
    glVertex3f(-0.6181f, -0.3717f, 0.0f);
    glVertex3f(-0.6034f, -0.3472f, 0.0f);
    glVertex3f(-0.5827f, -0.31895f, 0.0f);
    glVertex3f(-0.5666f, -0.3006f, 0.0f);
    glVertex3f(-0.5564f, -0.2888f, 0.0f);

    glVertex3f(-0.5504f, -0.2797f, 0.0f);
    glVertex3f(-0.5574f, -0.255f, 0.0f);
    glVertex3f(-0.5618f, -0.2299f, 0.0f);
    glVertex3f(-0.5677f, -0.2135f, 0.0f);
    glVertex3f(-0.5754f, -0.1909f, 0.0f);
    glVertex3f(-0.5852f, -0.1691f, 0.0f);
    glVertex3f(-0.5965f, -0.1484f, 0.0f);
    glVertex3f(-0.6045f, -0.1338f, 0.0f);
    glVertex3f(-0.61825f, -0.1164f, 0.0f);
    glVertex3f(-0.6309f, -0.1023f, 0.0f);
    glVertex3f(-0.6445f, -0.0894f, 0.0f);
    glVertex3f(-0.6566f, -0.0794f, 0.0f);
    glVertex3f(-0.6717f, -0.06875f, 0.0f);
    glVertex3f(-0.689f, -0.0584f, 0.0f);
    glVertex3f(-0.7089f, -0.049f, 0.0f);
    glVertex3f(-0.7278f, -0.0408f, 0.0f);
    setColor(85, 107, 47);
    glVertex3f(-0.709f, -0.0433f, 0.0f);
    glVertex3f(-0.6938f, -0.0484f, 0.0f);
    glVertex3f(-0.672f, -0.05594f, 0.0f);
    glVertex3f(-0.655f, -0.061f, 0.0f);
    glVertex3f(-0.63605f, -0.0674f, 0.0f);
    glVertex3f(-0.6191f, -0.0733f, 0.0f);
    glVertex3f(-0.6011f, -0.0813f, 0.0f);
    glVertex3f(-0.5857f, -0.0885f, 0.0f);
    glVertex3f(-0.57195f, -0.096f, 0.0f);
    glVertex3f(-0.5559f, -0.10354f, 0.0f);
    glVertex3f(-0.5379f, -0.1132f, 0.0f);
    glVertex3f(-0.5161f, -0.12644f, 0.0f);
    glVertex3f(-0.4967f, -0.1401f, 0.0f);
    glVertex3f(-0.4736f, -0.1564f, 0.0f);
    glVertex3f(-0.455f, -0.17115f, 0.0f);
    glVertex3f(-0.4399f, -0.1828f, 0.0f);
    glVertex3f(-0.42976f, -0.19085f, 0.0f);
    glVertex3f(-0.387f, -0.2356f, 0.0f);
    glVertex3f(-0.3773f, -0.2462f, 0.0f);
    glVertex3f(-0.3595f, -0.268f, 0.0f);
    glVertex3f(-0.3494f, -0.281f, 0.0f);
    glVertex3f(-0.3374f, -0.2977f, 0.0f);
    glVertex3f(-0.3286f, -0.3115f, 0.0f);
    glVertex3f(-0.31873f, -0.32623f, 0.0f);
    glVertex3f(-0.3069f, -0.34596f, 0.0f);
    glVertex3f(-0.2986f, -0.3624f, 0.0f);
    glVertex3f(-0.2881f, -0.3819f, 0.0f);
    glVertex3f(-0.2798f, -0.4006f, 0.0f);
    glVertex3f(-0.2734f, -0.4174f, 0.0f);
    glVertex3f(-0.265f, -0.4381f, 0.0f);
    glVertex3f(-0.2584f, -0.4537f, 0.0f);
    glVertex3f(-0.2438f, -0.5148f, 0.0f);
    glVertex3f(-0.2336f, -0.6007f, 0.0f);
    glVertex3f(-0.223f, -0.6856f, 0.0f);
    glVertex3f(-0.2214f, -0.7747f, 0.0f);
    glVertex3f(-0.2192f, -0.8943f, 0.0f);

    setColor(143, 188, 139);
    glVertex3f(-0.21735f, -0.8991f, 0.0f);
    glVertex3f(-0.2592f, -0.90194f, 0.0f);
    glVertex3f(-0.2915f, -0.90336f, 0.0f);
    glVertex3f(-0.3241f, -0.90604f, 0.0f);
    glVertex3f(-0.3642f, -0.91f, 0.0f);
    glVertex3f(-0.4019f, -0.91355f, 0.0f);
    glVertex3f(-0.43196f, -0.9144f, 0.0f);
    glVertex3f(-0.4626f, -0.9196f, 0.0f);
    glVertex3f(-0.49534f, -0.92765f, 0.0f);
    glVertex3f(-0.5264f, -0.93625f, 0.0f);
    glVertex3f(-0.5657f, -0.9482f, 0.0f);
    glVertex3f(-0.5998f, -0.9554f, 0.0f);
    glVertex3f(-0.6329f, -0.95785f, 0.0f);
    glVertex3f(-0.66264f, -0.95746f, 0.0f);
    glVertex3f(-0.6906f, -0.9569f, 0.0f);
    glVertex3f(-0.7179f, -0.95185f, 0.0f);
    glVertex3f(-0.75014f, -0.94554f, 0.0f);
    glVertex3f(-0.7723f, -0.9399f, 0.0f);
    glVertex3f(-0.7994f, -0.9303f, 0.0f);
    glVertex3f(-0.8218f, -0.92305f, 0.0f);
    glVertex3f(-0.8482f, -0.91446f, 0.0f);
    glVertex3f(-0.8721f, -0.9053f, 0.0f);
    glVertex3f(-0.8855, -0.90104f, 0.0f);
    glVertex3f(-0.914f, -0.8997f, 0.0f);
    glVertex3f(-0.9433f, -0.9024f, 0.0f);
    glVertex3f(-0.962f, -0.9076f, 0.0f);
    glVertex3f(-0.9844f, -0.9145f, 0.0f);
    glVertex3f(-1.0233f, -0.9296f, 0.0f);
    glVertex3f(-1.0579f, -0.9452f, 0.0f);
    glVertex3f(-1.08675f, -0.9566f, 0.0f);
    glVertex3f(-1.1168f, -0.9642f, 0.0f);
    glVertex3f(-1.1421f, -0.96885f, 0.0f);
    glVertex3f(-1.1756f, -0.9669f, 0.0f);
    glVertex3f(-1.2033f, -0.9576f, 0.0f);
    glVertex3f(-1.23105f, -0.9411f, 0.0f);
    glVertex3f(-1.251f, -0.9265f, 0.0f);
    glVertex3f(-1.2654f, -0.9147f, 0.0f);
    glVertex3f(-1.2849f, -0.9009f, 0.0f);
    glVertex3f(-1.30485f, -0.8909f, 0.0f);
    glVertex3f(-1.32196f, -0.8874f, 0.0f);
    glVertex3f(-1.35854f, -0.8886f, 0.0f);
    glVertex3f(-1.3924f, -0.8915f, 0.0f);
    glVertex3f(-1.4211f, -0.8981f, 0.0f);
    glVertex3f(-1.4556f, -0.9061f, 0.0f);
    glVertex3f(-1.4851f, -0.9174f, 0.0f);
    glVertex3f(-1.5155f, -0.9276f, 0.0f);
    glVertex3f(-1.54025f, -0.9362f, 0.0f);
    glVertex3f(-1.5688f, -0.94306f, 0.0f);
    glVertex3f(-1.5963f, -0.94684f, 0.0f);
    glVertex3f(-1.6325f, -0.945f, 0.0f);
    glVertex3f(-1.6666f, -0.9362f, 0.0f);
    glVertex3f(-1.6974f, -0.92537f, 0.0f);

    glEnd();
}

void setColor(float r, float g, float b)
{
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

//############## CHAHA TAPRI Function Definitions #######################

// ------------------------------------------------------------------------
void DrawRoofEdges(float fX, float fY, float fZ)
{
    // local variables
    float fXStart = fX;
    float fXEnd = fX * -1.0f;
    // code

    for (float fStart = fXStart; fStart < fXEnd; fStart += 0.2f)
    {
        glBegin(GL_TRIANGLE_STRIP);
        // setColor(55, 73, 192);//Blue Patra
        setColor(255, 0, 0); // R Patra

        glVertex3f(fStart, fY, fZ);
        glVertex3f(fStart + 0.1f, fY - 0.05f, fZ);
        glVertex3f(fStart + 0.2f, fY, fZ);

        glEnd();
    }
}

//==================================
void DrawFlameTwo(void)
{
    //--------------------------0L
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(-0.3f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.0f);

    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();
    //-----------------------------------0R
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(0.121f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();
    //-----------------------1L
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(-0.21f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.2f, -0.2f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();
    //-----------------------------------1R
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(0.21f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();

    //---------------------------2L
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(-0.12f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.2f, -0.2f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();
    //-----------------------------------2R
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(0.3f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.3f, -0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();

    //---------------------------3L
    glBegin(GL_TRIANGLES);
    setColor(245.0f, 50.0f, 7.0f);

    glVertex3f(0.0f, 0.3f, 0.0f);
    setColor(255.0f, 255.0f, 0.0f);
    glVertex3f(-0.2f, -0.2f, 0.0f);
    glVertex3f(0.3f, -0.3f, 0.0f);

    glEnd();
}
// ------------------------------------------------------------------------
void DrawTapri_PartOne(void)
{
    void DrawRoofEdges(GLfloat, GLfloat, GLfloat);

    void DrawFlameTwo(void);
    void CookingPot(void);

    // code
    //--Plank 1------Main Board---------------------------------------
    glBegin(GL_QUADS);

    setColor(200, 102, 60); // Wood color1

    glVertex3f(1.4f, 0.3f, 0.0f);
    glVertex3f(-1.4f, 0.3f, 0.0f);
    glVertex3f(-1.4f, -0.3f, 0.0f);
    glVertex3f(1.4f, -0.3f, 0.0f);
    glEnd();
    //---Plank 2--------------Poll Right-------------------------------
    glBegin(GL_POLYGON);
    //	glColor3f(255.0f / 255.0f, 178.0f / 255.0f, 0.0f / 255.0f);
    setColor(149, 149, 149); // Grey Metal

    glVertex3f(1.4f, 1.5f, 0.0f);
    glVertex3f(1.30f, 1.5f, 0.0f);
    glVertex3f(1.30f, 0.3f, 0.0f);
    glVertex3f(1.4f, 0.3f, 0.0f);
    glEnd();
    //---Plank 3--------------Poll Left-------------------------------
    glBegin(GL_POLYGON);
    setColor(149, 149, 149); // Grey Metal

    glVertex3f(-1.45f, 1.5f, 0.0f);
    glVertex3f(-1.35f, 1.5f, 0.0f);
    glVertex3f(-1.35f, 0.3f, 0.0f);
    glVertex3f(-1.45f, 0.3f, 0.0f);
    glEnd();
    //----Plank 4------Black Stand--------------------------
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(1.6f, 0.35f, 0.0f);
    glVertex3f(-1.6f, 0.35f, 0.0f);
    glVertex3f(-1.6f, 0.29f, 0.0f);
    glVertex3f(1.6f, 0.29f, 0.0f);

    glEnd();
    //------------Blue Roof------------------------
    glBegin(GL_POLYGON);
    setColor(55, 73, 192); // Blue Patra

    glVertex3f(1.7f, 2.0f, 0.0f);
    glVertex3f(-1.7f, 2.0f, 0.0f);
    glVertex3f(-2.3f, 1.5f, 0.0f);
    glVertex3f(2.3f, 1.5f, 0.0f);
    glEnd();

    DrawRoofEdges(-2.3f, 1.5f, 0.0f);
    //	DrawFlame();

    // end of function
}
// ------------------------------------------------------------------------
void DrawFlameThree(void)
{
    glPushMatrix();
    glBegin(GL_POLYGON);
    setColor(255.0, 255.0f, 0.0f); // Yellow

    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.2f, -0.9f, 0.0f);
    glVertex3f(0.5f, -0.7f, 0.0f);
    glVertex3f(0.9f, -0.3f, 0.0f);

    setColor(245.0f, 50.0f, 7.0f); // Orange
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.9f, 0.0f);
    glVertex3f(0.9f, 0.5f, 0.0f);

    glVertex3f(0.6f, 1.0f, 0.0f);
    glVertex3f(0.3f, 0.5f, 0.0f);
    glVertex3f(0.2f, 1.0f, 0.0f);

    glVertex3f(0.0f, 1.3f, 0.0f); // apex
    //------------------------------------------

    glVertex3f(-0.2f, 1.0f, 0.0f);
    glVertex3f(-0.3f, 0.5f, 0.0f);
    glVertex3f(-0.6f, 1.0f, 0.0f);

    glVertex3f(-0.9f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 0.9f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);

    setColor(255.0, 255.0f, 0.0f); // Yellow

    glVertex3f(-0.9f, -0.3f, 0.0f);
    glVertex3f(-0.5f, -0.7f, 0.0f);
    glVertex3f(-0.2f, -0.9f, 0.0f);
    glVertex3f(-0.0f, -1.0f, 0.0f);

    glEnd();
    glPopMatrix();
}

// ------------------------------------------------------------------------

void CookingPot(void)
{
    glPushMatrix();
    // glTranslatef(0.5f, 0.0f, -6.0f);

    //-------Left Handle-----------
    //--------------------
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(-1.1f, 0.2f, 0.0f);
    glVertex3f(-1.15f, 0.2f, 0.0f);
    glVertex3f(-1.15f, 0.0f, 0.0f);
    glVertex3f(-1.1f, 0.0f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(-0.95f, 0.2f, 0.0f);
    glVertex3f(-1.15f, 0.2f, 0.0f);
    glVertex3f(-1.15f, 0.15f, 0.0f);
    glVertex3f(-0.95f, 0.15f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(-0.95f, 0.0f, 0.0f);
    glVertex3f(-1.15f, 0.0f, 0.0f);
    glVertex3f(-1.15f, -0.05f, 0.0f);
    glVertex3f(-0.95f, -0.05f, 0.0f);

    glEnd();

    //######## Inner ########
    glBegin(GL_QUADS);

    //	glColor3f(195.0f / 255.0f, 195.0f / 255.0f, 195.0f / 255.0f);	//Greyish
    setColor(195.0f, 195.0f, 195.0f); // Greyish

    glVertex3f(-1.1f, 0.5f, 0.0f); // 4
    glVertex3f(-0.7f, 0.4f, 0.0f); // 5

    glVertex3f(0.7f, 0.4f, 0.0f); // 5
    glVertex3f(1.1f, 0.5f, 0.0f); // 4

    glEnd();

    //###########  Cooking Pot  ##################
    glBegin(GL_POLYGON);

    glColor3f(120.0f / 255.0f, 120.0f / 255.0f, 142.0f / 255.0f); // greyish
    setColor(120.0f, 120.0f, 142.0f);                             // Greyish
    glVertex3f(0.0f, -0.5f, 0.0f);

    // glColor3f(1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);   // black
    glVertex3f(0.7f, -0.5f, 0.0f); // diff color

    setColor(120.0f, 120.0f, 142.0f); // Greyish
    glVertex3f(1.0f, -0.4f, 0.0f);

    glVertex3f(1.1f, 0.5f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);  // Black
    glVertex3f(0.7f, 0.4f, 0.0f); // diff color

    setColor(120.0f, 120.0f, 142.0f); // Greyish
    glVertex3f(0.0f, 0.4f, 0.0f);

    glVertex3f(0.0f, -0.5f, 0.0f);
    //------------------------------------------------------
    glVertex3f(0.0f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.7f, 0.4f, 0.0f); // diff color

    setColor(120.0f, 120.0f, 142.0f); // Greyish
    glVertex3f(-1.1f, 0.5f, 0.0f);
    glVertex3f(-1.0f, -0.4f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);    // black
    glVertex3f(-0.7f, -0.5f, 0.0f); // diff color

    setColor(120.0f, 120.0f, 142.0f); // Greyish
    glVertex3f(0.0f, -0.5f, 0.0f);

    glEnd();
    //###### Right Handle------- #######

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(1.05f, 0.2f, 0.0f);
    glVertex3f(1.0f, 0.2f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.05f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(1.05f, 0.2f, 0.0f);
    glVertex3f(0.95f, 0.2f, 0.0f);
    glVertex3f(0.95f, 0.15f, 0.0f);
    glVertex3f(1.05f, 0.15f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(1.05f, 0.0f, 0.0f);
    glVertex3f(0.95f, 0.0f, 0.0f);
    glVertex3f(0.95f, -0.05f, 0.0f);
    glVertex3f(1.05f, -0.05f, 0.0f);
    glEnd();
    //---------- Bottom Stand -------------

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(0.7f, -0.5f, 0.0f);
    glVertex3f(0.9f, -1.0f, 0.0f);

    glVertex3f(-0.7f, -0.5f, 0.0f);
    glVertex3f(-0.9f, -1.0f, 0.0f);

    glEnd();
    glPopMatrix();
}
//--------------------------------------------------
void Wheel(void)
{

    // variables
    static float fXPt = 0.0f;
    static float fYPt = 0.0f;

    // keep distance between Radii 0.075f
    const float frad = 0.4f;
    const float fRAD = 0.475f;

    // code
    glBegin(GL_LINES);
    for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle += 0.01f)
    {

        glColor3f(0.0f, 0.0f, 0.0f); // Black

        glVertex3f(frad * cos(fAngle * PI / 180.0f), frad * sin(fAngle * PI / 180.0f), 0.0f);

        fXPt = fRAD * cos(fAngle * PI / 180.0f);
        fYPt = fRAD * sin(fAngle * PI / 180.0f);

        glVertex3f(fXPt, fYPt, 0.0f);
    }
    glEnd();

    //------------### keep 1.5f Units of Difference between Two wheel centers #######

    glBegin(GL_LINES);
    for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle += 0.01f)
    {

        glColor3f(0.0f, 0.0f, 0.0f); // Black

        glVertex3f((frad * cos(fAngle * PI / 180.0f) + 1.5f), frad * sin(fAngle * PI / 180.0f), 0.0f);

        fXPt = fRAD * cos(fAngle * PI / 180.0f);
        fYPt = fRAD * sin(fAngle * PI / 180.0f);

        glVertex3f(fXPt + 1.5f, fYPt, 0.0f);
    }
    glEnd();
    //-----------------------------------------

    //---Spokes L and R ------------
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle += 20.0f)
    {
        setColor(100.0f, 100.0f, 100.0f); // Black

        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(frad * cos(fAngle * PI / 180.0f), frad * sin(fAngle * PI / 180.0f), 0.0f);

        glVertex3f(0.0f + 1.5f, 0.0f, 0.0f);
        glVertex3f((frad * cos(fAngle * PI / 180.0f) + 1.5f), frad * sin(fAngle * PI / 180.0f), 0.0f);
    }

    glEnd();
    //----------------

    //------------# Joining Line #------------
    glLineWidth(5.0f);
    glBegin(GL_LINES);

    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.5f, 0.0f, 0.0f);

    glEnd();

    //----------------------------
}

void Vapour(void)
{
    glPushMatrix();
    //-------##  Vapour Lines ##----------

    // local variables for Vapour
    static float fXVp1 = -0.4f;
    static float fXVp2 = -0.35f;
    float fIncr = 0.0f;

    glTranslatef(fVx, fVy, 0.0f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);

    for (int i = 0; i < 4; i++)
    {
        //		glColor3f(213.0f / 255.0f, 213.0f / 255.0f, 213.0f / 255.0f);	//Greyish
        setColor(213.0f, 213.0f, 213.0f); // Greyish

        glVertex3f(fXVp1 + fIncr, 1.0f, 0.0f);
        glVertex3f(fXVp2 + fIncr, 0.9f, 0.0f);

        glVertex3f(fXVp2 + fIncr, 0.9f, 0.0f);
        glVertex3f(fXVp1 + fIncr, 0.8f, 0.0f);

        glVertex3f(fXVp1 + fIncr, 0.8f, 0.0f);
        glVertex3f(fXVp2 + fIncr, 0.7f, 0.0f);

        glVertex3f(fXVp2 + fIncr, 0.7f, 0.0f);
        glVertex3f(fXVp1 + fIncr, 0.6f, 0.0f);

        fIncr += 0.3f;
    }
    glEnd();
    glPopMatrix();
}
//------------------
void TeaKettle(void)
{
    // variables

    // code

    //--------- # Handle # ------------

    glLineWidth(4.5f);
    glBegin(GL_LINES);

    //	glColor3f(153.0f / 255.0f, 67.0f / 255.0f, 24.0f / 255.0f);
    setColor(153.0f, 67.0f, 24.0f); // Wooden

    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.65f, 0.5f, 0.0f);

    glVertex3f(0.65f, 0.5f, 0.0f);
    glVertex3f(0.65f, 0.2f, 0.0f);

    glVertex3f(0.65f, 0.2f, 0.0f);
    glVertex3f(0.5f, 0.2f, 0.0f);

    glEnd();

    //-------------  # Body #---------------------
    glBegin(GL_POLYGON);
    //	glColor3f(131.0f / 255.0f, 131.0f / 255.0f, 131.0f / 255.0f);//Grey
    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(0.0f, -0.5f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);    // White
    glVertex3f(0.4f, -0.53f, 0.0f); // diff color - White

    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(0.6f, -0.5f, 0.0f);
    glVertex3f(0.6f, 0.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f); // White
    glVertex3f(0.5f, 0.5f, 0.0f);

    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(0.0f, 0.5f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f); // White
    glVertex3f(-0.5f, 0.5f, 0.0f);

    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(-0.6f, 0.0f, 0.0f);

    glVertex3f(-0.6f, -0.5f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);      // White
    glVertex3f(-0.4f, -0.53f, 0.0f);  // diff color - White
    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(0.0f, -0.5f, 0.0f);
    glEnd();

    //------- Mouth --------

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.8f, 0.0f);
    glVertex3f(-0.9f, 0.8f, 0.0f);
    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(-0.7f, 0.6f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.4f, 0.45f, 0.0f);

    glEnd();
    //--------------- Top  -------------------------

    glBegin(GL_QUADS);

    //	glColor3f(45.0f / 255.0f, 45.0f / 255.0f, 45.0f / 255.0f);		//BlackIsh
    setColor(45.0f, 45.0f, 45.0f); // BlackIsh

    glVertex3f(0.5f, 0.9f, 0.0f);
    glVertex3f(-0.5f, 0.9f, 0.0f);
    setColor(131.0f, 131.0f, 131.0f); // Grey
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glEnd();

    glLineWidth(4.0f);
    glBegin(GL_LINES);

    setColor(45.0f, 45.0f, 45.0f); // BlackIsh
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);

    glEnd();
}
// Chaha
void drawChai()
{
    // chai
    // glLoadIdentity();

    // glTranslatef(0.0f, 0.0f, -8.0f);
    glBegin(GL_QUADS);

    glColor3f(210.0f / 255.0f, 134.0 / 255.0f, 19.0f / 255.0f); // chai-brown
    glVertex3f(0.7f, 1.0f, 0.0f);
    glVertex3f(-0.7f, 1.0f, 0.0f);
    glVertex3f(-0.3f, -1.0f, 0.0f);
    glVertex3f(0.3f, -1.0f, 0.0f);

    glEnd();

    // Lines for steam of "chai" -- 1
    // glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glLineWidth(2.0f);
    if (stationScene == 1)
        glLineWidth(10.0f);
    else if (stationScene == 2)
        glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);

    glColor4f(1.0f, 1.0, 1.0f, gfAlphaBlend); // white
    glVertex3f(0.6f, 0.0f, 0.0f);
    glVertex3f(0.55f, 0.2f, 0.0f);
    glVertex3f(0.6f, 0.4f, 0.0f);
    glVertex3f(0.55f, 0.6f, 0.0f);
    glVertex3f(0.6f, 0.8f, 0.0f);
    glVertex3f(0.55f, 1.0f, 0.0f);

    glEnd();
    glPopMatrix();
    glPushMatrix();
    // Lines for steam of "chai" -- 2
    // glLoadIdentity();
    glTranslatef(-0.4f, 1.5f, 0.0f);

    glBegin(GL_LINE_STRIP);

    glColor4f(1.0f, 1.0, 1.0f, gfAlphaBlend); // white
    glVertex3f(0.6f, 0.0f, 0.0f);
    glVertex3f(0.55f, 0.2f, 0.0f);
    glVertex3f(0.6f, 0.4f, 0.0f);
    glVertex3f(0.55f, 0.6f, 0.0f);
    glVertex3f(0.6f, 0.8f, 0.0f);
    glVertex3f(0.55f, 1.0f, 0.0f);

    glEnd();
    // Lines for steam of "chai" -- 3
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.8f, 1.5f, 0.0f);

    glBegin(GL_LINE_STRIP);

    glColor4f(1.0f, 1.0, 1.0f, gfAlphaBlend); // white
    glVertex3f(0.6f, 0.0f, 0.0f);
    glVertex3f(0.55f, 0.2f, 0.0f);
    glVertex3f(0.6f, 0.4f, 0.0f);
    glVertex3f(0.55f, 0.6f, 0.0f);
    glVertex3f(0.6f, 0.8f, 0.0f);
    glVertex3f(0.55f, 1.0f, 0.0f);

    glEnd();

    // Lines for steam of "chai" -- 4
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-1.2f, 1.5f, 0.0f);

    glBegin(GL_LINE_STRIP);

    glColor4f(1.0f, 1.0, 1.0f, gfAlphaBlend); // white
    glVertex3f(0.6f, 0.0f, 0.0f);
    glVertex3f(0.55f, 0.2f, 0.0f);
    glVertex3f(0.6f, 0.4f, 0.0f);
    glVertex3f(0.55f, 0.6f, 0.0f);
    glVertex3f(0.6f, 0.8f, 0.0f);
    glVertex3f(0.55f, 1.0f, 0.0f);

    glEnd();
    glPopMatrix();
    glPushMatrix();
    // Chai ki pyaali
    // glLoadIdentity();

    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.4f, 1.22f, 1.0f);
    glBegin(GL_QUADS);

    glColor4f(1.0f, 1.0, 1.0f, 0.3f); // white
    glVertex3f(0.7f, 1.0f, 0.0f);
    glVertex3f(-0.7f, 1.0f, 0.0f);
    glVertex3f(-0.3f, -1.0f, 0.0f);
    glVertex3f(0.3f, -1.0f, 0.0f);

    glEnd();

    // Design on pyaali -- quad 1
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.25f, 0.0f, +0.0f);
    glScalef(1.0f, 0.9f, 1.0f);        // scaling y axis to reduce height
    glRotatef(7.0f, 0.0f, 0.0f, 1.0f); // rotate along z axis
    glBegin(GL_QUADS);

    glColor4f(1.0f, 1.0, 1.0f, 0.45f); // white
    glVertex3f(0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, -0.9f, 0.0f);
    glVertex3f(0.07f, -0.9f, 0.0f);

    glEnd();
    // Design on pyaali -- quad 2
    // glLoadIdentity();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, +0.0f);
    glScalef(1.0f, 0.9f, 1.0f); // scaling y axis to reduce height
    glBegin(GL_QUADS);

    glColor4f(1.0f, 1.0, 1.0f, 0.45f); // white
    glVertex3f(0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, -0.9f, 0.0f);
    glVertex3f(0.07f, -0.9f, 0.0f);

    glEnd();
    // Design on pyaali -- quad 3
    // glLoadIdentity();

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.25f, 0.0f, +0.0f);
    glScalef(1.0f, 0.9f, 1.0f);         // scaling y axis to reduce height
    glRotatef(-7.0f, 0.0f, 0.0f, 1.0f); // rotate along z axis
    glBegin(GL_QUADS);

    glColor4f(1.0f, 1.0, 1.0f, 0.45f); // white
    glVertex3f(0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, 0.9f, 0.0f);
    glVertex3f(-0.07f, -0.9f, 0.0f);
    glVertex3f(0.07f, -0.9f, 0.0f);

    glEnd();
    // glPopMatrix();
}

// RTR BOARD

// White Rectangle
void DrawWhiteRectangle(void)
{
    // code
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
}

// Red
void BackCircle(void)
{
    // constant declarations
    const float r = 0.5f;
    glLineWidth(5.0f);
    glBegin(GL_LINES);

    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f); // certre of circle

        float x, y;
        x = r * cos(angle);
        y = r * sin(angle);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x, y, 0.0f);
    }

    glEnd();
}

// White
void FrontCircle(void)
{
    // constant declarations
    glLineWidth(5.0f);
    const float r = 0.4f;

    glBegin(GL_LINES);

    for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f); // certre of circle

        float x, y;
        x = r * cos(angle);
        y = r * sin(angle);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(x, y, 0.0f);
    }

    glEnd();
}

// Blue
void Rect(void)
{
    // code
    if (stationScene == 4)
    {
        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.0f, 0.8f);
        glVertex2f(2.5f, 0.59f);
        glVertex2f(-2.5f, 0.59f);
        glVertex2f(-2.5f, -0.5f);
        glVertex2f(2.5f, -0.5f);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(0.4f, 0.0f, 0.8f);
        glVertex2f(2.5f, 0.5f);
        glVertex2f(-2.5f, 0.5f);
        glVertex2f(-2.5f, -0.5f);
        glVertex2f(2.5f, -0.5f);
        glEnd();
    }
}

// RTR Name
void Text_R1(void)
{
    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.4f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, -1.0f, 0.0f);
    glVertex3f(-0.4f, -1.0f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 0.5f, 0.0f);
    glVertex3f(0.9f, 0.5f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 1.0f, 0.0f);
    glVertex3f(0.4f, 1.0f, 0.0f);
    glVertex3f(0.4f, 0.0f, 0.0f);
    glVertex3f(0.9f, 0.0f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 0.2, 0.0f);
    glVertex3f(-0.9f, 0.2f, 0.0f);
    glVertex3f(-0.9f, -0.2f, 0.0f);
    glVertex3f(0.9f, -0.2f, 0.0f);
    glEnd();

    // R
    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.1f, -0.2f, 0.0f);
    glVertex3f(-0.6f, -0.2f, 0.0f);
    glVertex3f(0.4f, -1.0f, 0.0f);
    glVertex3f(0.9f, -1.0f, 0.0f);

    glEnd();
}

void Text_R2(void)
{
    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.4f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, -1.0f, 0.0f);
    glVertex3f(-0.4f, -1.0f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 1.0f, 0.0f);
    glVertex3f(-0.9f, 0.5f, 0.0f);
    glVertex3f(0.9f, 0.5f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 1.0f, 0.0f);
    glVertex3f(0.4f, 1.0f, 0.0f);
    glVertex3f(0.4f, 0.0f, 0.0f);
    glVertex3f(0.9f, 0.0f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.9f, 0.2, 0.0f);
    glVertex3f(-0.9f, 0.2f, 0.0f);
    glVertex3f(-0.9f, -0.2f, 0.0f);
    glVertex3f(0.9f, -0.2f, 0.0f);
    glEnd();

    // R
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.1f, -0.2f, 0.0f);
    glVertex3f(-0.6f, -0.2f, 0.0f);
    glVertex3f(0.4f, -1.0f, 0.0f);
    glVertex3f(0.9f, -1.0f, 0.0f);

    glEnd();
}

void Text_T(void)
{
    // T
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.25f, 1.0f, 0.0f);
    glVertex3f(-0.25f, 1.0f, 0.0f);
    glVertex3f(-0.25f, -1.0f, 0.0f);
    glVertex3f(0.25f, -1.0f, 0.0f);
    glEnd();

    // T
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.8f, 1.0f, 0.0f);
    glVertex3f(-0.8f, 1.0f, 0.0f);
    glVertex3f(-0.8f, 0.5f, 0.0f);
    glVertex3f(0.8f, 0.5f, 0.0f);
    glEnd();
}
