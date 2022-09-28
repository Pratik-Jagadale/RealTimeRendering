/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <math.h>

/* OpenGL Header files */
#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include "vmath.h"
#include "Sphere.h"

using namespace vmath;

/* OpenGL libraries */
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Sphere.lib")

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
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>

// PP Related Global Variables
GLuint shaderProgramObject;

enum
{
	PRJ_ATRIBUTE_POSITION = 0,
	PRJ_ATRIBUTE_COLOR,
	PRJ_ATRIBUTE_NORMAL,
	PRJ_ATRIBUTE_TEXTURE0
};

GLuint gVao_sphere;			 // Vertex Array Object
GLuint gVbo_sphere_position; // Vertex Buffer Object
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

GLuint laUniform;			// light Ambiant
GLuint ldUniform;			// light Diffuse
GLuint lsUniform;			// light Spec
GLuint lighPositionUniform; // light Position

GLuint kaUniform; // material amb
GLuint kdUniform; // mat diff
GLuint ksUniform; // mat specular
GLuint materialShininessUniform;

GLuint lightingEnabledUniform;

mat4 perspectiveProjectionMatrix;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

BOOL bLight = FALSE;

GLfloat lightAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositions[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShinniness = 120.0f;

GLfloat angleForRotation = 0.0f;

GLint keyPressed = 0;

int currentWindowWidth = 0;
int currentWindowHeight = 0;

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	/* variable declarations */
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	int iHeightOfWindow, iWidthOfWindow;

	// Code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
	{
		MessageBox(NULL, TEXT("Creation of Log File Failed..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
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
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "GLEW Initialization Failed...\n");
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
		case 'f':
		case 'F':
			ToggleFullScreen();
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

		case 'x':
		case 'X':
			keyPressed = 1;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'y':
		case 'Y':
			keyPressed = 2;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'z':
		case 'Z':
			keyPressed = 3;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
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
	void printfGLInfo(void);
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
	printfGLInfo();

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_normal;"
		"uniform vec4 u_lightPosition;"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform int u_lightingEnabled;"
		"out vec3 transformedNormals;"
		"out vec3 lightDirection;"
		"out vec3 viewerVector;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
		"transformedNormals = normalize(normalMatrix * a_normal);"
		"viewerVector = normalize(-eyeCoordinates.xyz);"
		"lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" // Swizaling
		"}"
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char *log = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// fragment Shader

	status = 0;
	infoLogLength = 0;

	const GLchar *fragmentShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec3 transformedNormals;"
		"in vec3 lightDirection;"
		"in vec3 viewerVector;"
		"uniform vec3 u_la;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_ls;"
		"uniform vec4 u_lightPosition;"
		"uniform vec3 u_ka;"
		"uniform vec3 u_ks;"
		"uniform vec3 u_kd;"
		"uniform float u_materialShininnes;"
		"uniform int u_lightingEnabled;"
		"out vec4 FragColor;"
		"vec3 phong_ads_light;"
		"void main(void)"
		"{"
		"phong_ads_light = vec3(0.0,0.0,0.0);"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec3 ambiant;"
		"vec3 diffuse;"
		"vec3 reflectionVector;"
		"vec3 specular;"
		"ambiant = u_la * u_ka;"
		"diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);"
		"reflectionVector = reflect(-lightDirection,transformedNormals);"
		"specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininnes);"
		"phong_ads_light = phong_ads_light + ambiant + diffuse +  specular;"
		"}"
		"else"
		"{"
		"phong_ads_light = vec3(1.0,1.0,1.0);"
		"}"
		"FragColor = vec4(phong_ads_light, 1.0);"
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// Shader Program Object
	// pr
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);

	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_NORMAL, "a_normal");

	// link
	glLinkProgram(shaderProgramObject);

	status = 0;
	infoLogLength = 0;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG: %s \n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// post link - getting
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");

	laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
	ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
	lighPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

	lightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

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

	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);

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

	glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);

	return (0);
}

void printfGLInfo(void)
{
	// Local Variable declarations
	GLint numExtensions = 0;

	// Code
	fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR));					 // Graphic Card Compony
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));				 // Graphic Card
	fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION));				 // OpenGL Version
	fprintf(gpFile, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); // Graphic Library Shading Language Version

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	fprintf(gpFile, "Number of Supported Extensions: %d\n", numExtensions);

	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // to avoid devided by zero
		height = 1;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	currentWindowWidth = width;
	currentWindowHeight = height;
}

void display(void)
{
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	void draw24Sphere(void);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	draw24Sphere();

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */
	if (keyPressed == 1 || keyPressed == 2 || keyPressed == 3)
	{
		angleForRotation = angleForRotation + 1.0f;
		if (angleForRotation > 360.0f)
			angleForRotation = angleForRotation - 360.0f;

		// Set Light Zero Position - rotating  light
		GLfloat angle = (angleForRotation * M_PI) / 180.0f;
		GLfloat x = 10.0f * cos(angle);
		GLfloat y = 10.0f * sin(angle);

		if (keyPressed == 1)
		{
			lightPositions[1] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 2)
		{
			lightPositions[0] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 3)
		{
			lightPositions[0] = x;
			lightPositions[1] = y;
		}
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

	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
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

void draw24Sphere(void)
{
	// Variable Declartions
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	// Code

	int horDistance = currentWindowWidth / 6;
	int verDistance = currentWindowHeight / 7;

	scaleMatrix = vmath::scale(1.3f, 1.3f, 1.3f);

	// ***** 1st sphere on 1st column, emerald *****
	// translationMatrix = vmath::translate(-9.0f, 6.0f, -21.0f); // glTranslatef() is replaced by this line
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glViewport(0.0f, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0215; // r
	materialAmbiant[1] = 0.1745; // g
	materialAmbiant[2] = 0.0215; // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.633;	// r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;	// b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 2
	// ***** 2nd sphere on 1st column, jade *****
	//	glViewport(0.0f, (GLsizei)(WINHEIGHT) - (WINHEIGHT) / 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.135;	 // r
	materialAmbiant[1] = 0.2225; // g
	materialAmbiant[2] = 0.1575; // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// ***** 3rd sphere on 1st column, obsidian *****
	// 3
	// glViewport(0.0f, (GLsizei)(WINHEIGHT) - ((WINHEIGHT) / 2), (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.05375; // r
	materialAmbiant[1] = 0.05;	  // g
	materialAmbiant[2] = 0.06625; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;	  // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.3 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 4
	// ***** 4th sphere on 1st column, pearl *****
	// glViewport(0.0f, (GLsizei)(WINHEIGHT) - ((WINHEIGHT) / 1.3f), (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	// glViewport(0.0f, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, -0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.25;	  // r
	materialAmbiant[1] = 0.20725; // g
	materialAmbiant[2] = 0.20725; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 1.0;	// r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;	// a

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.088 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 5
	// ***** 5th sphere on 1st column, ruby *****
	glViewport(0.0f, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.1745;  // r
	materialAmbiant[1] = 0.01175; // g
	materialAmbiant[2] = 0.01175; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 6
	// ***** 6th sphere on 1st column, turquoise *****
	//	glViewport(0.0f, 0.0f, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(0.0f, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.1;	  // r
	materialAmbiant[1] = 0.18725; // g
	materialAmbiant[2] = 0.1745;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.396;	  // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;	// g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	// 6
	// glViewport(425.0f, 120 * 5.5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	materialAmbiant[0] = 0.329412; // r
	materialAmbiant[1] = 0.223529; // g
	materialAmbiant[2] = 0.027451; // b
	materialAmbiant[3] = 1.0f;	   // a

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;	   // a

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.21794872 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 7
	// ***** 2nd sphere on 2nd column, bronze *****
	// glViewport(425.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	glViewport(horDistance * 1.5, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.2125; // r
	materialAmbiant[1] = 0.1275; // g
	materialAmbiant[2] = 0.054;	 // b
	materialAmbiant[3] = 1.0f;	 // a

	// diffuse material
	materialDiffuse[0] = 0.714;	  // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.2 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 8
	// ***** 3rd sphere on 2nd column, chrome *****
	// glViewport(425.0f, 120 * 3.3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.25; // r
	materialAmbiant[1] = 0.25; // g
	materialAmbiant[2] = 0.25; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.6 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 9
	// ***** 4th sphere on 2nd column, copper *****
	// glViewport(425.0f, 120 * 2.2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.19125; // r
	materialAmbiant[1] = 0.0735;  // g
	materialAmbiant[2] = 0.0225;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.1 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 10
	// ***** 5th sphere on 2nd column, gold *****
	//	glViewport(425.0f, 120 * 1.1, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.24725; // r
	materialAmbiant[1] = 0.1995;  // g
	materialAmbiant[2] = 0.0745;  // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.4 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 11
	// ***** 6th sphere on 2nd column, silver *****
	// glViewport(425.0f, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 1.5, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.19225; // r
	materialAmbiant[1] = 0.19225; // g
	materialAmbiant[2] = 0.19225; // b
	materialAmbiant[3] = 1.0f;	  // a

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;	  // a

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;		// a

	// shininess
	materialShinniness = 0.4 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 12
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// glViewport(850.0f, 120 * 5.5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// ***** 2nd sphere on 3rd column, cyan *****
	// 13
	glViewport(850.0f, 120 * 4.4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.1;  // g
	materialAmbiant[2] = 0.06; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.0;		 // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;		 // a

	// specular material
	materialSpecular[0] = 0.50185078; // r
	materialSpecular[1] = 0.50185078; // g
	materialSpecular[2] = 0.50185078; // b
	materialSpecular[3] = 1.0f;		  // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 14
	// ***** 3rd sphere on 2nd column, green *****
	glViewport(horDistance * 3, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 15
	// ***** 4th sphere on 3rd column, red *****
	// glViewport(850.0f, 120 * 2.2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.6;	// g
	materialSpecular[2] = 0.6;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 16
	// ***** 5th sphere on 3rd column, white *****
	// glViewport(850.0f, 120 * 1.1, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));
	glViewport(horDistance * 3, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 17
	// ***** 6th sphere on 3rd column, yellow plastic *****
	glViewport(horDistance * 3, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.25 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 18
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	glViewport(horDistance * 4.5, verDistance * 5, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	materialAmbiant[0] = 0.02; // r
	materialAmbiant[1] = 0.02; // g
	materialAmbiant[2] = 0.02; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.4;	// r
	materialSpecular[1] = 0.4;	// g
	materialSpecular[2] = 0.4;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 19
	// ***** 2nd sphere on 4th column, cyan *****
	glViewport(horDistance * 4.5, verDistance * 4, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 20
	// ***** 3rd sphere on 4th column, green *****
	glViewport(horDistance * 4.5, verDistance * 3, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 21
	// ***** 4th sphere on 4th column, red *****
	glViewport(horDistance * 4.5, verDistance * 2, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 22
	// ***** 5th sphere on 4th column, white *****
	glViewport(horDistance * 4.5, verDistance, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// 23
	// ***** 6th sphere on 4th column, yellow rubber *****
	glViewport(horDistance * 4.5, 0, (GLsizei)(currentWindowWidth / 4), (GLsizei)(currentWindowHeight / 4));

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a

	// shininess
	materialShinniness = 0.078125 * 128;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(lsUniform, 1, lightSpecular);
		glUniform4fv(lighPositionUniform, 1, lightPositions);

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShinniness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
}
