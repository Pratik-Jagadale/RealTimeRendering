/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* OpenGL Header files */
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

// for cubeMap Imahe load
//#include "CubeMap/cubeMap.h"

/* OpenGL libraries */
//#pragma comment(lib, "glew.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

#define WINWIDTH 800
#define WINHEIGHT 600
unsigned int cubemapTexture;
enum
{
    TGA_UNSUPPORTED = 1,
    TGA_NO_IMAGE = 2,
    TGA_MAP = 4,
    TGA_RGB = 8,
    TGA_BW = 16,
    TGA_RLE = 32
};

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;
int iHeightOfWindow;
int iWidthOfWindow;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>

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
    MessageBox(NULL, TEXT("Creation of Log File Faile..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
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
        fprintf(gpFile, "loadGLTexture Failed for texture kundali ...\n");
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
    BOOL LoadGLTexture(GLuint *, TCHAR[]);
    // BOOL loadGLCubeMap(char *faces[]);
    unsigned int LoadCubemap(char *faces[]);
    char *faces[6] = {"front.tga", "front.tga", "front.tga", "front.tga", "front.tga", "front.tga"};

    /* variable declartions */
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex = 0;

    /* code */
    /* initialization of pixelformatdesciptor structure */
    MessageBox(NULL, TEXT("initaillize enter"), TEXT("File I/O Error"), MB_OK);
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

    /* Clear the  screen using black color */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // enbale cube map
    cubemapTexture = LoadCubemap(faces);

    glEnable(GL_TEXTURE_CUBE_MAP);

    resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL
    MessageBox(NULL, TEXT("initaillize exit"), TEXT("File I/O Error"), MB_OK);

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
    /* Code */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    // CUBE ***************************
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -6.0f);

    glScalef(0.75f, 0.75f, 0.75f);

    //  glRotatef(AngleCube, 1.0f, 1.0f, 1.0f); // Rolling

    //    glBindTexture(GL_TEXTURE_2D, texture_kundali);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glBegin(GL_QUADS);

    // FRONT FACE
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // RIGHT FACE
    glTexCoord2f(1.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // BACK FACE
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, -1.0f, -1.0f);

    // LEFT FACE
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // y = -1

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // TOP
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // BOTTOM FACE
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

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
enum cube
{
    GL_TEXTURE_CUBE_MAP_POSITIVEX,
    GL_TEXTURE_CUBE_MAP_NEGATIVEX,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_z
};
*/

unsigned int LoadCubemap(char *faces[])
{
    unsigned int texture;
    TCHAR str[1024];
    int iWidth, iHeight, iComponents;
    unsigned char *Data = NULL;

    GLubyte *pBytes;
    GLenum eFormat;

    // code
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

    // Load cube map images
    for (int i = 0; i < 6; i++)
    {

        // Load this texture map
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
        pBytes = (GLubyte *)gltLoadTGA(faces[i], &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, iComponents, iWidth, iHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);

        free(pBytes);

        wsprintf(str, TEXT("Height : %d and Width : %d , tgaHeader.cBitsPer = %d "), iHeight, iWidth, iComponents);
        MessageBox(NULL, str, TEXT("Desktop Window Sizes"), MB_OK);
    }

    // gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP, 3, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind texture

    return texture;
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

        // wrap

        // 

        // create the texture
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

        // DELETE Object
        DeleteObject(hBitmap);
    }
    return bResult;
}

struct ImageHeader
{
    GLint width;
    GLint height;
    char bitsPerPixel;
};

struct TGAHEADER
{
    // Lenght of id string
    char cIdLength;

    // Image Storage Info
    char cColorMapType;
    char cImgaetype;

    // Image Description
    short FirstEntry;
    short NumEntries;
    short BitsPerEntry;

    // Image descrption
    short sXOrigin;
    short sYOrigin;
    GLint width;
    GLint height;
    char bits;
    char cDescriptor;
};

////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *gltLoadTGA(const char *szFileName,
                   GLint *iWidth, GLint *iHeight,
                   GLint *iComponents, GLenum *eFormat)
{
    FILE *pFile;              // File pointer
    TGAHEADER tgaHeader;      // TGA file header
    unsigned long lImageSize; // Size in bytes of image
    short sDepth;             // Pixel depth;
    GLbyte *pBits = NULL;     // Pointer to bits
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
    // Attempt to open the file
    pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
        return NULL;
    // Read in header (binary)
    fread(&tgaHeader, 18 /* sizeof(TGAHEADER)*/, 1, pFile);
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    BYTE_SWAP(tgaHeader.colorMapStart);
    BYTE_SWAP(tgaHeader.colorMapLength);
    BYTE_SWAP(tgaHeader.xstart);
    BYTE_SWAP(tgaHeader.ystart);
    BYTE_SWAP(tgaHeader.width);
    BYTE_SWAP(tgaHeader.height);
#endif
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targas.
    if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    // Allocate memory and check for success
    pBits = (GLbyte *)malloc(lImageSize * sizeof(GLbyte));
    if (pBits == NULL)
        return NULL;
    // Read in the bits
    // Check for read error. This should catch RLE or other
    // weird formats that I don't want to recognize
    if (fread(pBits, lImageSize, 1, pFile) != 1)
    {
        free(pBits);
        return NULL;
    }
    // Set OpenGL format expected
    switch (sDepth)
    {
    case 3: // Most likely case
        *eFormat = GL_BGR_EXT;
        *iComponents = GL_RGB8;
        break;
    case 4:
        *eFormat = GL_BGRA_EXT;
        *iComponents = GL_RGBA8;
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE8;
        break;
    };
    // Done with File
    fclose(pFile);
    // Return pointer to image data
    TCHAR str[1024];

    wsprintf(str, TEXT("-------------------------------- "));
    MessageBox(NULL, str, TEXT("Desktop Window Sizes"), MB_OK);

    return pBits;
}