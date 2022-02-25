/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>

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
	iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN);	 // Width Of Window Screen

	/* Create Window */
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
						  TEXT("OpenGL Window - Perspective View - Birds using Geometric shapes"),
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
	/* Clear the  screen using whitw color */
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
	//  function prototype
	void drawBird(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	drawBird();

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

void drawBird(void)
{
	// Rigth wing
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.85f, 0.525f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.75f, 0.5f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.15f, 0.525f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.85f, 0.55f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.85f, 0.525f, 0.0f);
	glEnd();

	// 2 nd
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.75f, 0.45f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.6f, 0.4f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.15f, 0.5f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.75f, 0.45f, 0.0f);

	glEnd();

	// 3 rd
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.4f, 0.40f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.42f, 0.35f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.3f, 0.32f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.07f, 0.43f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.4f, 0.40f, 0.0f);
	glEnd();

	// 4 th
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.28f, 0.33f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.3f, 0.26f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.01f, 0.4f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.28f, 0.33f, 0.0f);

	glEnd();

	// 5 th
	glBegin(GL_QUADS);
	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.2f, 0.285f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.2f, 0.225f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.01f, 0.365f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.2f, 0.285f, 0.0f);

	glEnd();

	// 6 th
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.13f, 0.25f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.1f, 0.18f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.0f, 0.33f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.13f, 0.25f, 0.0f);
	glEnd();

	// Center (NOT SET)
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.26f, 0.38f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.01f, 0.27f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.2f, -0.12f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.17f, 0.1f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.26f, 0.38f, 0.0f);
	glEnd();

	//	 center down 1
	glBegin(GL_QUADS);
	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.1f, 0.1f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.25f, -0.05f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.3f, -0.2f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.1f, 0.1f, 0.0f);

	glEnd();

	// center down 2

	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.3f, -0.09f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.585f, -0.305f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.35f, -0.242f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.3f, -0.09f, 0.0f);

	glEnd();

	// center down 3
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(-0.335f, -0.26f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(-0.378f, -0.498f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(-0.16f, -0.207f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(-0.335f, -0.26f, 0.0f);

	glEnd();

	// center down 4
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.024f, -0.048f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.296f, -0.22f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.124f, -0.172f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(-0.024f, -0.048f, 0.0f);

	glEnd();

	// center left top 1
	glBegin(GL_QUADS);
	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.28f, 0.58f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.23f, 0.50f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.155f, 0.605f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(-0.28f, 0.58f, 0.0f);

	glEnd();

	// center left top 2
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.0f, 0.55f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.045f, 0.375f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.23f, 0.50f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.0f, 0.55f, 0.0f);

	glEnd();

	// center top 1
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.265f, 0.53f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.555f, 0.645f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.49f, 0.67, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.265f, 0.53f, 0.0f);
	glEnd();

	// center top 2
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.585f, 0.626f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.582f, 0.568f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.676f, 0.685, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.585f, 0.626f, 0.0f);
	glEnd();

	// center top 3
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.582f, 0.568f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.63f, 0.591f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.676f, 0.685, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.585f, 0.568f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.256f, 0.508f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.275f, 0.445f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.43f, 0.545f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.256f, 0.508f, 0.0f);
	glEnd();

	// center top 6
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.23f, 0.49f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.04f, 0.32f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.27f, 0.41f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.23f, 0.49f, 0.0f);

	glEnd();

	// EYE
	glPointSize(8.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.570f, 0.0f);

	glEnd();

	// Center top right side
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.40f, 0.462f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.43f, 0.405f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.56f, 0.55f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.295f, 0.462f, 0.0f);

	glEnd();

	// Center top right side
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.45f, 0.385, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	;
	glVertex3f(0.53f, 0.345f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.595f, 0.545f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.45f, 0.385f, 0.0f);

	glEnd();

	// Center top left wing top 1
	glBegin(GL_QUADS);
	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.426f, 0.38, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.35f, 0.25f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.57f, 0.14f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.6f, 0.27f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.426f, 0.38f, 0.0f);
	glEnd();

	// Center top left wing top
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.57f, 0.14, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.695f, -0.386f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.68f, 0.135f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.6f, 0.27f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.57f, 0.14f, 0.0f);
	glEnd();

	//  right wing
	glBegin(GL_QUADS);
	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.282f, 0.394f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.212f, 0.118f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.4f, 0.38f, 0.0f);

	glColor3f(0.501f, 0.435f, 0.011f);
	glVertex3f(0.282f, 0.394f, 0.0f);
	glEnd();

	// 11 th last
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.247f, 0.14f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.17f, 0.0f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.205f, 0.0f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.247f, 0.14f, 0.0f);
	glEnd();

	// 10 th last
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.314f, 0.206f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.216f, -0.052f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.264f, -0.11f, 0.0f);

	glColor3f(0.14f, 0.11f, 0.07f);
	glVertex3f(0.314f, 0.206f, 0.0f);
	glEnd();

	// 9 th  missing

	// 8 th last
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.41f, 0.194f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.31f, -0.19f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.355f, -0.17f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.41f, 0.194f, 0.0f);

	glEnd();

	// 7 th last
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.44f, 0.18f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.35f, -0.25f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.395f, -0.23f, 0.0f);

	glColor3f(0.25f, 0.21f, 0.003f);
	glVertex3f(0.44f, 0.18f, 0.0f);

	glEnd();

	// 6 th last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.475f, 0.12f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.400f, -0.34f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.43f, -0.314f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.475f, 0.12f, 0.0f);

	glEnd();

	// 5 th last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.512f, 0.08f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.435f, -0.42f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.47f, -0.395f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.512f, -0.008f, 0.0f);
	glEnd();

	// 4 th last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.548f, -0.014f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.465f, -0.51f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.505f, -0.48f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.548f, -0.014f, 0.0f);
	glEnd();

	// 3 rd last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.58f, -0.04f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.51f, -0.64f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.57f, -0.61f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.58f, -0.04f, 0.0f);
	glEnd();

	// 2 ns last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.61f, -0.22f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.57f, -0.84f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.61f, -0.74f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.62f, -0.22f, 0.0f);
	glEnd();

	// last
	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.65f, -0.32f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.61f, -0.94f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.65f, -0.98f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.68f, -0.89f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.69f, -0.48f, 0.0f);

	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(0.65f, -0.32f, 0.0f);
	glEnd();
}