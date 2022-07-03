/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"
#define _USE_MATH_DEFINES
#include <math.h>

/* OpenGL Header files */
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

// for cubeMap Imahe load
//#include "CubeMap/cubeMap.h"

/* OpenGL libraries */
#pragma comment(lib, "glew32.lib")
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

float AngleCube = 90.0f;
GLfloat radius = 8.0f;
float yEyeVector = 0;

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL LoadGLPNGTexture(GLuint *texture, char *image);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

unsigned int cubemapTexture;
GLuint texture;

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
        fprintf(gpFile, "loadGLPNGTexture Failed...\n");
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
    //    BOOL loadGLCubeMap(char *faces[]);
    unsigned int LoadCubemap(char *faces[]);

    /* variable declartions */
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;
    char *faces[] =
        {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"};

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

    // glewInit();

    /* Here start OpeGL Code */
    // Depth related changes
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // BLED
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

    /* Clear the  screen using black color */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // enbale cube map
    // cubemapTexture = LoadCubemap(faces);

    if (LoadGLPNGTexture(&texture, "png.png") == FALSE)
        return -5;

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
float angle = 0.0f;
void display(void)
{
    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    // CUBE ***************************
    glLoadIdentity();
    float angle = AngleCube * M_PI / 180.0f;

    gluLookAt(radius * cos(angle), yEyeVector, radius * sin(angle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    //  FRONT FACE
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
    glBindTexture(GL_TEXTURE_2D, texture);

    // BACK FACE
    glBegin(GL_QUADS);
    // glColor3f(1.0f, 0.0f, 0.0f);
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

    SwapBuffers(ghdc);
}

void update(void)
{
    /* code */
    angle += 0.01f;
    if (angle > 360.0f)
        angle = angle - 360.0f;
}

void uninitialize(void)
{
    /* function declarations */
    void ToggleFullScreen(void);

    /* code */
    if (texture)
    {
        glDeleteTextures(1, &texture);
        texture = 0;
    }

    if (cubemapTexture)
    {
        glDeleteTextures(1, &cubemapTexture);
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

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // create the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
        // glTexImage2D(GL_TEXTURE_2D, 0, );
        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        DeleteObject(hBitmap);
    }
    return bResult;
}

BOOL LoadGLPNGTexture(GLuint *texture, char *image)
{
    // variable declartions
    int iWidth, iHeight, iComponents;

    // code
    // hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    unsigned char *Data = stbi_load(image, &iWidth, &iHeight, &iComponents, 0);

    if (!Data)
    {

        fprintf(gpFile, "Cubemap tex failed to load at path: ");
        stbi_image_free(Data);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glGenTextures(1, texture);

        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // create the texture
        // gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, Data);

        if (iComponents == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        else if (iComponents == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        stbi_image_free(Data);
        return TRUE;
    }

    return FALSE;
}

unsigned int LoadCubemap(char *faces[])
{
    unsigned int texture;
    TCHAR str[1024];
    int iWidth, iHeight, iComponents;

    // code
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &texture);

    // glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /*   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
       glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
       glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);*/
    /*glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
*/
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    /*  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
     */
    // Load cube map images
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *Data = stbi_load(faces[i], &iWidth, &iHeight, &iComponents, 0);
        if (!Data)
        {

            fprintf(gpFile, "Cubemap tex failed to load at path: ");
            stbi_image_free(Data);
        }
        else
        {
            // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
            //            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, iComponents, iWidth, iHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Data);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_BGR_EXT, iWidth, iHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Data);

            // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
            stbi_image_free(Data);
        }
        wsprintf(str, TEXT("Height : %d and Width : %d , iCompoents = %d "), iHeight, iWidth, iComponents);
        MessageBox(NULL, str, TEXT("Desktop Window Sizes"), MB_OK);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP, 3, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, Data);
    }
    //  glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind texture

    return texture;
}
