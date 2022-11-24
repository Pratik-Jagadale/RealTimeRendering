/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include "vmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

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

GLuint vao_Cube;		  // Vertex Array Object - Cube
GLuint vbo_Cube_Position; // Vertex Buffer Object - Cube- Position
GLuint vbo_Cube_TexCoord; // Vertex Buffer Object - Cube - Color
GLuint mvpMatrixUniform;  // model View Projection
mat4 perspectiveProjectionMatrix;

GLfloat angleCube = 0.0f;

GLuint texture_cube;
GLuint textureSamplerUniform;

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
		fprintf(gpFile, "Makeing OpenGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "GLEW Initialization Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -6)
	{
		fprintf(gpFile, " loadGLTexture Failed for texture_stone ...\n");
		uninitialize();
	}
	else if (iRetVal == -7)
	{
		fprintf(gpFile, "loadGLTexture Failed for texture cube ...\n");
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
	BOOL LoadGLTexture(GLuint *, TCHAR[]);
	BOOL LoadCubemap(GLuint *, char *faces[]);
	BOOL LoadCubemap(GLuint * texture, TCHAR ImageResourceID[]);

	char *faces[] =
		{
			"z_right.png",
			"z_left.png",
			"z_top.png",
			"z_bottom.png",
			"z_front.png",
			"z_back.png"};

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
		"uniform mat4 u_mvpMatrix;"
		"out vec3 a_texcoord_out;"
		"void main(void)"
		"{"
		"gl_Position = u_mvpMatrix * a_position;"
		"a_texcoord_out = vec3(a_position);"
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
		"in vec3 a_texcoord_out;"
		"uniform samplerCube  u_textureSampler;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = texture( u_textureSampler, a_texcoord_out);"
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
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	// vao_Pyramid and vba related code

	const GLfloat CubePosition[] = {
		// positions
		// right
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

		// top
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		// bottom
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// back
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// front
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f

	};

	// vao and vbo related code
	// vao for Cube
	glGenVertexArrays(1, &vao_Cube);
	glBindVertexArray(vao_Cube);

	// vbo for position
	glGenBuffers(1, &vbo_Cube_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubePosition), CubePosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Cube

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.25f, 0.3f, 1.0f);

	// Texture Initiallization
	// if (LoadGLTexture(&texture_cube, MAKEINTRESOURCE(IDBITMAP_cube)) == FALSE)
	if (LoadCubemap(&texture_cube, MAKEINTRESOURCE(IDBITMAP_RIGHT)) == FALSE)
		return -7;

	// LoadCubemap(&texture_cube, faces);

	// Enabaling the texture
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Pyramid
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix_X = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix_Z = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	// Cube
	// Tranformations
	translationMatrix = mat4::identity();
	modelViewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f); // glTranslatef() is replaced by this line

	rotationMatrix_X = vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f);
	// rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	//  rotationMatrix_Z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	//	rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	scaleMatrix = vmath::scale(10.0f, 10.0f, 10.0f);

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix_X;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cube);
	glUniform1i(textureSamplerUniform, 0); //

	glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);

	//	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */
	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	if (texture_cube)
	{
		glDeleteTextures(1, &texture_cube);
		texture_cube = 0;
	}

	if (vbo_Cube_TexCoord)
	{
		glDeleteBuffers(1, &vbo_Cube_TexCoord);
		vbo_Cube_TexCoord = 0;
	}

	// delete vbo_Cube_Position
	if (vbo_Cube_Position)
	{
		glDeleteBuffers(1, &vbo_Cube_Position);
		vbo_Cube_Position = 0;
	}

	// deletion of vao_Cube
	if (vao_Cube)
	{
		glDeleteVertexArrays(1, &vao_Cube);
		vao_Cube = 0;
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

BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[])
{
	// variable declartions
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		bResult = TRUE;
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 4 TO 1 FOR BETTER PERFORMANCE

		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// create the texture
		glTexImage2D(GL_TEXTURE_2D,	   // Targter
					 0,				   // MipMap Level (done by opengl)
					 GL_RGBA,		   // Opengl Image format
					 bmp.bmWidth,	   // Image Width
					 bmp.bmHeight,	   // Image Height
					 0,				   // Border Width
					 GL_BGR,		   // Image Format
					 GL_UNSIGNED_BYTE, // Data type of bmp.bmBits
					 bmp.bmBits);	   //

		glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMap

		glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

		// DELETE Object
		DeleteObject(hBitmap);
	}
	return bResult;
}

BOOL LoadCubemap(GLuint *texture, TCHAR ImageResourceID[])
{
	// variable declartions
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 4 TO 1 FOR BETTER PERFORMANCE
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{

		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDBITMAP_RIGHT + i), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		if (hBitmap)
		{
			// create the texture
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Targter
						 0,									 // MipMap Level (done by opengl)
						 GL_RGBA,							 // Opengl Image format
						 bmp.bmWidth,						 // Image Width
						 bmp.bmHeight,						 // Image Height
						 0,									 // Border Width
						 GL_BGR,							 // Image Format
						 GL_UNSIGNED_BYTE,					 // Data type of bmp.bmBits
						 bmp.bmBits);						 //

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // Generate MipMap

			bResult = TRUE;
		}
		else
		{
			fprintf(gpFile, "Cubemap tex failed to load at path: ");
			bResult = FALSE;
		}

		// DELETE Object
		DeleteObject(hBitmap);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind texture

	return bResult;
}

BOOL LoadCubemap(GLuint *texture, char *faces[])
{
	//   GLuint *texture;
	TCHAR str[1024];
	int iWidth, iHeight, iComponents;

	// code
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Load cube map images
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char *Data = stbi_load(faces[i], &iWidth, &iHeight, &iComponents, 0);
		if (!Data)
		{

			fprintf(gpFile, "Cubemap tex failed to load at path: ");
			stbi_image_free(Data);
			return FALSE;
		}
		else
		{

			if (iComponents == 3)
			{
				fprintf(gpFile, "iComponents == 3 \n");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
			}
			else if (iComponents == 4)
			{
				fprintf(gpFile, "iComponents == 4 \n");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
			}

			stbi_image_free(Data);
		}
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return TRUE;
}
