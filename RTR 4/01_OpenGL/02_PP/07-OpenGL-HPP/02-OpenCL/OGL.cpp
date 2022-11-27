/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include "vmath.h"

#include <CL/opencl.h>

using namespace vmath;

/* OpenGL libraries */
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "OpenCL.lib")

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

GLuint vao; // Vertex Array Object
GLuint vbo; // Vertex Buffer Object
GLuint mvpMatrixUniform;

mat4 perspectiveProjectionMatrix;

// sin wave Related variables
const unsigned int mesh_width = 1024;
const unsigned int mesh_height = 1024;

#define MYARRAYSIZE mesh_width *mesh_height * 4

float pos[mesh_width][mesh_height][4];
float animation_time = 0.0f;

// OpenCL Related Code
cl_int oclResult;

cl_mem graphicResource = NULL; // cl_mem is internally is pointer hgence no need mentione * explicteerly
cl_context oclContext;
cl_command_queue oclCommandQueue;

cl_program oclProgram;
cl_kernel oclKernel;

GLuint vbo_gpu;
BOOL onGPU = FALSE;

int colorNumber = 0;

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
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
	{
		MessageBox(NULL, TEXT("Creation of Log File Failed..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File is Successfuly Created.\n");
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

		case 'c':
		case 'C':
			onGPU = FALSE;
			break;

		case 'g':
		case 'G':
			onGPU = TRUE;
			break;

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

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			colorNumber++;
			if (colorNumber > 3)
			{
				colorNumber = 0;
			}
			break;

		case VK_DOWN:
			// uiNumberOfSegments--;
			// if (uiNumberOfSegments <= 0)
			//{
			//	uiNumberOfSegments = 1;
			// }
			break;
			//}
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
	cl_platform_id oclPlatformID;
	unsigned int devCount;
	cl_device_id *oclDeviceIDs = NULL;
	cl_device_id oclDeviceID;

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

	//
	// printfGLInfo();

	///*** OpenCL Initialization Related CODE
	// STEP 1 - Get Platform Id

	oclResult = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetPlatformIDs() failed : %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// STEP 2 - Get GPU Device ID
	// 2.1 Get All Deivice IDs
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 0, NULL, &devCount);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() failed to get device count : %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}
	else if (devCount == 0)
	{
		fprintf(gpFile, "GPU Device Count is Zero : %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// 2.2 Create Memory for the array of Device IDs for DevCount
	oclDeviceIDs = (cl_device_id *)malloc(devCount * sizeof(cl_device_id));
	if (oclDeviceIDs == NULL)
	{
		fprintf(gpFile, "malloc failed : %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// 2.3 Fill The Created Array
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, devCount, oclDeviceIDs, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetDeviceIDs() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// 2.4 Take the Zero th Deivce Array
	oclDeviceID = oclDeviceIDs[0];

	// 2.5 Free malloced Array
	free(oclDeviceIDs);

	cl_context_properties oclContextProperties[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)oclPlatformID,
			0};

	// STEP 3 - OpenCL Context for selected OpenCL Devicer
	// 3.1 Create Contect Properties Array
	oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceID, NULL, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateContext() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// STEP 4 - Create Command Queue.
	oclCommandQueue = clCreateCommandQueueWithProperties(oclContext, oclDeviceID, 0, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateCommandQueueWithProperties() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// STEP 5 - Create OpenCL Progra from OpenCL Kernel SoruceCode
	// 5.1 write OpenCL Kernel soruce code
	const char *oclKernelSourceCode =
		"__kernel void sinWaveKernel(__global float4 *position, unsigned int width, unsigned int height, float time)"
		"		{"
		"			unsigned int i = get_global_id(0);"
		"			unsigned int j = get_global_id(1);"

		"			float u = (float)i / (float)width;"
		"			float v = (float)j / (float)height;"
		"			u = u * 2.0f - 1.0f;"
		"			v = v * 2.0f - 1.0f;"

		"			float frequency = 4.0f;"
		"			float w = sin(u * frequency + time) * cos(v * frequency + time) * 0.5f;"

		"			position[j * width + i] = (float4)(u, w, v, 1.0f);"
		"		}";

	// 5.2 Create Actual OpenCL Program from above Soruce Code
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char **)&oclKernelSourceCode, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateProgramWithSource() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// STEP 6 - BUILD Program
	oclResult = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clBuildProgram() failed: %d \n", oclResult);

		char buffer[1024];
		size_t len;

		oclResult = clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);

		fprintf(gpFile, "clGetProgramBuildInfo() failed: %s \n", buffer);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// STEP 7 - Create OpenCL Kernel
	oclKernel = clCreateKernel(oclProgram, "sinWaveKernel", &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateKernel() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_color;"
		"uniform mat4 u_mvpMatrix;"
		"out vec3 a_color_out;"
		"void main(void)"
		"{"
		"gl_Position = u_mvpMatrix * a_position;"
		"a_color_out =a_color; "
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
		"in vec3 a_color_out;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = vec4(a_color_out,1.0);"
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
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_COLOR, "a_color");

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
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

	// Initializing mesh position array
	for (unsigned int i = 0; i < mesh_width; i++)
	{
		for (unsigned int j = 0; j < mesh_height; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				pos[i][j][k] = 0.0f;
			}
		}
	}

	// vao and vba related code
	// declartions of vertex Data array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, MYARRAYSIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
	glBufferData(GL_ARRAY_BUFFER, MYARRAYSIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Create OpenCL-OpenGL Interoperrability Resource
	graphicResource = clCreateFromGLBuffer(oclContext, CL_MEM_WRITE_ONLY, vbo_gpu, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateFromGLBuffer() failed: %d \n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

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
	// Function Declartions
	void sinWave(unsigned int, unsigned int, float);
	void uninitialize(void);

	// Variables Declartions
	size_t globalWorkSize[2];

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, -0.1f, -0.8f); // glTranslatef() is replaced by this line
	modelViewMatrix = translationMatrix;
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glBindVertexArray(vao);

	if (colorNumber == 0)
		glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 0.75f, 0.75f, 0.75f);
	else if (colorNumber == 1)
		glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 0.0f, 1.0f, 1.0f);
	else if (colorNumber == 2)
		glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 0.8f, 0.8f, 0.0f);
	else if (colorNumber == 3)
		glVertexAttrib3f(PRJ_ATRIBUTE_COLOR, 0.49f, 1.0f, 0.83f);

	int fps;
	TCHAR str[255];
	if (!onGPU)
		wsprintf(str, TEXT("OpenGL-OpenCL Interop | Running On CPU intel i7 				| FPS :  		|  Mesh Width :%d   Mesh Height : %d"), mesh_width, mesh_height);
	else
		wsprintf(str, TEXT("OpenGL-OpenCL Interop | Running On GPU NVIDIA GeForce GTX 1050 	| FPS :  		|  Mesh Width :%d   Mesh Height : %d"), mesh_width, mesh_height);

	SetWindowText(ghwnd, str);

	if (onGPU == TRUE)
	{
		// STEP 1 : Set OpenCL kernel parameters
		// 1.1 passing 1 st Parameter (0 th index)
		oclResult = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void *)&graphicResource);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed for 1 st Parameter: %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// 1.2 passing 1 st Parameter (index - 1)
		oclResult = clSetKernelArg(oclKernel, 1, sizeof(unsigned int), &mesh_width);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed for 2 nd Parameter: %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// 1.3 passing 3 nd Parameter (index - 2)
		oclResult = clSetKernelArg(oclKernel, 2, sizeof(unsigned int), &mesh_height);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed for 3 rd Parameter: %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// 1.4 passing 4 th Parameter (index - 3)
		oclResult = clSetKernelArg(oclKernel, 3, sizeof(float), &animation_time);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed for 4 th Parameter: %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// STEP 2 : Enqueue Graphics Resource into the command Queue
		oclResult = clEnqueueAcquireGLObjects(oclCommandQueue, 1, &graphicResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueAcquireGLObjects() failed :  %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// STEP 3 : RUN THE OPENCL KERNEL
		globalWorkSize[0] = mesh_width;
		globalWorkSize[1] = mesh_height;

		oclResult = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueNDRangeKernel() failed : %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		oclResult = clEnqueueReleaseGLObjects(oclCommandQueue, 1, &graphicResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueReleaseGLObjects() failed : %d \n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// STEP 4 Finsh the Command Queue
		clFinish(oclCommandQueue);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
	}
	else
	{
		sinWave(mesh_width, mesh_height, animation_time);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, MYARRAYSIZE * sizeof(float), pos, GL_DYNAMIC_DRAW);
	}

	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_POINTS, 0, mesh_width * mesh_height);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

	animation_time = animation_time + 0.01f;

	SwapBuffers(ghdc);
}

void sinWave(unsigned int width, unsigned int height, float time)
{
	// CODE
	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			for (unsigned int k = 0; k < 4; k++)
			{
				float u = (float)i / (float)width;
				float v = (float)j / (float)height;
				u = u * 2.0f - 1.0f;
				v = v * 2.0f - 1.0f;

				float frequency = 4.0f;
				float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;

				if (k == 0)
					pos[i][j][k] = u;
				else if (k == 1)
					pos[i][j][k] = w;
				else if (k == 2)
					pos[i][j][k] = v;
				else if (k == 3)
					pos[i][j][k] = 1.0f;
			}
		}
	}
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
	// deletion of vbo
	if (vbo_gpu)
	{
		if (graphicResource)
		{
			clReleaseMemObject(graphicResource);
			graphicResource = NULL;
		}
		glDeleteBuffers(1, &vbo_gpu);
		vbo_gpu = 0;
	}

	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}

	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}

	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}

	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	// deletion of vao
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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
