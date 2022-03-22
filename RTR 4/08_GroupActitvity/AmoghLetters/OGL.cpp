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
	void drawAmogh(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	drawAmogh();

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
////////////////changes for amogh collor toggle /////////////
YELLOW, LAVENDER , PI macros
*/
#define YELLOW 0
#define LAVENDER 1
#define PI 3.1459265

float yellow[] = {1.0f, 1.0f, 0.0f};
float lavender[] = {0.5f, 0.5f, 1.0f};

///////////////////
void drawAmogh(void)
{
	/* function prototype */
	void toggleCollor();
	void drawShadowAmogh();

	/* Code */
	toggleCollor();
	drawShadowAmogh();

	glLineWidth(10);

	glBegin(GL_QUADS);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7744f, 0.162f, 0.05f);   // top
	glVertex3f(-0.8866f, -0.1832f, 0.05f); // bottom
	glVertex3f(-0.839f, -0.1826f, 0.05f);  // bottom
	glVertex3f(-0.7202f, 0.1616f, 0.05f);  // top
	glEnd();

	glBegin(GL_QUADS);

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.7744f, 0.162f, 0.0f); // top

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.8866f, -0.1832f, 0.0f); // bottom
	glVertex3f(-0.839f, -0.1826f, 0.0f);  // bottom

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.7202f, 0.1616f, 0.0f); // top
	glEnd();

	// mid
	glBegin(GL_QUADS);
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.8045f, -0.0769f, 0.0f);  // top
	glVertex3f(-0.6941f, -0.0765f, 0.0f);  // top
	glVertex3f(-0.79607f, -0.0361f, 0.0f); // top
	glVertex3f(-0.70203f, -0.0369f, 0.0f); // top

	glEnd();

	// right

	glBegin(GL_QUADS);
	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.6594f, -0.1838f, 0.0f); // bottom
	glVertex3f(-0.6103f, -0.1825f, 0.0f); // bottom

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.7202f, 0.1616f, 0.0f); // top
	glVertex3f(-0.7744f, 0.162f, 0.0f);	 // top

	glEnd();

	/*   M  */
	glBegin(GL_QUADS);

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.5513f, 0.1625f, 0.0f); // top

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.5509f, -0.1835f, 0.0f); // bottom

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.5065f, -0.1704f, 0.0f); // bottom

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.5065f, 0.1619f, 0.0f); // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.5513f, 0.1625f, 0.0f); // top

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.4168f, -0.1842f, 0.0f); // bottom
	glVertex3f(-0.3579f, -0.1832f, 0.0f); // bottom
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.47976f, 0.162f, 0.0f); // top
	glEnd();

	glBegin(GL_QUADS);

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.41697f, -0.18417f, 0.0f); // bottom
	glVertex3f(-0.35967f, -0.18439f, 0.0f); // bottom

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.2199f, 0.16233f, 0.0f); // top;
	glVertex3f(-0.2929f, 0.16233f, 0.0f); // top
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.274f, 0.1621f, 0.0f); // top

	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(-0.274f, -0.1846f, 0.0f); // bottom
	glColor3f(0.901f, 0.784f, 0.019f);
	glVertex3f(-0.2192f, -0.18398f, 0.0f); // bottom

	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(-0.2192f, 0.1615f, 0.0f); // top

	glEnd();

	/* O */
	glEnable(GL_SMOOTH);
	glPointSize(11);
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.01f)
	{
		float x, y;
		x = 0.12f * cos(angle);
		y = 0.14f * sin(angle);
		glVertex3f(x, y, 0.1f);
	}
	glEnd();

	/* G */
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.01f)
	{
		if (angle < 0.785f || angle > 5.713)
			continue;

		float x, y;

		x = (0.12f * cos(angle)) + 0.3348f;
		y = 0.14f * sin(angle);
		glVertex3f(x, y, 0.1f);
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(0.4241, -0.0479f, 0.0f);	 // top
	glVertex3f(0.3355f, -0.047f, 0.0f);	 // top
	glVertex3f(0.33515f, -0.006f, 0.0f); // top
	glVertex3f(0.4703f, -0.006f, 0.0f);	 // top
	glVertex3f(0.4714f, -0.0442f, 0.0f); // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.4242f, -0.0068f, 0.0f); // top
	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(0.4344f, -0.1864f, 0.0f); // bottom
	glVertex3f(0.471f, -0.1861f, 0.0f);	 // bottom
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(0.4703f, -0.006f, 0.0f); // top

	glEnd();

	/* H */
	glBegin(GL_QUADS);
	glVertex3f(0.6038f, 0.1624f, 0.0f); // top
	glVertex3f(0.5561f, 0.1623f, 0.0f); // top
	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(0.5569f, -0.1837f, 0.0f); // bottom
	glVertex3f(0.6043f, -0.1836f, 0.0f); // bottom
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(yellow[0], yellow[1], yellow[2]);
	glVertex3f(0.6028f, 0.0194f, 0.0f);	 // top
	glVertex3f(0.6033f, -0.0246f, 0.0f); // top
	glVertex3f(0.7625f, -0.0253f, 0.0f); // top
	glVertex3f(0.76f, 0.0202f, 0.0f);	 // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.7594f, 0.1632f, 0.0f); // top
	glVertex3f(0.81f, 0.1629f, 0.0f);	// top
	glColor3f(lavender[0], lavender[1], lavender[2]);
	glVertex3f(0.8102f, -0.1831f, 0.0f); // bottom
	glVertex3f(0.759f, -0.1838f, 0.0f);	 // bottom
	glEnd();
}

void drawShadowAmogh(void)
{

	glBegin(GL_QUADS);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7744f, 0.162f, 0.05f);   // top
	glVertex3f(-0.8866f, -0.1832f, 0.05f); // bottom
	glVertex3f(-0.839f, -0.1826f, 0.05f);  // bottom
	glVertex3f(-0.7202f, 0.1616f, 0.05f);  // top
	glEnd();

	// mid
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-0.8045f, -0.0769f, 0.05f);	// top
	glVertex3f(-0.6941f, -0.0765f, 0.05f);	// top
	glVertex3f(-0.79607f, -0.0361f, 0.05f); // top
	glVertex3f(-0.70203f, -0.0369f, 0.05f); // top

	glEnd();

	// right

	glBegin(GL_QUADS);
	glVertex3f(-0.6594f, -0.1838f, 0.05f); // bottom
	glVertex3f(-0.6103f, -0.1825f, 0.05f); // bottom
	glVertex3f(-0.7202f, 0.1616f, 0.05f);  // top
	glVertex3f(-0.7744f, 0.162f, 0.05f);   // top

	glEnd();

	/*   M  */
	glBegin(GL_QUADS);
	glVertex3f(-0.5513f, 0.1625f, 0.05f);  // top
	glVertex3f(-0.5509f, -0.1835f, 0.05f); // bottom
	glVertex3f(-0.5065f, -0.1704f, 0.05f); // bottom
	glVertex3f(-0.5065f, 0.1619f, 0.05f);  // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.5513f, 0.1625f, 0.05f);  // top
	glVertex3f(-0.4168f, -0.1842f, 0.05f); // bottom
	glVertex3f(-0.3579f, -0.1832f, 0.05f); // bottom
	glVertex3f(-0.47976f, 0.162f, 0.05f);  // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.41697f, -0.18417f, 0.05f); // bottom
	glVertex3f(-0.35967f, -0.18439f, 0.05f); // bottom
	glVertex3f(-0.2199f, 0.16233f, 0.05f);	 // top;
	glVertex3f(-0.2929f, 0.16233f, 0.05f);	 // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.274f, 0.1621f, 0.05f);	// top
	glVertex3f(-0.274f, -0.1846f, 0.05f);	// bottom
	glVertex3f(-0.2192f, -0.18398f, 0.05f); // bottom
	glVertex3f(-0.2192f, 0.1615f, 0.05f);	// top

	glEnd();

	/* O */
	glEnable(GL_SMOOTH);
	glPointSize(11);
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.01f)
	{
		float x, y;
		x = (0.12f * cos(angle)) + 0.01;
		y = 0.14f * sin(angle);
		glVertex3f(x, y, 0.05f);
	}
	glEnd();

	/* G */
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < PI * 2; angle = angle + 0.01f)
	{
		if (angle < 0.785f || angle > 5.713)
			continue;

		float x, y;

		x = (0.12f * cos(angle)) + 0.3500f;
		y = 0.14f * sin(angle);
		glVertex3f(x, y, 0.05f);
	}
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.4241, -0.0479f, 0.05f);  // top
	glVertex3f(0.3355f, -0.047f, 0.05f);  // top
	glVertex3f(0.33515f, -0.006f, 0.05f); // top
	glVertex3f(0.4703f, -0.006f, 0.05f);  // top
	glVertex3f(0.4714f, -0.0442f, 0.05f); // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.4242f, -0.0068f, 0.05f); // top
	glVertex3f(0.4344f, -0.1864f, 0.05f); // bottom
	glVertex3f(0.471f, -0.1861f, 0.05f);  // bottom
	glVertex3f(0.4703f, -0.006f, 0.05f);  // top

	glEnd();

	/* H */
	glBegin(GL_QUADS);
	glVertex3f(0.6038f, 0.1624f, 0.05f);  // top
	glVertex3f(0.5561f, 0.1623f, 0.05f);  // top
	glVertex3f(0.5569f, -0.1837f, 0.05f); // bottom
	glVertex3f(0.6043f, -0.1836f, 0.05f); // bottom
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.6028f, 0.0194f, 0.05f);  // top
	glVertex3f(0.6033f, -0.0246f, 0.05f); // top
	glVertex3f(0.7625f, -0.0253f, 0.05f); // top
	glVertex3f(0.76f, 0.0202f, 0.05f);	  // top
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0.7594f, 0.1632f, 0.05f);  // top
	glVertex3f(0.81f, 0.1629f, 0.05f);	  // top
	glVertex3f(0.8102f, -0.1831f, 0.05f); // bottom
	glVertex3f(0.759f, -0.1838f, 0.05f);  // bottom
	glEnd();
}

void toggleCollor()
{

	static int iTop = 0;	// flag for inner vertex
	static int iBottom = 0; // flag for outer vertex

	// for Inner Vertex ->
	if (iTop == YELLOW) // If Inner is Yellow Then Converting yellow[] array to Lavender
	{
		if (yellow[0] > 0.5f)
		{
			yellow[0] = yellow[0] - 0.01f;
		}
		if (yellow[1] > 0.5f)
		{
			yellow[1] = yellow[1] - 0.01f;
		}
		if (yellow[2] <= 1.0f)
		{
			yellow[2] = yellow[2] + 0.01f;

			if (yellow[2] >= 1.0f)
			{
				iTop = LAVENDER; // we are changing the flag here...because this if  will interate longer than other two
			}
		}
	}
	else // If yellow[] array is Lavender... converting yellow to lavender
	{
		if (yellow[0] < 1.0f)
		{
			yellow[0] = yellow[0] + 0.01f;
		}
		if (yellow[1] < 1.0f)
		{
			yellow[1] = yellow[1] + 0.01f;
		}
		if (yellow[2] >= 0.0f)
		{
			yellow[2] = yellow[2] - 0.01f;
			if (yellow[2] <= 0.0f)
			{
				iTop = YELLOW;
			}
		}
	}

	// for Outer Coordinate ->
	if (iBottom == LAVENDER) // If outer is lavender then converting lavender[] array to Yellow
	{
		if (lavender[0] < 1.0f)
		{
			lavender[0] = lavender[0] + 0.01f;
		}
		if (lavender[1] < 1.0f)
		{
			lavender[1] = lavender[1] + 0.01f;
		}
		if (lavender[2] >= 0.0f)
		{
			lavender[2] = lavender[2] - 0.01f;
			if (lavender[2] <= 0.0f)
			{
				iBottom = YELLOW;
			}
		}
	}
	else // if Outer is Yellow then converting Lavender[] array to -> lavender
	{
		if (lavender[0] > 0.5f)
		{
			lavender[0] = lavender[0] - 0.01f;
		}
		if (lavender[1] > 0.5f)
		{
			lavender[1] = lavender[1] - 0.01f;
		}
		if (lavender[2] <= 1.0f)
		{
			lavender[2] = lavender[2] + 0.01f;

			if (lavender[2] >= 1.0f)
			{
				iBottom = LAVENDER;
			}
		}
	}
}