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
BOOL gbActiveWindow = FALSE;

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
void setColor(float r, float g, float b);

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
	/* Function protype */
	void drawGarden(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -9.0f);

	drawGarden();

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

#define PI 3.1459265

void drawGarden(void)
{

	for (float k = 0.0f; k < 5.0f; k = k + 0.5f)
	{
		glLoadIdentity();
		glTranslatef(-5.0f, -2.0f, -9.0f - k);

		glScalef(0.50f, 0.50f, 0.0f);
		void grass1(void);
		void grass2(void);

		glPushMatrix();
		for (int i = 0; i < 30; i++)
		{
			glTranslatef(0.5, 0.0f, 0.0f);
			grass1();
		}

		glPopMatrix();
		// glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		for (int i = 0; i < 30; i++)
		{
			glTranslatef(0.7, 0.0f, 0.0f);
			grass2();
		}
	}

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -9.0f);

	glScalef(0.50f, 0.50f, 0.0f);

	glTranslatef(-8.0f, -3.0f, 0.0f);
	// Vertical BAR
	for (int i = 0; i < 8; i++)
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

void grass1(void)
{
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
