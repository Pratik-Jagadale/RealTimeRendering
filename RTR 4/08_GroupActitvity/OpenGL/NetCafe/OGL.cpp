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

///////////////////////////////////BIRTHDAY DEMO CHANGES

#pragma comment(lib, "Winmm.lib")

float AnglePyramid = 0.0f;
float AngleCube = 0.0f;
int flagTexture = -1;
float yEye = 0.0f;
float g = 0.0f;
int flagTextureKeyBoard = -1;
float eye = 0.0f;

enum
{
    TEXTURE_WIN_7,
    TEXTURE_WIN_XP,
    TEXTURE_SMILEY,
    TEXTURE_INTERNET,
    TEXTURE_LAMINATION_WODDEN_FLOORING,
    TEXTURE_TILE_WALLPAPER,        // 5
    TEXTURE_WALLPAPER_OLIVE_GREEN, // 6
    TEXTURE_WALLPAPER_BLUE_DESIGNE,
    TEXTURE_WODDEN_TEXTURE_DARK,
    TEXTURE_WOODEN_TEEXTURE_LIGHT,
    TEXTURE_CEILING,
    BLACK,
    TEXTURE_DOOR,
    TEXTURE_KEYBOARD
};

GLuint texture_WIN_7;
GLuint texture_WIN_XP;
GLuint texture_Internet;
GLuint texture_LaminatedWoodenFlooring;
GLuint texture_TileWallpaper;
GLuint texture_Wallpaper_Olive_Green;
GLuint texture_WoodenTexture_Dark;
GLuint texture_WoodenTexture_Light;
GLuint texture_Ceiling;
GLuint texture_Door;
GLuint texture_sky;
GLuint texture_building_one;
GLuint texture_building_two;
GLuint texture_Keyboard;
GLuint texture_Internet_Cafe;
//-------------------------------------------------
GLuint texture_Road;
GLuint texture_BaseTile;
//////////////////////////////////////////////////

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
        fprintf(gpFile, " loadGLTexture Failed for texture_stone ...\n");
        uninitialize();
    }
    else if (iRetVal == -6)
    {
        fprintf(gpFile, "loadGLTexture Failed for texture kundali ...\n");
        uninitialize();
    }
    else
    {
        fprintf(gpFile, "Initialize Successfull...\n");
    }

    ///////////////////////////////////
    PlaySound(
        MAKEINTRESOURCE(MYMUSIC),
        GetModuleHandle(NULL),
        SND_RESOURCE | SND_ASYNC);

    //////////////////////////////

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
    /* Clear the  screen using black color */
    glClearColor(0.45f, 0.45f, 0.45f, 0.0f);

    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    ///////////////////////////////// BIRTHDAY DEMO CHANGES

    if (LoadGLTexture(&texture_WIN_7, MAKEINTRESOURCE(IDBITMAP_WIN_7)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_WIN_XP, MAKEINTRESOURCE(IDBITMAP_WIN_XP)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Internet, MAKEINTRESOURCE(IDBITMAP_INTERNET)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Internet_Cafe, MAKEINTRESOURCE(IDBITMAP_INTERNET_CAFE)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_LaminatedWoodenFlooring, MAKEINTRESOURCE(IDBITMAP_LAMINATEDWOODENFLOORING)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_TileWallpaper, MAKEINTRESOURCE(IDBITMAP_TILEWALLPAPER)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Wallpaper_Olive_Green, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_OLIVEGREEN)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_WoodenTexture_Dark, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_WOODENTEXTUREDARK)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_WoodenTexture_Light, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_WOODENTEXTURE_LIGHT)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Ceiling, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_CEILING)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Door, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_DOOR)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_sky, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_SKY)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_building_one, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_BUILDING_ONE)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_building_two, MAKEINTRESOURCE(IDBITMAP_WALLPAPER_BUILDING_TWO)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Keyboard, MAKEINTRESOURCE(IDBITMAP_KEYBOARD_TEXTURE)) == FALSE)
        return -6; // write log in wndproc
                   //---------------------------------------------------------------------------------------------------------------
    if (LoadGLTexture(&texture_BaseTile, MAKEINTRESOURCE(IDBITMAP_BASETILE)) == FALSE)
        return -6; // write log in wndproc

    if (LoadGLTexture(&texture_Road, MAKEINTRESOURCE(IDBITMAP_ROAD)) == FALSE)
        return -6; // write log in wndproc
                   //-------------------------------------------------------------------------------------------------------------------
    ///////////////////////////////////////////////////////////////////
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
    void DrawICafe(void);
    void drawOutSide(void);

    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ///////////////////////////////// BIRTHDAY DEMO CHANGES
    gluLookAt(0.0f, 0.0f + yEye, 0.0f + eye, 0.0f, 0.0f, -6.f + g, 0.0, 1.0, 0.0f);

    glPushMatrix();
    DrawICafe();

    glPopMatrix();

    glPushMatrix();

    flagTexture = -1;
    drawOutSide();

    glPopMatrix();
    //////////////////////////////////
    SwapBuffers(ghdc);
}

void update(void)
{
    ///////////////////////////////// BIRTHDAY DEMO CHANGES
    if (AngleCube > 360.0f)
        AngleCube = 0.0f;

    if (eye < 12.0f)
        eye = eye + 0.001f;
    else if (eye < 22.0f)
    {
        eye = eye + 0.01f;
        yEye = yEye + 0.005f;
    }
    else if (eye < 32.0f)
    {
        yEye = yEye + 0.0001f;
        eye = eye + 0.001f;
    }
    ////////////////////////////////
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

    ///////////////////////////////// BIRTHDAY DEMO CHANGES

    if (texture_WIN_7)
    {
        glDeleteTextures(1, &texture_WIN_7);
        texture_WIN_7 = NULL;
    }

    if (texture_WIN_XP)
    {
        glDeleteTextures(1, &texture_WIN_XP);
        texture_WIN_XP = NULL;
    }

    if (texture_Internet)
    {
        glDeleteTextures(1, &texture_WoodenTexture_Dark);
        texture_WoodenTexture_Dark = NULL;
    }

    if (texture_Internet)
    {
        glDeleteTextures(1, &texture_Internet);
        texture_Internet = NULL;
    }
    if (texture_Internet_Cafe)
    {
        glDeleteTextures(1, &texture_Internet_Cafe);
        texture_Internet_Cafe = NULL;
    }

    if (texture_LaminatedWoodenFlooring)
    {
        glDeleteTextures(1, &texture_LaminatedWoodenFlooring);
        texture_LaminatedWoodenFlooring = NULL;
    }

    if (texture_LaminatedWoodenFlooring)
    {
        glDeleteTextures(1, &texture_LaminatedWoodenFlooring);
        texture_LaminatedWoodenFlooring = NULL;
    }

    if (texture_TileWallpaper)
    {
        glDeleteTextures(1, &texture_TileWallpaper);
        texture_TileWallpaper = NULL;
    }

    if (texture_Wallpaper_Olive_Green)
    {
        glDeleteTextures(1, &texture_Wallpaper_Olive_Green);
        texture_Wallpaper_Olive_Green = NULL;
    }

    if (texture_WoodenTexture_Dark)
    {
        glDeleteTextures(1, &texture_WoodenTexture_Dark);
        texture_WoodenTexture_Dark = NULL;
    }

    if (texture_WoodenTexture_Light)
    {
        glDeleteTextures(1, &texture_WoodenTexture_Light);
        texture_WoodenTexture_Light = NULL;
    }

    if (texture_Ceiling)
    {
        glDeleteTextures(1, &texture_Ceiling);
        texture_Ceiling = NULL;
    }

    if (texture_Door)
    {
        glDeleteTextures(1, &texture_Door);
        texture_Door = NULL;
    }

    if (texture_sky)
    {
        glDeleteTextures(1, &texture_sky);
        texture_sky = NULL;
    }

    if (texture_building_one)
    {
        glDeleteTextures(1, &texture_building_one);
        texture_building_one = NULL;
    }

    if (texture_building_two)
    {
        glDeleteTextures(1, &texture_building_two);
        texture_building_two = NULL;
    }

    if (texture_Keyboard)
    {
        glDeleteTextures(1, &texture_Keyboard);
        texture_Keyboard = NULL;
    }
    //////////////////////////
    //-------------------------------------------------------------------------------------------
    if (texture_BaseTile)
    {
        glDeleteTextures(1, &texture_BaseTile);
    }

    if (texture_Road)
    {
        glDeleteTextures(1, &texture_Road);
    }
    //-------------------------------------------------------------------------------------------
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

void DrawICafe(void)
{
    void DrawBox(void);
    void DrawComputer(void);

    // CUBE
    glTranslatef(0.0f, 0.0f, 2.0f);
    glScalef(4.5f, 3.0f, 7.0f);

    glRotatef(AngleCube, 1.0f, 1.0f, 1.0f); // Rolling

    flagTexture = TEXTURE_DOOR;
    DrawBox();
    flagTexture = -1;
    glPopMatrix();
    glPushMatrix();

    // Back Side Table
    glTranslatef(0.0f, -1.5f, -4.01f + g);
    glScalef(5.0f, 0.08f, 0.75f);

    glBindTexture(GL_TEXTURE_2D, texture_WoodenTexture_Light);
    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Left Side Table
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(0.0f - g, -1.5f, -4.0f);
    glScalef(5.0f, 0.08f, 1.0f);
    // use texture

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Right Side Table
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(0.0f + g, -1.5f, -4.0f);
    glScalef(5.0f, 0.08f, 1.0f);

    // draw Box for right tabke
    DrawBox();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    // Chairs 1
    glTranslatef(1.7f + g, -2.5f, -3.0f);
    glScalef(0.45f, 0.45f, 0.45f);

    glBindTexture(GL_TEXTURE_2D, texture_WoodenTexture_Dark);

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Chairs 2
    glTranslatef(-1.7f + g, -2.5f, -3.0f);
    glScalef(0.45f, 0.45f, 0.45f);

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Chairs 3
    glTranslatef(-2.0f + g, -2.5f, -0.5f);
    glScalef(0.45f, 0.45f, 0.45f);

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Chairs 4
    glTranslatef(2.0f + g, -2.5f, -0.5f);
    glScalef(0.45f, 0.45f, 0.45f);

    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    // Back Wallper

    glTranslatef(0.0f, 1.2f, -5.0f);
    glScalef(1.5f, 1.5f, 0.10f);

    glBindTexture(GL_TEXTURE_2D, texture_Internet);
    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    // Computer 1
    glTranslatef(-1.4f, -0.1f, -3.5f + g);
    glScalef(0.72f, 0.70f, 0.70f);

    DrawComputer();

    glPopMatrix();
    glPushMatrix();

    // Computer 2
    glTranslatef(1.4f, -0.1f, -3.5f + g);
    glScalef(0.72f, 0.70f, 0.70f);

    // draw Box for right tabke
    DrawComputer();

    glPopMatrix();
    glPushMatrix();

    // Computer 3
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-1.0f, -0.1f, -3.6f + g);
    glScalef(0.72f, 0.70f, 0.70f);

    // draw Box for right tabke
    DrawComputer();

    glPopMatrix();
    glPushMatrix();

    // Computer 4
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.0f, -0.1f, -3.6f + g);
    glScalef(0.72f, 0.70f, 0.70f);

    // draw Box for right tabke
    DrawComputer();

    glPopMatrix();
}

void DrawComputer(void)
{
    void DrawBox(void);
    static int iScreenFlag = 0;

    // desktop
    glPushMatrix();

    // Screen
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(AngleCube, 0.0f, 1.0f, 0.0f);
    glScalef(1.5f, 1.0f, 0.05f);
    flagTexture = BLACK;
    DrawBox();
    flagTexture = -1;

    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 0.0001f);
    glRotatef(AngleCube, 0.0f, 1.0f, 0.0f);
    glScalef(1.4f, 0.9f, 0.05f);

    if (iScreenFlag == 0)
    {
        glBindTexture(GL_TEXTURE_2D, texture_WIN_7);
        iScreenFlag = 1;
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture_WIN_XP);
        iScreenFlag = 0;
    }

    DrawBox();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Stand Vertical Bar
    flagTexture = BLACK;
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0.0f, -1.3f, 0.0f);
    glRotatef(AngleCube, 0.0f, 1.0f, 0.0f);
    glScalef(0.20f, 0.30f, 0.05f);

    DrawBox();

    // Stand Horizontal Bar

    glPopMatrix();
    glPushMatrix();

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glTranslatef(0.0f, 0.0f, -1.5f);
    glRotatef(AngleCube, 0.0f, 0.0f, 1.0f);
    glScalef(0.40f, 0.40f, 0.05f);

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // KeyBoard
    flagTextureKeyBoard = TEXTURE_KEYBOARD;
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glTranslatef(0.0f, -1.2f, -1.5f);
    // glRotatef(AngleCube, 0.0f, 0.0f, -1.0f);
    glScalef(1.0f, 0.40f, 0.02f);

    DrawBox();

    flagTextureKeyBoard = -1;

    // MOUSE
    glPopMatrix();
    glPushMatrix();

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glTranslatef(1.2f, -1.2f, -1.5f);
    // glRotatef(AngleCube, 0.0f, 0.0f, -1.0f);
    glScalef(0.10f, 0.10f, 0.06f);

    DrawBox();

    flagTexture = -1;

    glPopMatrix();
}

void DrawBox(void)
{
    //    glRotatef(90.0f,0.0f,0.0f,1.0f);
    if (flagTexture == TEXTURE_DOOR)
        glBindTexture(GL_TEXTURE_2D, texture_Door);

    if (flagTextureKeyBoard == TEXTURE_KEYBOARD)
    {
        glBindTexture(GL_TEXTURE_2D, texture_Keyboard);
    }

    glBegin(GL_QUADS);
    // FRONT FACE

    if (flagTexture == BLACK && flagTextureKeyBoard != TEXTURE_KEYBOARD)
        glColor3f(0.25f, 0.25f, 0.25f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    if (flagTexture == TEXTURE_DOOR)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = TEXTURE_WALLPAPER_OLIVE_GREEN;
    }

    // RIGHT FACE

    if (flagTexture == TEXTURE_WALLPAPER_OLIVE_GREEN)
        glBindTexture(GL_TEXTURE_2D, texture_Wallpaper_Olive_Green);
    // glColor3f(0.4f, 0.803f, 0.419f);

    glBegin(GL_QUADS);
    if (flagTexture == BLACK)
        glColor3f(0.25f, 0.25f, 0.25f);

    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    if (flagTexture == TEXTURE_WALLPAPER_OLIVE_GREEN)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = TEXTURE_WALLPAPER_OLIVE_GREEN;
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // BACK FACE
    if (flagTexture == TEXTURE_WALLPAPER_OLIVE_GREEN)
        glBindTexture(GL_TEXTURE_2D, texture_Wallpaper_Olive_Green);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    if (flagTextureKeyBoard == TEXTURE_KEYBOARD)
        glColor3f(1.0f, 0.0f, 1.0f);

    if (flagTexture == TEXTURE_WALLPAPER_BLUE_DESIGNE)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = TEXTURE_WALLPAPER_OLIVE_GREEN;
    }

    // LEFT FACE
    if (flagTexture == TEXTURE_WALLPAPER_OLIVE_GREEN)
    {
        glBindTexture(GL_TEXTURE_2D, texture_Wallpaper_Olive_Green);
    }
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

    if (flagTexture == TEXTURE_WALLPAPER_OLIVE_GREEN)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = TEXTURE_CEILING;
    }

    // TOP
    if (flagTexture == TEXTURE_CEILING)
        glBindTexture(GL_TEXTURE_2D, texture_Ceiling);

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

    if (flagTexture == TEXTURE_CEILING)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = TEXTURE_TILE_WALLPAPER;
    }

    if (flagTextureKeyBoard == TEXTURE_KEYBOARD)
    {
        flagTextureKeyBoard = -1;
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // BOTTOM FACE
    if (flagTexture == TEXTURE_TILE_WALLPAPER)
        glBindTexture(GL_TEXTURE_2D, texture_TileWallpaper);
    if (flagTextureKeyBoard == TEXTURE_KEYBOARD)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture_Keyboard);
    }

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

    if (flagTexture == TEXTURE_TILE_WALLPAPER)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        flagTexture = 0;
    }

    if (flagTexture == BLACK)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
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

void drawOutSide(void)
{
    void DrawBox(void);
    // base

    glBindTexture(GL_TEXTURE_2D, texture_BaseTile);

    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, -3.1f, -50.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, -3.1f, -50.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -3.1f, 30.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -3.1f, 30.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();
    //-----------------------------------------------------------------------
    // ROAD

    glBindTexture(GL_TEXTURE_2D, texture_Road);
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, -3.001f, 9.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, -3.001f, 9.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -3.001f, 13.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -3.001f, 13.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    glPushMatrix();

    //-------------------------------------------------------------------------------
    // BACKGROUND
    flagTexture = -1;
    glBindTexture(GL_TEXTURE_2D, texture_sky);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 40.0f, -25.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 40.1f, -25.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -10.0f, -25.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -10.0f, -25.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPushMatrix();

    // left 2 building
    glBindTexture(GL_TEXTURE_2D, texture_building_one);
    glTranslatef(-14.0f, 0.0f, 0.0f);
    glScalef(3.0f, 8.0f, 3.0f);
    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // left 1 building
    glTranslatef(-8.0f, 0.0f, 0.0f);
    glScalef(3.0f, 5.0f, 3.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_two);
    DrawBox();

    glPopMatrix();
    glPushMatrix();
    //------------------------------------------------------------------------------------
    glTranslatef(-20.0f, 0.0f, 0.0f);
    glScalef(3.0f, 7.0f, 3.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_two);
    DrawBox();

    glPopMatrix();
    glPushMatrix();
    //-------------------------------------------------------------------------------------
    // Right 1 building
    glTranslatef(11.5f, 0.0f, 0.0f);
    glScalef(6.5f, 8.0f, 4.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_two);

    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Right 1 building

    glTranslatef(14.5f, 0.0f, -1.0f);
    glScalef(3.0f, 8.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_one);
    DrawBox();

    glPopMatrix();
    glPushMatrix();
    //-------------------------------------------------------------------------------
    // Right 2 building
    glTranslatef(16.5f, 4.0f, -1.0f);
    glScalef(4.0f, 7.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_one);
    DrawBox();

    glPopMatrix();
    glPushMatrix();

    // Right 3 building
    glTranslatef(16.5f, 3.0f, 4.0f);
    glScalef(4.0f, 10.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture_building_one);
    DrawBox();
    //---------------------------------------------------------------------------------------
    // middle
    glPopMatrix();
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_building_one);
    glTranslatef(0.0f, 0.0f, -10.0f);
    glScalef(4.0f, 8.0f, 4.0f);

    DrawBox();

    // Outer Board
    glPopMatrix();
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_Internet_Cafe);
    glTranslatef(0.0f, 4.0f, 9.0f);
    glScalef(4.0f, 1.0f, 0.10f);

    DrawBox();
}
