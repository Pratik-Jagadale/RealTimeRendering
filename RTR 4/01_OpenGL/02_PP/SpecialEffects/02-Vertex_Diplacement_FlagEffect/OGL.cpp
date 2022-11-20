/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

/* OpenGL Header files */
#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include "vmath.h"

using namespace vmath;

/* OpenGL libraries */
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")

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

GLuint vao_Triangle;		  // Vertex Array Object - Triangle
GLuint vbo_Triangle_Position; // Vertex Buffer Object - Triangle - Position
GLuint vbo_Triangle_Color;	  // Vertex Buffer Object - Triangle
GLuint vao_Square;			  // Vertex Array Object - Square
GLuint vbo_Square_Position;	  // Vertex Buffer Object - Square- Position

GLuint vbo_Cube_Normal;
GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

GLuint timeUniform;
GLuint waveNumberUniform; // k
GLuint velocityUniform;
GLuint ampUniform;

GLuint lDUniform; //
GLuint kDUniform; //
GLuint lightPositionUniform;
GLuint lightingEnableUniform;

mat4 perspectiveProjectionMatrix;

GLfloat angleTriangle = 0.0f;
GLfloat angleSquare = 0.0f;

float numberOfWaves = ((6 * M_PI) / 3.0f); // k = ( 2PI / lambada) lamvda is distance between two waves
// float numberOfWaves = M_PI / 2;
float velocityOfWave = 16.5f;
float amplitudeOfWave = 0.2f; // height of wave

float timeincrementFactor = 0.001900f;
float numberOfWavesFactor = 2.5f;
float velocityOfWaveFactor = 2.5f;
float ampOfWaveFactor = 0.2f;

GLfloat radius = 8.0f;
float AngleCube = 270.0f;
float yEyeVector = 0;

GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPositions[] = {0.0f, 0.0f, 2.0f, 1.0f};

BOOL bLight = FALSE;

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
						  TEXT("OpenGL - Vertex Displacement"),
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
		fprintf(gpFile, "Makeing OpenGL as current Context Failed...\n");
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

		case 't':
			timeincrementFactor = timeincrementFactor + 0.0001f;
			fprintf(gpFile, "%f\n", timeincrementFactor);
			break;

		case 'T':
			timeincrementFactor = timeincrementFactor - 0.0001f;
			break;

		case 'v':
			velocityOfWaveFactor = velocityOfWaveFactor + 0.0001f;
			break;

		case 'V':
			velocityOfWaveFactor = velocityOfWaveFactor - 0.01f;
			break;

		case 'n':
			numberOfWavesFactor = numberOfWavesFactor + 0.01f;
			break;

		case 'N':
			numberOfWavesFactor = numberOfWavesFactor - 0.01f;
			break;

		case 'b':
			ampOfWaveFactor = ampOfWaveFactor + 0.01f;
			break;

		case 'B':
			ampOfWaveFactor = ampOfWaveFactor - 0.01f;
			fprintf(gpFile, "%f\n", ampOfWaveFactor);
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

float u_time = 0.0f;
SYSTEMTIME st;

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
		"in vec4 a_color;"
		"in vec3 a_normal;"

		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_kd;"
		"uniform vec4 u_lightPosition;"
		"uniform int u_lightingEnabled;"
		"out vec3 diffuse_light_color;"

		"uniform float Time;"
		"uniform float k;"		   // Wavenumber
		"uniform float Velocity ;" // Wave's velocity
		"uniform float Amp;"	   // Wave's amplitude

		"out vec4 a_color_out;"
		"void main(void)"
		"{"
		"vec4 pos = a_position;"
		"float u = k * (pos.x - Velocity * Time);"
		"pos.y= Amp * sin(u);"

		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinate = u_viewMatrix * u_modelMatrix * pos;"
		"mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));"
		"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
		"vec3 lightDirection = vec3(normalize(u_lightPosition - eyeCoordinate));"
		"diffuse_light_color = u_ld * u_kd * max(dot(lightDirection,transformedNormals),0.0);"
		"}"

		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * pos;"
		"a_color_out = a_color;"
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
		"in vec4 a_color_out;"
		"in vec3 diffuse_light_color;"
		"uniform int u_lightingEnabled;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"FragColor = vec4(diffuse_light_color, 1.0);"
		"}"
		"else"
		"{"
		"FragColor = a_color_out;"
		"}"
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
	// Binding Position Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");
	// Binding Color Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_COLOR, "a_color");

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

	timeUniform = glGetUniformLocation(shaderProgramObject, "Time");
	waveNumberUniform = glGetUniformLocation(shaderProgramObject, "k");
	velocityUniform = glGetUniformLocation(shaderProgramObject, "Velocity");
	ampUniform = glGetUniformLocation(shaderProgramObject, "Amp");

	lDUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	kDUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
	lightingEnableUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

	// vao_Triangle and vba related code
	// declartions of vertex Data array
	const GLfloat trianglePosition[] = {

		1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f

	};

	const GLfloat triangleColor[] = {
		1.0f, 0.0f, 0.0f, // RED
		0.0f, 1.0f, 0.0f, // BLUE
		0.0f, 0.0f, 0.1f, // GREEN,
		0.0f, 1.0f, 0.1f  // GREEN
	};

	const GLfloat SquarePosition[] = {

		1.0f, 1.0f, 0.0f,
		3.9f, 1.0f, 0.0f,    // 40
		3.9f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.8f, 1.0f, 0.0f,    
		3.8f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.7f, 1.0f, 0.0f,    
		3.7f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.6f, 1.0f, 0.0f,    
		3.6f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.5f, 1.0f, 0.0f,    
		3.5f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.4f, 1.0f, 0.0f,    
		3.4f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.3f, 1.0f, 0.0f,    
		3.3f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.2f, 1.0f, 0.0f,    
		3.2f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.1f, 1.0f, 0.0f,    
		3.1f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		3.0f, 1.0f, 0.0f,    
		3.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.9f, 1.0f, 0.0f,    // 30
		2.9f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.8f, 1.0f, 0.0f,
		2.8f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.7f, 1.0f, 0.0f,
		2.7f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.6f, 1.0f, 0.0f,
		2.6f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.5f, 1.0f, 0.0f,
		2.5f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.4f, 1.0f, 0.0f,
		2.4f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.3f, 1.0f, 0.0f,
		2.3f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.2f, 1.0f, 0.0f,
		2.2f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.1f, 1.0f, 0.0f,
		2.1f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		2.0f, 1.0f, 0.0f,
		2.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.9f, 1.0f, 0.0f,    // 20
		1.9f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.8f, 1.0f, 0.0f,
		1.8f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.7f, 1.0f, 0.0f,
		1.7f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.6f, 1.0f, 0.0f,
		1.6f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.5f, 1.0f, 0.0f,
		1.5f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.4f, 1.0f, 0.0f,
		1.4f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.3f, 1.0f, 0.0f,
		1.3f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.2f, 1.0f, 0.0f,
		1.2f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.1f, 1.0f, 0.0f,
		1.1f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		// -------------------------------
		1.0f, 1.0f, 0.0f,
		0.9f, 1.0f, 0.0f,    // 10
		0.9f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		0.9f, 1.0f, 0.0f,
		0.8f, 1.0f, 0.0f,
		0.8f, -1.0f, 0.0f,
		0.9f, -1.0f, 0.0f,

		0.8f, 1.0f, 0.0f,
		0.7f, 1.0f, 0.0f,
		0.7f, -1.0f, 0.0f,
		0.8f, -1.0f, 0.0f,

		0.7f, 0.0f, 1.0f,
		0.6f, 0.0f, 1.0f,
		0.6f, 0.0f, -1.0f,
		0.7f, 0.0f, -1.0f,

		0.6f, 0.0f, 1.0f,
		0.5f, 0.0f, 1.0f,
		0.5f, 0.0f, -1.0f,
		0.6f, 0.0f, -1.0f,

		0.5f, 0.0f, 1.0f,
		0.4f, 0.0f, 1.0f,
		0.4f, 0.0f, -1.0f,
		0.5f, 0.0f, -1.0f,

		0.4f, 0.0f, 1.0f,
		0.3f, 0.0f, 1.0f,
		0.3f, 0.0f, -1.0f,
		0.4f, 0.0f, -1.0f,

		0.3f, 0.0f, 1.0f,
		0.2f, 0.0f, 1.0f,
		0.2f, 0.0f, -1.0f,
		0.3f, 0.0f, -1.0f,

		0.2f, 0.0f, 1.0f,
		0.1f, 0.0f, 1.0f,
		0.1f, 0.0f, -1.0f,
		0.2f, 0.0f, -1.0f,

		0.1f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.1f, 0.0f, -1.0f};

	GLfloat cubeNormals[] = {
		// top surface
		0.0f, 1.0f, 0.0f, // top-right of top
		0.0f, 1.0f, 0.0f, // top-left of top
		0.0f, 1.0f, 0.0f, // bottom-left of top
		0.0f, 1.0f, 0.0f, // bottom-right of top
	};

	// vao and vbo related code
	// vao for Triangle
	glGenVertexArrays(1, &vao_Triangle);
	glBindVertexArray(vao_Triangle);

	// vbo for position
	glGenBuffers(1, &vbo_Triangle_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Triangle_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for color
	glGenBuffers(1, &vbo_Triangle_Color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Triangle_Color);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(0);

	// vao for Square
	glGenVertexArrays(1, &vao_Square);
	glBindVertexArray(vao_Square);

	// vbo for position
	glGenBuffers(1, &vbo_Square_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Square_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(SquarePosition), SquarePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	// vbo for Normal
	glGenBuffers(1, &vbo_Cube_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Normal);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for sqaure color
	glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 1.0f, 0.0f, 0.0f);

	glBindVertexArray(0); // ubind vao for Square

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		1000.0f);
}

void display(void)
{
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Triangle
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scalenMatrix = mat4::identity();

	vec3 eye = vec3(0.0f, 0.0f, 5.0f);
	vec3 center = vec3(0.0f, 0.0f, 0.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);

	float angle = AngleCube * M_PI / 180.0f;

	eye[0] = radius * cos(angle);
	eye[1] = yEyeVector;
	eye[2] = radius * sin(angle);

	viewMatrix = vmath::lookat(eye, center, up); // like gluLookAt()
												 /*
													 for (float i = -10.0f; i < 10.0f; i = i + 2.0f)
													 {
											 
														 translationMatrix = mat4::identity();
														 modelMatrix = mat4::identity();
														 modelViewProjectionMatrix = mat4::identity();
											 
														 translationMatrix = vmath::translate(i, 0.0f, -6.0f); // glTranslatef() is replaced by this line
														 // rotationMatrix = vmath::rotate(angleTriangle, 0.0f, 1.0f, 0.0f);
											 
														 modelMatrix = translationMatrix; // order is very important
											 
														 modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;
											 
														 glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
														 glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
														 glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
											 
														 // Sending Light Related uniforms
														 if (bLight == TRUE)
														 {
															 glUniform1i(lightingEnableUniform, 1);
															 glUniform3fv(lDUniform, 1, lightDiffuse);
															 glUniform3fv(kDUniform, 1, materialDiffuse);
															 glUniform4fv(lightPositionUniform, 1, lightPositions);
														 }
														 else
														 {
															 glUniform1i(lightingEnableUniform, 0);
														 }
														 // u_time = time(NULL);
											 
														 glUniform1f(timeUniform, u_time);
														 glUniform1f(waveNumberUniform, numberOfWavesFactor);
														 glUniform1f(velocityUniform, velocityOfWaveFactor);
														 glUniform1f(ampUniform, ampOfWaveFactor);
											 
														 glBindVertexArray(vao_Triangle);
											 
														 glDrawArrays(GL_TRIANGLE_FAN, 0, 16);
											 
														 glBindVertexArray(0);
													 }
												 */
	// Square
	// Tranformations
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f); // glTranslatef() is replaced by this line
	rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	scalenMatrix = vmath::scale(3.0f, 1.0f, 1.0f);
	// rotationMatrix = vmath::rotate(angleSquare, 1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scalenMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// Sending Light Related uniforms
	if (bLight == TRUE)
	{
		glUniform1i(lightingEnableUniform, 1);
		glUniform3fv(lDUniform, 1, lightDiffuse);
		glUniform3fv(kDUniform, 1, materialDiffuse);
		glUniform4fv(lightPositionUniform, 1, lightPositions);
	}
	else
	{
		glUniform1i(lightingEnableUniform, 0);
	}
	glUniform1f(timeUniform, u_time);
	glUniform1f(waveNumberUniform, numberOfWaves);
	glUniform1f(velocityUniform, velocityOfWave);
	glUniform1f(ampUniform, amplitudeOfWave);

	glBindVertexArray(vao_Square);

	// draw the desired graphics
	// drawing code -- magic

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 36, 4);   // 10

	glDrawArrays(GL_TRIANGLE_FAN, 40, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 44, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 48, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 52, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 56, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 60, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 64, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 68, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 72, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 76, 4);   // 20

	glDrawArrays(GL_TRIANGLE_FAN, 80, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 84, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 88, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 92, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 96, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 100, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 104, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 108, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 112, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 116, 4);  // 30
	
	glDrawArrays(GL_TRIANGLE_FAN, 120, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 124, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 128, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 132, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 136, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 140, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 144, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 148, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 152, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 156, 4);  // 40





	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */
	// angleTriangle = angleTriangle + 0.1f;
	if (angleTriangle >= 360.0f)
		angleTriangle = angleTriangle - 360.0f;

	// angleSquare = angleSquare + 0.1f;
	if (angleSquare >= 360.0f)
		angleSquare = angleSquare - 360.0f;

	u_time = u_time + timeincrementFactor;
	if (u_time > 10000.0f)
		u_time = 0.0f;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	// delete vbo_Square_Position
	if (vbo_Square_Position)
	{
		glDeleteBuffers(1, &vbo_Square_Position);
		vbo_Square_Position = 0;
	}

	// deletion of vao_Square
	if (vao_Square)
	{
		glDeleteVertexArrays(1, &vao_Square);
		vao_Square = 0;
	}

	// deletion of vbo_Triangle_Color
	if (vbo_Triangle_Color)
	{
		glDeleteBuffers(1, &vbo_Triangle_Color);
		vbo_Triangle_Color = 0;
	}

	// deletion of vbo_Triangle_Position
	if (vbo_Triangle_Position)
	{
		glDeleteBuffers(1, &vbo_Triangle_Position);
		vbo_Triangle_Position = 0;
	}

	// deletion of vao_Triangle
	if (vao_Triangle)
	{
		glDeleteVertexArrays(1, &vao_Triangle);
		vao_Triangle = 0;
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
