// Header Files
#include <Windows.h>
#include <stdlib.h> //For exit()
#include <stdio.h>	//For File IO
#include "OGL.h"	//For Icon Resource
#define _USE_MATH_DEFINES
#include <math.h>

// include header file here
#include <GL/gl.h>
#include <GL/glu.h>

#define WIN_WIDTH 800
#define WIN_HIGHT 600

// openGL libraries

#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "glu32.lib")

// Global Function Declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// long __far __stdcall WndProc(unsigned int, unsigned int, unsigned int, long);

// Global variable declarations
BOOL gbActiveWindow = FALSE;
BOOL gbFullScreen = FALSE;
FILE *gfFile = NULL;
HWND gHwnd = NULL;
HDC gHdc = NULL;
HGLRC ghrc = NULL;

float angleCube = 0.0f;
int flag = 0;
float AngleCube = 270.0f;
GLfloat radius = 8.0f;
float yEyeVector = 0;
GLUquadric *quadric = NULL;
BOOL bIsTrunk = TRUE;

GLuint texture_wood;
GLuint texture_leaf;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declarations
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	if (fopen_s(&gfFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("File Creation Failed.... exiting"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gfFile, "Log File Successfully Created\n");
	}

	// Initialization of WNDCLASSEX structure
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

	// Register wndclass
	RegisterClassEx(&wndclass);

	// Create The Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
						  TEXT("Pratik R Jagadale"),
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						  (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2,
						  (GetSystemMetrics(SM_CYSCREEN) - WIN_HIGHT) / 2,
						  WIN_WIDTH,
						  WIN_HIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	gHwnd = hwnd;

	// initialize
	iRetVal = initialize();
	if (iRetVal == -1)
	{
		fprintf(gfFile, "Choose Pixcel Format failed\n ");
		uninitialize();
	}
	if (iRetVal == -2)
	{
		fprintf(gfFile, "Set Pixcel Format failed\n ");
		uninitialize();
	}
	if (iRetVal == -3)
	{
		fprintf(gfFile, " Create OpenGL context failed\n ");
		uninitialize();
	}

	if (iRetVal == -4)
	{
		fprintf(gfFile, " Making OpenGL context current context failed\n ");
		uninitialize();
	}
	if (iRetVal == -5)
	{
		fprintf(gfFile, " Making OpenGL creation of quadric failed\n ");
		uninitialize();
	}
	if (iRetVal == -6)
	{
		fprintf(gfFile, " Making OpenGL Texture wood failed\n ");
		uninitialize();
	}
	if (iRetVal == -7)
	{
		fprintf(gfFile, " Making OpenGL Texture leaf failed\n ");
		uninitialize();
	}

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Forgrounding and focusing the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
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
				// Render the Scene
				display();
				// upadate the Scene
				update();
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);
}

// CALLBACK FUNCTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam)
{

	// function declarations
	void toggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);
	// code
	switch (imsg)
	{
	case WM_ERASEBKGND:
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			toggleFullScreen();
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 27:
			DestroyWindow(hwnd);
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

		default:
			break;
		}
		break;
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
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
	return (DefWindowProc(hwnd, imsg, wParam, lParam));
}

void toggleFullScreen()
{
	// variable declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(gHwnd, &wp) && GetMonitorInfo(MonitorFromWindow(gHwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(gHwnd, GWL_STYLE, dwStyle & (~WS_OVERLAPPEDWINDOW));
				SetWindowPos(gHwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, (mi.rcMonitor.right - mi.rcMonitor.left), (mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{

		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wp);
		SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

int initialize(void)
{

	// function declarations
	BOOL LoadGLTexture(GLuint * texture, TCHAR ImageResourceID[]);
	void resize(int, int);
	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 32; // Change For depth 24 also can be done
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	// GetDC
	gHdc = GetDC(gHwnd);
	// choose pixel format
	iPixelFormatIndex = ChoosePixelFormat(gHdc, &pfd);
	if (iPixelFormatIndex == 0)
		return -1;

	// Set The chosen pixel format
	if (SetPixelFormat(gHdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return -2;
	}
	// Create OpenGL rendering Context
	ghrc = wglCreateContext(gHdc);
	if (ghrc == NULL)
		return -3;

	// make the rendering as current context.
	if (wglMakeCurrent(gHdc, ghrc) == FALSE)
		return -4;

	// Here Starts openGl code
	// clearScreen using Blue color

	// warmup resize call
	resize(WIN_WIDTH, WIN_HIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Depth Related Changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// Optional
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	quadric = gluNewQuadric();
	if (quadric == NULL)
	{
		return -5;
	}

	if (LoadGLTexture(&texture_wood, MAKEINTRESOURCE(IDBITMAP_WOOD)) == FALSE)
	{

		fprintf(gfFile, "LoadGLTexture failed for wood");
		return -6;
	}

	if (LoadGLTexture(&texture_leaf, MAKEINTRESOURCE(IDBITMAP_LEAF)) == FALSE)
	{

		fprintf(gfFile, "LoadGLTexture failed for leaf");
		return -7;
	}
	glEnable(GL_TEXTURE_2D);

	return 0;
}

void resize(int width, int hight)
{

	// code
	if (hight == 0)
		hight = 1; // To avoid divided by 0 exception

	glViewport(0, 0, (GLsizei)width, (GLsizei)hight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)hight, 0.1f, 100.0f);
}

void display(void)
{
	void drawCylinder(GLfloat base, GLfloat top, GLfloat height);
	void drawTree(GLfloat, GLfloat, GLfloat);
	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float angle = AngleCube * M_PI / 180.0f;

	gluLookAt(radius * cos(angle), yEyeVector, radius * sin(angle),
			  0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	// glTranslatef(0.0f,-10.0f, -50.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawTree(0.5f, 0.5f, 10.0f);
	bIsTrunk = TRUE;
	fprintf(gfFile, "\n\n\n\n");

	SwapBuffers(gHdc);
}

void drawTree(GLfloat base, GLfloat top, GLfloat height)
{
	void drawCylinder(GLfloat, GLfloat, GLfloat);
	// GLfloat subHeight = height - (height / 3);

	if (bIsTrunk == TRUE)
	{
		// glColor3f(150.0f / 255.0f, 75.0f / 255.0f, 0.0f / 255.0f);
		// glPushMatrix();
		drawCylinder(base, top, height);

		bIsTrunk = FALSE;
		// glPopMatrix();
	}
	else
	{
		if (height <= 2.0f)
		{
			// glColor3f(0.0f,1.0f,0.0f);
			glBindTexture(GL_TEXTURE_2D, texture_leaf);
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.0f);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			// glColor3f(150.0f / 255.0f, 75.0f / 255.0f, 0.0f / 255.0f);
			drawCylinder(base, top, height);
		}
	}

	if (height <= 1.0f)
	{

		return;
	}
	else
	{
		glPushMatrix();
		glTranslatef(0.0f, height, 0.0f);
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		drawTree(top, base * 0.5f, height * 0.6f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, height * 0.7f, 0.0f);
		glRotatef(45.0f, 0.0f, 0.0f, -1.0f);
		drawTree(top, base * 0.5f, height * 0.6f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, height * 0.9f, 0.0f);
		glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
		drawTree(top, base * 0.5f, height * 0.6f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, height * 0.5f, 0.0f);
		glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
		drawTree(top, base * 0.5f, height * 0.6f);
		glPopMatrix();
	}
}

void drawCylinder(GLfloat base, GLfloat top, GLfloat height)
{
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture_wood);
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, base, top, height, 10, 10);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void update(void)
{

	// code
	angleCube = angleCube + 0.08f;
	if (angleCube > 360.0f)
	{
		angleCube = -360.0f;
	}
}

void uninitialize(void)
{

	// function declarations
	void toggleFullScreen(void);
	// code
	if (gbFullScreen)
	{
		toggleFullScreen();
	}
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}
	if (gHdc)
	{
		ReleaseDC(gHwnd, gHdc);
		gHdc = NULL;
	}
	if (gHwnd)
	{
		DestroyWindow(gHwnd);
		gHwnd = NULL;
	}
	if (gfFile)
	{
		fprintf(gfFile, "Log File Successfully Closed");
		fclose(gfFile);
		gfFile = NULL;
	}
	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
	if (texture_leaf)
	{
		glDeleteTextures(1, &texture_leaf);
		texture_leaf = 0;
	}
	if (texture_wood)
	{
		glDeleteTextures(1, &texture_wood);
		texture_wood = 0;
	}
}

BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[])
{

	// variable declaration

	HBITMAP hBitMap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	hBitMap = (HBITMAP)LoadImage(GetModuleHandle(NULL),
								 ImageResourceID,
								 IMAGE_BITMAP,
								 0,
								 0,
								 LR_CREATEDIBSECTION);

	if (hBitMap)
	{

		bResult = TRUE;
		GetObject(hBitMap, sizeof(bmp), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// create the texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		glBindTexture(GL_TEXTURE_2D, 0);

		DeleteObject(hBitMap);
	}

	return bResult;
}