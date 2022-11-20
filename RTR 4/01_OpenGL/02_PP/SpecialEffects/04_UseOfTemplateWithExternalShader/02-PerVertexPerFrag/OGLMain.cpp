/* Header Files */
#include <windows.h>
#include "OGLMain.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include "vmath.h"
#include "GLShaders.h"
#include "GLLog.h"
#include "Sphere.h"

#pragma comment(lib, "Sphere.lib")

using namespace vmath;

#define WINWIDTH 800
#define WINHEIGHT 600

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;

// PP Related Global Variables
GLuint shaderProgramObject_PV;
GLuint shaderProgramObject_PF;

GLuint gVao_sphere;			 // Vertex Array Object
GLuint gVbo_sphere_position; // Vertex Buffer Object
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint modelMatrixUniform_PV;
GLuint viewMatrixUniform_PV;
GLuint projectionMatrixUniform_PV;

GLuint laUniform_PV;		   // light Ambiant
GLuint ldUniform_PV;		   // light Diffuse
GLuint lsUniform_PV;		   // light Spec
GLuint lighPositionUniform_PV; // light Position

GLuint kaUniform_PV; // material amb
GLuint kdUniform_PV; // mat diff
GLuint ksUniform_PV; // mat specular
GLuint materialShininessUniform_PV;

GLuint lightingEnabledUniform_PV;

GLuint modelMatrixUniform_PF;
GLuint viewMatrixUniform_PF;
GLuint projectionMatrixUniform_PF;

GLuint laUniform_PF;		   // light Ambiant
GLuint ldUniform_PF;		   // light Diffuse
GLuint lsUniform_PF;		   // light Spec
GLuint lighPositionUniform_PF; // light Position

GLuint kaUniform_PF; // material amb
GLuint kdUniform_PF; // mat diff
GLuint ksUniform_PF; // mat specular
GLuint materialShininessUniform_PF;

GLuint lightingEnabledUniform_PF;

mat4 perspectiveProjectionMatrix;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

BOOL bLight = FALSE;
BOOL bPerFragShader = FALSE;

GLfloat lightAmbiant[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositions[] = {100.0f, 100.0f, 100.0f, 1.0f};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat meterialDeffuse[] = {0.5f, 0.2f, 0.7f, 1.0f};
GLfloat materialSpecular[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat materialShineeness = 120.0f;

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);

	/* variable declarations */
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	int iHeightOfWindow, iWidthOfWindow;

	// Code
	CreateLogFile();

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
						  TEXT("OpenGL -  Pratik Rajendra Jagadale"),
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
		PrintLog("Choose Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		PrintLog("Set Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		PrintLog("Crete OpenGl Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -4)
	{
		PrintLog("Makeing OpenGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		PrintLog("GLEW Initialization Failed...\n");
		uninitialize();
	}
	else
	{
		PrintLog("Initialize Successfull...\n");
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
	// void ToggleFullScreen();
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

		case 'p':
		case 'P':
			if (bPerFragShader == FALSE)
			{
				bPerFragShader = TRUE;
			}
			else
			{
				bPerFragShader = FALSE;
			}
			break;

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
			}
			break;

		case 'f':
		case 'F':
			ToggleFullScreen();
			break;
		case 27:
			PostQuitMessage(0);
		}
		break;

	case WM_SIZE:
		resize(WORD(lParam), HIWORD(lParam));
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
	void uninitialize(void);

	/* variable declartions */
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	/* code */
	/* initialization of pixelformatdesciptor structure */
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // memset((void*)&pfd , NULL, sizeof(OIXELFORAMTEDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	/* GetDC */
	ghdc = GetDC(ghwnd);

	/* Choose Pixel Format */
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
		return -1;

	/* Set The choosen Puxel Format */
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return -2;

	/* binding API */
	/* Create OpenGL Rendering Context */
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return -3;

	/* make the rendering as current cintext */
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return -4;

	/* Here start OpeGL Code */
	// GLEW INITIALIZATION
	if (glewInit() != GLEW_OK)
		return -5;

	// Print OpenGL Info
	PrintGLInfo();

	// vertex Shader - PerVertex
	GLuint vertexShaderObject_PV = CreateAndCompileShaderObjects("Shaders\\PerVertex.vs", VERTEX);

	// fragment Shader - PerVertex
	GLuint fragmentShaderObject_PV = CreateAndCompileShaderObjects("Shaders\\PerVertex.fs", FRAGMENT);

	// vertex Shader - PerFragment
	GLuint vertexShaderObject_PF = CreateAndCompileShaderObjects("Shaders\\PerFrag.vs", VERTEX);

	// fragment Shader - Perfragment
	GLuint fragmentShaderObject_PF = CreateAndCompileShaderObjects("Shaders\\PerFrag.fs", FRAGMENT);

	// Shader Program Object - Per Vertex
	shaderProgramObject_PV = glCreateProgram();
	glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);
	glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

	// Shader Program Object - Per Fragment
	shaderProgramObject_PF = glCreateProgram();
	glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
	glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject_PV, PVG_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject_PV, PVG_ATTRIBUTE_NORMAL, "a_normal");

	glBindAttribLocation(shaderProgramObject_PF, PVG_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject_PF, PVG_ATTRIBUTE_NORMAL, "a_normal");

	// link
	BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_PF);
	bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_PV);

	// post link - getting
	// Perver - Post link
	modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_modelMatrix");
	viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
	projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");

	laUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_la");
	ldUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ld");
	lsUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ls");
	lighPositionUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition");

	kaUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ka");
	kdUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_kd");
	ksUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ks");

	materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_materialShininnes");

	lightingEnabledUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");

	// PerFrag - Post link
	modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_modelMatrix");
	viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
	projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");

	laUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_la");
	ldUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ld");
	lsUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ls");
	lighPositionUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition");

	kaUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ka");
	kdUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_kd");
	ksUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ks");

	materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_materialShininnes");

	lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");

	// gVao_sphere and vba related code
	// declartions of vertex Data array
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao vbo reelated code
	// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// position vbo
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(PVG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(PVG_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);

	return (0);
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // to avoid devided by zero
		height = 1;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}
void display(void)
{
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	if (bPerFragShader == FALSE)
	{
		glUseProgram(shaderProgramObject_PV);
	}
	else
	{
		glUseProgram(shaderProgramObject_PF);
	}
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;
	if (bPerFragShader == FALSE)
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{

			glUniform1i(lightingEnabledUniform_PV, 1);

			glUniform3fv(laUniform_PV, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_PV, 1, lightDiffuse);
			glUniform3fv(lsUniform_PV, 1, lightSpecular);
			glUniform4fv(lighPositionUniform_PV, 1, lightPositions);

			glUniform3fv(kaUniform_PV, 1, materialAmbiant);
			glUniform3fv(kdUniform_PV, 1, meterialDeffuse);
			glUniform3fv(ksUniform_PV, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PV, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PV, 0);
		}
	}
	else
	{
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{

			glUniform1i(lightingEnabledUniform_PF, 1);

			glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_PF, 1, lightDiffuse);
			glUniform3fv(lsUniform_PF, 1, lightSpecular);
			glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

			glUniform3fv(kaUniform_PF, 1, materialAmbiant);
			glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
			glUniform3fv(ksUniform_PF, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PF, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PF, 0);
		}
	}

	// draw the desired graphics
	// drawing code -- magic

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// glDrawArrays(GL_TRIANGLES, 0, gNumElements);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

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
	// deletion of gVbo_sphere_element
	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

	// deletion of gVbo_sphere_normal
	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_normal);
		gVbo_sphere_normal = 0;
	}

	// deletion of gVbo_sphere_Position
	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	// deletion of gVao_sphere
	if (gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
	}

	UninitializeShaders(shaderProgramObject_PF);
	UninitializeShaders(shaderProgramObject_PV);

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

	CloseLogFile();
}
