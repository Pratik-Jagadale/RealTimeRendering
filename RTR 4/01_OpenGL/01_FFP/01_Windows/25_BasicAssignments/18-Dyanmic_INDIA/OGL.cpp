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
#pragma comment(lib, "Winmm.lib")

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
void setColor4f(float r, float g, float b, float alpha);

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

	PlaySound(
		MAKEINTRESOURCE(MYMUSIC),
		GetModuleHandle(NULL),
		SND_RESOURCE | SND_ASYNC);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

	return (0);
}

// for lettres movements
float pos_I_ONE = -5.0f;
float pos_N = 9.0f;
float pos_D = 0.0f;
float pos_I_TWO = -9.0f;
float pos_A = 7.0f;

// for A "-" blend effect
float horizontalLine_A = 0.0f;

// for jet movements
float arpl_A = -10.5f;
float arpl_B = -10.5f;
float arpl_C = -10.5f;
float arpl_A_Y = 5.0f;
float arpl_C_Y = -5.0f;
float jetAngleA = 270.0f;
float jetAngleC = 90.0f;

// for jet booster
float y = 0.67f;
float y_one = 0.67f;
float y_TWO = 0.300f;

// for Z coordinate of all letters
float zCord = 0.0f;

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
	void drawIndia(void);
	void fighterJet(void);
	void drawIAF(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -12.0f);

	drawIndia();

	// Plane B
	glLoadIdentity();
	glTranslatef(0.0f + arpl_B, -0.1f, -12.0f);
	glScalef(0.30f, 0.30f, 0.30f);

	fighterJet();

	glLoadIdentity();
	glTranslatef(-0.8f + arpl_B, -0.1f, -12.0f);
	glScalef(0.15f, 0.15f, 0.15f);
	drawIAF();

	// Plane A
	glLoadIdentity();

	glTranslatef(-1.0f + arpl_A, 1.5f + arpl_A_Y, -12.0f);
	glRotatef(jetAngleA, 0.0f, 0.0f, 1.0f);
	glScalef(0.20f, 0.20f, 0.20f);

	fighterJet();

	glLoadIdentity();

	glTranslatef(-1.0f + arpl_A, 1.5f + arpl_A_Y, -12.0f);
	glRotatef(jetAngleA, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, 0.0f, 0.0f);
	glScalef(0.10f, 0.10f, 0.10f);
	drawIAF();

	// Plane C
	glLoadIdentity();
	glTranslatef(-1.0f + arpl_C, -1.65f + arpl_C_Y, -12.0f);
	glRotatef(jetAngleC, 0.0f, 0.0f, 1.0f);
	glScalef(0.20f, 0.20f, 0.20f);

	fighterJet();

	glLoadIdentity();
	glTranslatef(-1.0f + arpl_C, -1.65f + arpl_C_Y, -12.0f);
	glRotatef(jetAngleC, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, 0.0f, 0.0f);
	glScalef(0.10f, 0.10f, 0.10f);
	drawIAF();

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */
	y = y - 0.05;
	if (y < 0.300)
		y = 0.67f;

	y_one = y_one - 0.01;
	if (y_one < 0.300)
		y_one = 0.67f;

	y_TWO = y_TWO + 0.01;
	if (y_TWO > 0.67f)
		y_TWO = 0.300f;

	if (pos_I_ONE < 0.0f)
		pos_I_ONE = pos_I_ONE + 0.0015f;
	else if (pos_N > 0.0f)
		pos_N = pos_N - 0.001f;
	else if (pos_D < 255.0f)
		pos_D = pos_D + 0.1f;
	else if (pos_I_TWO < 0.0f)
		pos_I_TWO = pos_I_TWO + 0.0015f;
	else if (pos_A > 0.0f)
		pos_A = pos_A - 0.0015f;
	else if (arpl_B < 12.5f)
	{
		arpl_B = arpl_B + 0.001f;
		arpl_A = arpl_A + 0.001f;
		arpl_C = arpl_C + 0.001f;

		if (arpl_B < -4.0f)
		{
			if (arpl_A_Y > 0.0f)
				arpl_A_Y = arpl_A_Y - 0.001f;

			if (jetAngleA < 360.0f)
				jetAngleA = jetAngleA + 0.018f;

			if (jetAngleC > 0.0f)
				jetAngleC = jetAngleC - 0.018f;

			if (arpl_C_Y < 0.0f)
				arpl_C_Y = arpl_C_Y + 0.001;
		}
		else if (arpl_B > 7.5f)
		{

			if (arpl_A_Y < 5.0f)
				arpl_A_Y = arpl_A_Y + 0.001f;

			if (jetAngleA < 90.0f)
				jetAngleA = jetAngleA + 0.018f;
			else
				jetAngleA = 0.0f;

			if (jetAngleC > 270.0f)
				jetAngleC = jetAngleC - 0.018f;
			else
				jetAngleC = 360.0f;

			if (arpl_C_Y > -5.0)
				arpl_C_Y = arpl_C_Y - 0.001;
		}
		else if (arpl_B > 6.3f)
			horizontalLine_A = horizontalLine_A + 0.2f;
	}
	else
	{
		zCord = zCord + 0.001f;
	}
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

void drawIndia(void)
{
	// I
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(-8.0f + pos_I_ONE, 0.0f, 0.0f + zCord);

	glBegin(GL_QUADS);
	setColor(255, 153, 51);
	glVertex3f(-1.5f, 2.0f, 0.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 2.0f, 0.0f);

	setColor(19, 136, 8);
	glVertex3f(-1.5f, -2.0f, 0.0f);
	glVertex3f(-1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -2.0f, 0.0f);

	setColor(255, 153, 51);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	// 	N
	// glTranslatef(4.0f, 0.0f + pos_N, 0.0f);
	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(-4.0f, 0.0f + pos_N, -12.0f + zCord);

	glBegin(GL_QUADS);

	// "\"
	// bck
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	// bck
	setColor(19, 136, 8);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glVertex3f(2.0f, -2.0f, 0.0f);

	// frnt
	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(2.0f, -1.0f, 0.0f);

	// "| -" 1
	// bck
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// 2
	// bck
	setColor(19, 136, 8);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// fnt
	setColor(255, 255, 255);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	// "- |"
	// bcknd
	setColor(255, 153, 51);
	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);

	glVertex3f(2.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	// bkdn
	setColor(19, 136, 8);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glVertex3f(2.0f, -2.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(2.0f, -1.0f, 0.0f);

	glEnd();

	//	D
	//	glTranslatef(4.5f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(0.5f, 0.0f, -12.0f + zCord);

	glBegin(GL_QUADS);
	// bck
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// frnt
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// bck
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// frnt
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	// "-" TOP HORIZONTAL LINE
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 2.0f, 0.0f);

	// "_" BOTOM HORIZONTAL LINE
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -2.0f, 0.0f);

	//"-|" RIGHT VERTICAL LINE
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, 0.0f, 0.0f);

	setColor4f(255, 255, 255, pos_D);
	glVertex3f(1.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);

	// TOP TRAINGLE FOR D using QUAD
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);

	// BOTTOM TRAINGLE FOR D using QUAD
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(0.5f, -2.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

	// I
	//	glTranslatef(3.3f, 0.0f + pos_I_TWO, 0.0f);

	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(3.8f, 0.0f + pos_I_TWO, -12.0f + zCord);

	glBegin(GL_QUADS);
	setColor(255, 153, 51);

	glVertex3f(-1.5f, 2.0f, 0.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 2.0f, 0.0f);

	setColor(19, 136, 8);
	glVertex3f(-1.5f, -2.0f, 0.0f);
	glVertex3f(-1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -2.0f, 0.0f);

	setColor(255, 153, 51);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	// A
	//	glTranslatef(3.6f + pos_A, 0.0f, 0.0f);
	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(7.4f + pos_A, 0.0f, -12.0f + zCord);

	// " - "
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	setColor4f(255, 153, 51, horizontalLine_A);
	glVertex3f(0.25f, 0.0f + 0.4f, 0.0f);
	glVertex3f(-0.25f, 0.0f + 0.4f, 0.0f);
	setColor4f(255, 2555, 255, horizontalLine_A);
	glVertex3f(-0.45f, -0.5f + 0.4f, 0.0f);
	glVertex3f(0.45f, -0.5f + 0.4f, 0.0f);

	glVertex3f(0.45f, -0.5f + 0.4f, 0.0f);
	glVertex3f(-0.45f, -0.5f + 0.4f, 0.0f);
	setColor4f(152, 251, 152, horizontalLine_A);
	glVertex3f(-0.65f, -1.0f + 0.4f, 0.0f);
	glVertex3f(0.65f, -1.0f + 0.4f, 0.0f);

	glEnd();
	glDisable(GL_BLEND);

	glBegin(GL_QUADS);
	// "/"
	setColor4f(255, 153, 51, 255);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 2.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-1.25f, 0.0f, 0.0f);
	glVertex3f(-0.25f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(-0.25f, 0.0f, 0.0f);
	glVertex3f(-1.25f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// "\"
	setColor(255, 153, 51);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 2.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(0.25f, 0.0f, 0.0f);
	glVertex3f(1.25f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.25f, 0.0f, 0.0f);
	glVertex3f(1.25f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(2.0f, -2.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glEnd();
}

void fighterJet(void)
{

	// top nitro
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3);
	glBegin(GL_LINES);
	setColor4f(255, 153, 51, 255.0f);
	glVertex3f(-4.20, y, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y, 0.0f);

	setColor4f(255, 153, 51, 255.5f);
	glVertex3f(-4.200, y_one, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y_one, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.200, y_TWO, 0.0f);
	setColor4f(19, 136, 8, 0.5f);
	glVertex3f(-8.0, y_TWO, 0.0f);

	glEnd();

	// bottom nitro
	glBegin(GL_LINES);
	setColor4f(255, 153, 51, 255.5f);
	glVertex3f(-4.200, -y_one, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, -y_one, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.20, y - 0.9f, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y - 0.9f, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.20, y_one - 0.9f, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y_one - 0.9f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

	///////////////// top wing
	glBegin(GL_POLYGON);
	setColor(186, 226, 238);
	// setColor(0, 212, 250);
	glVertex3f(1.36f, 0.760f, 0.0f);
	glVertex3f(0.6f, 1.597f, 0.0f);
	glVertex3f(0.24f, 1.613f, 0.0f);
	glVertex3f(0.56f, 0.818f, 0.0f);
	glEnd();

	// bottom wing
	glBegin(GL_POLYGON);
	glVertex3f(1.36, -0.733, 0.0f);
	glVertex3f(0.443, -0.9, 0.0f);
	glVertex3f(0.251, -1.565, 0.0f);
	glVertex3f(0.568, -1.5664, 0.0f);
	glEnd();

	////////////////
	// tip
	glBegin(GL_POLYGON);
	glVertex3f(4.394, 0.014, 0.0f);
	glVertex3f(3.618, 0.224, 0.0f);
	glVertex3f(2.962, 0.331, 0.0f);
	glVertex3f(2.175, 0.5325, 0.0f);
	glVertex3f(1.512, 0.640, 0.0f);
	glVertex3f(1.138, 0.650, 0.0f);
	glVertex3f(1.138, -0.650, 0.0f);
	glVertex3f(1.437, -0.640, 0.0f);
	glVertex3f(1.726, -0.600, 0.0f);
	glVertex3f(2.423, -0.404, 0.0f);
	glVertex3f(2.823, -0.312, 0.0f);
	glVertex3f(3.513, -0.206, 0.0f);
	glVertex3f(3.931, -0.125, 0.0f);
	glVertex3f(4.39, 0.008000, 0.0f);
	glEnd();

	// back
	glBegin(GL_POLYGON);
	glVertex3f(-0.256, 1.155, 0.0f);
	glVertex3f(-2.242, 3.127, 0.0f);
	glVertex3f(-2.279, 3.1546, 0.0f);
	glVertex3f(-3.194, 3.183, 0.0f);
	glVertex3f(-3.351, 0.93, 0.0f);
	glVertex3f(-3.7883, 0.9295, 0.0f);
	glVertex3f(-3.7955, 0.7366, 0.0f);
	glVertex3f(-4.1017, 0.672, 0.0f);
	glVertex3f(-4.098, 0.2597, 0.0f);
	glVertex3f(-3.808, 0.211, 0.0f);
	glVertex3f(-4.091, 0.193, 0.0f);
	glVertex3f(-4.093, 0.141, 0.0f);
	glVertex3f(-4.248, 0.1155, 0.0f);
	glVertex3f(-4.258, 0.078, 0.0f);
	glVertex3f(-4.732, 0.0725, 0.0f);
	glVertex3f(-4.74, -0.026, 0.0f);
	glVertex3f(-4.267, -0.0294, 0.0f);
	glVertex3f(-4.2404, -0.0754, 0.0f);
	glVertex3f(-4.0995, -0.092, 0.0f);
	glVertex3f(-4.0966, -0.1646, 0.0f);
	glVertex3f(-3.8896, -0.1804, 0.0f);
	glVertex3f(-4.096, -0.229, 0.0f);
	glVertex3f(-4.0976, -0.642, 0.0f);
	glVertex3f(-3.798, -0.6916, 0.0f);
	glVertex3f(-3.7935, -0.884, 0.0f);
	glVertex3f(-3.348, -0.887, 0.0f);
	glVertex3f(-3.189, -3.0945, 0.0f);
	glVertex3f(-2.2505, -3.109, 0.0f);
	glVertex3f(-0.256, -1.11300, 0.0f);
	glEnd();

	// middle
	glBegin(GL_POLYGON);
	glVertex3f(1.39, 0.76, 0.0f);
	glVertex3f(-0.256, 1.155, 0.0f);
	glVertex3f(-0.256, -1.11300, 0.0f);
	glVertex3f(1.39, -0.733, 0.0f);

	glEnd();

	glLineWidth(1);

	glBegin(GL_LINE_LOOP);
	setColor(0, 0, 0);
	glVertex3f(2.972, 0.15500, 0.0f);
	glVertex3f(2.276, 0.283, 0.0f);
	glVertex3f(0.772, 0.298, 0.0f);
	glVertex3f(0.651, 0.21, 0.0f);
	glVertex3f(0.563, 0.112, 0.0f);
	glVertex3f(0.552, -0.034, 0.0f);
	glVertex3f(0.627, -0.16, 0.0f);
	glVertex3f(0.778, -0.264, 0.0f);
	glVertex3f(2.21, -0.253, 0.0f);
	glVertex3f(2.979, -0.121, 0.0f);
	glVertex3f(3.007, 0, 0.0f);
	glEnd();

	setColor(0, 0, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.556, 0.803, 0.0f);
	glVertex3f(-0.26, 1.15, 0.0f);
	glVertex3f(-2.826, 1.143, 0.0f);
	glVertex3f(-2.82, 0.94, 0.0f);
	glVertex3f(-3.788, 0.918, 0.0f);
	glVertex3f(-3.788, -0.87, 0.0f);
	glVertex3f(-2.81, -0.888, 0.0f);
	glVertex3f(-2.804, -1.109, 0.0f);
	glVertex3f(-0.242, -1.106, 0.0f);
	glVertex3f(0.569, -0.77, 0.0f);

	glEnd();

	glLineWidth(10);
	glBegin(GL_LINES);
	setColor(186, 226, 238);
	glVertex3f(-1.754, 3.2, 0.0f);
	glVertex3f(-3.379, 3.2, 0.0f);

	glVertex3f(-1.754 + 1.0f, 3.2 - 1.0f, 0.0f);
	glVertex3f(-3.379 + 1.0f, 3.2 - 1.0f, 0.0f);

	glVertex3f(-1.754, -3.1, 0.0f);
	glVertex3f(-3.379, -3.1, 0.0f);

	glVertex3f(-1.754 + 1.0f, -3.1 + 1.0f, 0.0f);
	glVertex3f(-3.379 + 1.0f, -3.1 + 1.0f, 0.0f);

	glEnd();

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);

	glVertex3f(-2.803, 3.128, 0.0f);
	glVertex3f(-3.14, 3.132, 0.0f);
	glVertex3f(-3.308, 0.926, 0.0f);
	glVertex3f(-2.808, 0.932, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glVertex3f(-2.803, -3.128, 0.0f);
	glVertex3f(-3.14, -3.132, 0.0f);
	glVertex3f(-3.308, -0.926, 0.0f);
	glVertex3f(-2.808, -0.932, 0.0f);
	glEnd();

	glScalef(0.75f, 0.75f, 0.75f);
	glTranslatef(0.3f, 0.0f, 0.0f);

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	setColor(0, 212, 250);
	glVertex3f(2.972, 0.15500, 0.0f);
	glVertex3f(2.276, 0.283, 0.0f);
	glVertex3f(0.772, 0.298, 0.0f);
	glVertex3f(0.651, 0.21, 0.0f);
	glVertex3f(0.563, 0.112, 0.0f);
	glVertex3f(0.552, -0.034, 0.0f);
	glVertex3f(0.627, -0.16, 0.0f);
	glVertex3f(0.778, -0.264, 0.0f);
	glVertex3f(2.21, -0.253, 0.0f);
	glVertex3f(2.979, -0.121, 0.0f);
	glVertex3f(3.007, 0, 0.0f);
	glEnd();
}

void drawIAF(void)
{
	void drawI(void);
	void drawA(void);
	void drawF(void);

	glLineWidth(3);
	drawI();
	glTranslatef(2.2f, 0.0f, 0.0f);
	drawA();

	glTranslatef(2.5f, 0.0f, 0.0f);
	drawF();
}

void drawI(void)
{
	glBegin(GL_LINES);
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}
void drawA(void)
{
	glBegin(GL_LINES);
	setColor(255, 255, 255);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();
}

void drawF(void)
{
	glBegin(GL_LINES);
	setColor(19, 136, 8);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}

void setColor(float r, float g, float b)
{
	glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void setColor4f(float r, float g, float b, float alpha)
{
	glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f);
}
