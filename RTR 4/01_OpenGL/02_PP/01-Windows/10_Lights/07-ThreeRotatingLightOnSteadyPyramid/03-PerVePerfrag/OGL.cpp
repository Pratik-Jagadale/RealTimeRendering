/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>

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
GLuint shaderProgramObject_PF;
GLuint shaderProgramObject_PV;

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

GLuint modelMatrixUniform_PF;
GLuint viewMatrixUniform_PF;
GLuint projectionMatrixUniform_PF;

GLuint modelMatrixUniform_PV;
GLuint viewMatrixUniform_PV;
GLuint projectionMatrixUniform_PV;

mat4 perspectiveProjectionMatrix;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

// For Three Light on Sphere
GLuint laUniform_PF[3];			  // light Ambiant
GLuint ldUniform_PF[3];			  // light Diffuse
GLuint lsUniform_PF[3];			  // light Spec
GLuint lighPositionUniform_PF[3]; // light Position

GLuint kaUniform_PF; // material amb
GLuint kdUniform_PF; // mat diff
GLuint ksUniform_PF; // mat specular
GLuint materialShininessUniform_PF;

GLuint lightingEnabledUniform_PF;

// For Three Light on Sphere
GLuint laUniform_PV[3];			  // light Ambiant
GLuint ldUniform_PV[3];			  // light Diffuse
GLuint lsUniform_PV[3];			  // light Spec
GLuint lighPositionUniform_PV[3]; // light Position

GLuint kaUniform_PV; // material amb
GLuint kdUniform_PV; // mat diff
GLuint ksUniform_PV; // mat specular
GLuint materialShininessUniform_PV;

GLuint lightingEnabledUniform_PV;

BOOL bLight;

struct Light
{
	vmath::vec4 lightAmbiant;
	vmath::vec4 lightDiffuse;
	vmath::vec4 lightSpecular;
	vmath::vec4 lightPositions;
};

struct Light lights[3];

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat meterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShineeness = 124.0f;

GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 150.0f;
GLfloat lightAngleZero = 300.0f;

BOOL isPerFrag = FALSE;

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

		case 'p':
		case 'P':
			if (isPerFrag == FALSE)
			{
				isPerFrag = TRUE;
			}
			else
			{
				isPerFrag = FALSE;
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

		case 27:
			DestroyWindow(hwnd);
			break;
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

	{

		// vartex Shader
		const GLchar *vertexShaderSourceCode_PF =
			"#version 460 core"
			"\n"
			"in vec4 a_position;"
			"in vec3 a_normal;"
			"uniform vec4 u_lightPosition[3];"
			"uniform mat4 u_modelMatrix;"
			"uniform mat4 u_viewMatrix;"
			"uniform mat4 u_projectionMatrix;"
			"uniform int u_lightingEnabled;"
			"out vec3 transformedNormals;"
			"out vec3 lightDirection[3];"
			"out vec3 viewerVector;"
			"void main(void)"
			"{"
			"if(u_lightingEnabled == 1)"
			"{"
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
			"transformedNormals = normalize(normalMatrix * a_normal);"
			"viewerVector = normalize(-eyeCoordinates.xyz);"
			"for(int i = 0 ; i < 3 ; i++)"
			"{"
			"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" // Swizaling
			"}"
			"}"
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
			"}";

		GLuint vertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShaderObject_PF, 1, (const GLchar **)&vertexShaderSourceCode_PF, NULL);

		glCompileShader(vertexShaderObject_PF);

		GLint status;
		GLint infoLogLength;
		char *log = NULL;
		glGetShaderiv(vertexShaderObject_PF, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(vertexShaderObject_PF, infoLogLength, &written, log);
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

		const GLchar *fragmentShaderSourceCode_PF =
			"#version 460 core"
			"\n"
			"in vec3 transformedNormals;"
			"in vec3 lightDirection[3];"
			"in vec3 viewerVector;"
			"uniform vec3 u_la[3];"
			"uniform vec3 u_ld[3];"
			"uniform vec3 u_ls[3];"
			"uniform vec4 u_lightPosition[3];"
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
			"vec3 ambiant[3];"
			"vec3 diffuse[3];"
			"vec3 reflectionVector[3];"
			"vec3 specular[3];"
			"for(int i = 0 ; i < 3 ; i++)"
			"{"
			"ambiant[i] = u_la[i] * u_ka;"
			"diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);"
			"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
			"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);"
			"phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];"
			"}"
			"}"
			"else"
			"{"
			"phong_ads_light = vec3(1.0,1.0,1.0);"
			"}"
			"FragColor = vec4(phong_ads_light, 1.0);"
			"}";

		GLuint fragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShaderObject_PF, 1, (const GLchar **)&fragmentShaderSourceCode_PF, NULL);

		glCompileShader(fragmentShaderObject_PF);

		glGetShaderiv(fragmentShaderObject_PF, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(fragmentShaderObject_PF, infoLogLength, &written, log);
					fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG : %s\n", log);
					free(log);
					log = NULL;
					uninitialize();
				}
			}
		}

		// Shader Program Object
		// pr
		shaderProgramObject_PF = glCreateProgram();
		glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);

		glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

		// prelinked binding
		glBindAttribLocation(shaderProgramObject_PF, PRJ_ATRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject_PF, PRJ_ATRIBUTE_NORMAL, "a_normal");

		// link
		glLinkProgram(shaderProgramObject_PF);

		status = 0;
		infoLogLength = 0;

		glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength, &written, log);
					fprintf(gpFile, "SHADER PROGRAM LINK LOG: %s \n", log);
					free(log);
					log = NULL;
					uninitialize();
				}
			}
		}

		// post link - getting
		modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_modelMatrix");
		viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
		projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");

		laUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_la[0]");
		ldUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[0]");
		lsUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[0]");
		lighPositionUniform_PF[0] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[0]");

		laUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_la[1]");
		ldUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[1]");
		lsUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[1]");
		lighPositionUniform_PF[1] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[1]");

		laUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_la[2]");
		ldUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_ld[2]");
		lsUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_ls[2]");
		lighPositionUniform_PF[2] = glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition[2]");

		kaUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ka");
		kdUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_kd");
		ksUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_ks");
		materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_materialShininnes");

		lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");
	}

	{
		// vartex Shader
		const GLchar *vertexShaderSourceCode_PV =
			"#version 460 core"
			"\n"
			"in vec4 a_position;"
			"in vec3 a_normal;"
			"uniform mat4 u_modelMatrix;"
			"uniform mat4 u_viewMatrix;"
			"uniform mat4 u_projectionMatrix;"
			"uniform vec3 u_la[3];"
			"uniform vec3 u_ld[3];"
			"uniform vec3 u_ls[3];"
			"uniform vec4 u_lightPosition[3];"
			"uniform vec3 u_ka;"
			"uniform vec3 u_ks;"
			"uniform vec3 u_kd;"
			"uniform float u_materialShininnes;"
			"uniform int u_lightingEnabled;"
			"out vec3 phong_ads_light;"
			"void main(void)"
			"{"
			"phong_ads_light = vec3(0.0,0.0,0.0);"
			"if(u_lightingEnabled == 1)"
			"{"
			"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
			"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
			"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);"
			"vec3 ambiant[3];"
			"vec3 lightDirection[3];"
			"vec3 diffuse[3];"
			"vec3 reflectionVector[3];"
			"vec3 specular[3];"
			"for(int i = 0 ; i < 3 ; i++)"
			"{"
			"ambiant[i] = u_la[i] * u_ka;"
			"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" // Swizaling
			"diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);"
			"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
			"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);"
			"phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];"
			"}"
			"}"
			"else"
			"{"
			"phong_ads_light = vec3(1.0,1.0,1.0);"
			"}"
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
			"}";

		GLuint vertexShaderObject_PV = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShaderObject_PV, 1, (const GLchar **)&vertexShaderSourceCode_PV, NULL);

		glCompileShader(vertexShaderObject_PV);

		GLint status;
		GLint infoLogLength;
		char *log = NULL;
		glGetShaderiv(vertexShaderObject_PV, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(vertexShaderObject_PV, infoLogLength, &written, log);
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

		const GLchar *fragmentShaderSourceCode_PV =
			"#version 460 core"
			"\n"
			"in vec3 phong_ads_light;"
			"out vec4 FragColor;"
			"void main(void)"
			"{"
			"FragColor = vec4(phong_ads_light, 1.0);"
			"}";

		GLuint fragmentShaderObject_PV = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShaderObject_PV, 1, (const GLchar **)&fragmentShaderSourceCode_PV, NULL);

		glCompileShader(fragmentShaderObject_PV);

		glGetShaderiv(fragmentShaderObject_PV, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(fragmentShaderObject_PV, infoLogLength, &written, log);
					fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG : %s\n", log);
					free(log);
					log = NULL;
					uninitialize();
				}
			}
		}

		// Shader Program Object
		// pr
		shaderProgramObject_PV = glCreateProgram();
		glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);

		glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

		// prelinked binding
		glBindAttribLocation(shaderProgramObject_PV, PRJ_ATRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject_PV, PRJ_ATRIBUTE_NORMAL, "a_normal");

		// link
		glLinkProgram(shaderProgramObject_PV);

		status = 0;
		infoLogLength = 0;

		glGetProgramiv(shaderProgramObject_PV, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			glGetProgramiv(shaderProgramObject_PV, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				log = (char *)malloc(infoLogLength);
				if (log != NULL)
				{
					GLsizei written;
					glGetProgramInfoLog(shaderProgramObject_PV, infoLogLength, &written, log);
					fprintf(gpFile, "SHADER PROGRAM LINK LOG: %s \n", log);
					free(log);
					log = NULL;
					uninitialize();
				}
			}
		}

		// post link - getting
		modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_modelMatrix");
		viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
		projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");

		laUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_la[0]");
		ldUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[0]");
		lsUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[0]");
		lighPositionUniform_PV[0] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[0]");

		laUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_la[1]");
		ldUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[1]");
		lsUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[1]");
		lighPositionUniform_PV[1] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[1]");

		laUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_la[2]");
		ldUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_ld[2]");
		lsUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_ls[2]");
		lighPositionUniform_PV[2] = glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition[2]");

		kaUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ka");
		kdUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_kd");
		ksUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_ks");
		materialShininessUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_materialShininnes");

		lightingEnabledUniform_PV = glGetUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
	}

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

	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	lights[0].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights[1].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[1].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[1].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights[2].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2].lightDiffuse = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightSpecular = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights[2].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

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
		100.0f);
}

void display(void)
{
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// use shader program obejct
	if (isPerFrag)
		glUseProgram(shaderProgramObject_PF);
	else
		glUseProgram(shaderProgramObject_PV);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	// Light Related Code
	// Set Light Zero Position - rotating zero light - x Around
	float angle = lightAngleZero * (M_PI / 180.0f);
	float x = 5.0f * cos(angle);
	float y = 5.0f * sin(angle);
	lights[0].lightPositions[1] = x;
	lights[0].lightPositions[2] = y;

	// Set Light One Position  rotating One Light - Y Rotation
	angle = (lightAngleOne * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[1].lightPositions[0] = x;
	lights[1].lightPositions[2] = y;

	// Set Light Two Position rotating Two Light Z Rotation
	angle = (lightAngleTwo * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[2].lightPositions[0] = x;
	lights[2].lightPositions[1] = y;
	if (isPerFrag == TRUE)
	{
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);

		// draw the desired graphics
		// drawing code -- magic

		if (bLight == TRUE)
		{
			glUniform1i(lightingEnabledUniform_PF, 1);

			for (int i = 0; i < 3; i++)
			{
				glUniform3fv(laUniform_PF[i], 1, lights[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform_PF
				glUniform3fv(ldUniform_PF[i], 1, lights[i].lightDiffuse);
				glUniform3fv(lsUniform_PF[i], 1, lights[i].lightSpecular);
				glUniform4fv(lighPositionUniform_PF[i], 1, lights[i].lightPositions);
			}

			glUniform3fv(kaUniform_PF, 1, materialAmbiant);
			glUniform3fv(kdUniform_PF, 1, meterialDiffuse);
			glUniform3fv(ksUniform_PF, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PF, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PF, 0);
		}
	}
	else
	{
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);

		if (bLight == TRUE)
		{
			glUniform1i(lightingEnabledUniform_PV, 1);

			for (int i = 0; i < 3; i++)
			{
				glUniform3fv(laUniform_PV[i], 1, lights[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
				glUniform3fv(ldUniform_PV[i], 1, lights[i].lightDiffuse);
				glUniform3fv(lsUniform_PV[i], 1, lights[i].lightSpecular);
				glUniform4fv(lighPositionUniform_PV[i], 1, lights[i].lightPositions);
			}

			glUniform3fv(kaUniform_PV, 1, materialAmbiant);
			glUniform3fv(kdUniform_PV, 1, meterialDiffuse);
			glUniform3fv(ksUniform_PV, 1, materialSpecular);
			glUniform1f(materialShininessUniform_PV, materialShineeness);
		}
		else
		{
			glUniform1i(lightingEnabledUniform_PV, 0);
		}
	}
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
	lightAngleZero = lightAngleZero + 1.0f;
	if (lightAngleZero > 360.0f)
		lightAngleZero = lightAngleZero - 360.0f;

	lightAngleOne = lightAngleOne + 1.0f;
	if (lightAngleOne > 360.0f)
		lightAngleOne = lightAngleOne - 360.0f;

	lightAngleTwo = lightAngleTwo + 1.0f;
	if (lightAngleTwo > 360.0f)
		lightAngleTwo = lightAngleTwo - 360.0f;
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

	if (shaderProgramObject_PF)
	{
		glUseProgram(shaderProgramObject_PF);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_PF, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_PF, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_PF);
		shaderProgramObject_PF = 0;
	}

	if (shaderProgramObject_PV)
	{
		glUseProgram(shaderProgramObject_PV);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_PV, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_PV, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_PV, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_PV);
		shaderProgramObject_PV = 0;
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
