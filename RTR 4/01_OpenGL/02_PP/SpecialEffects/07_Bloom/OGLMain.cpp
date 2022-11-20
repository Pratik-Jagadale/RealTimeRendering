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
float winWidth;
float winHeight;

// PP Related Global Variables
GLuint shaderProgramObject_PV;
GLuint shaderProgramObject_render;

GLuint shaderProgramObject_filter;
GLuint shaderProgramObject_resolve;

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
BOOL bPerFragShader = FALSE;

GLfloat lightAmbiant[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositions[] = {0.0f, 5.0f, 0.0f, 1.0f};

GLfloat materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat meterialDeffuse[] = {0.5f, 0.2f, 0.7f, 1.0f};
GLfloat materialSpecular[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat materialShineeness = 120.0f;

/////////////////////////////////
// bloom related code
GLuint program_render;
GLuint program_filter;

// GLuint vao; alreadywe have
// GLuint ubo_transform;
// GLuint ubo_material;

GLuint tex_src;
GLuint tex_lut;

GLuint render_fbo;
GLuint filter_fbo[2];

GLuint tex_scene;
GLuint tex_brightpass;
GLuint tex_depth;
GLuint tex_filter[2];

float exposure = 1.0f;
int mode = 0;
bool paused = false;
float bloom_factor = 1.0f;
bool show_bloom = false;
bool show_scene = false;
bool show_prefilter = false;
float bloom_thresh_min = 0.8f;
float bloom_thresh_max = 1.2f;

int bloom_thresh_min_scene_uniform;
int bloom_thresh_max_scene_uniform;
int exposure_resolve_uniform;
int bloom_factor_resolve_uniform;
int scene_factor_resolve_uniform;
GLuint light_pos_uniform;

int MAX_SCENE_WIDTH = 2048;
int MAX_SCENE_HEIGHT = 2048;

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void display_resolve();

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
				// display_resolve();
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

		case 'w':
			lightPositions[2] = lightPositions[2] - 0.1f;
			break;

		case 's':
			lightPositions[2] = lightPositions[2] + 0.1f;
			break;

		case 'a':
			lightPositions[0] = lightPositions[0] - 0.1f;
			break;

		case 'd':
			lightPositions[0] = lightPositions[0] + 0.1f;
			break;

		case 'q':
			lightPositions[1] = lightPositions[1] + 0.1f;
			break;

		case 'e':
			lightPositions[1] = lightPositions[1] - 0.1f;
			break;

		case 'B':
			show_bloom = !show_bloom;
			break;
		case 'V':
			show_scene = !show_scene;
			break;
		case 'N':
			show_prefilter = !show_prefilter;
			break;

		case 'A':
			bloom_factor += 0.1f;
			break;
		case 'Z':
			bloom_factor -= 0.1f;
			break;
		case 'S':
			bloom_thresh_min += 0.1f;
			break;
		case 'X':
			bloom_thresh_min -= 0.1f;
			break;
		case 'D':
			bloom_thresh_max += 0.1f;
			break;
		case 'C':
			bloom_thresh_max -= 0.1f;
			break;

		case 'M':
			mode = (mode + 1) % 3;
			break;

		case 'o':
			paused = !paused;
			break;

		case 'j':
			exposure *= 1.1f;
			break;

		case 'J':
			exposure /= 1.1f;
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
	void initialize_resolve();
	void initialize_filter();

	/* variable declartions */
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	int i;
	static const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	static const GLfloat exposureLUT[20] = {11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f, 1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f};

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

	// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	/////////----------------------------------------------------------------------
	// vertex Shader - hdrbloom-scene
	{
		GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom-scene.vs", VERTEX);
		GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom-scene.fs", FRAGMENT);
		// Shader Program Object - Per Fragment
		shaderProgramObject_render = glCreateProgram();
		glAttachShader(shaderProgramObject_render, vertexShaderObject);
		glAttachShader(shaderProgramObject_render, fragmentShaderObject);

		glBindAttribLocation(shaderProgramObject_render, PVG_ATTRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject_render, PVG_ATTRIBUTE_NORMAL, "a_normal");

		// link
		BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_render);

		// PerFrag - Post link
		modelMatrixUniform = glGetUniformLocation(shaderProgramObject_render, "u_modelMatrix");
		viewMatrixUniform = glGetUniformLocation(shaderProgramObject_render, "u_viewMatrix");
		projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_render, "u_projectionMatrix");

		laUniform = glGetUniformLocation(shaderProgramObject_render, "u_la");
		ldUniform = glGetUniformLocation(shaderProgramObject_render, "u_ld");
		lsUniform = glGetUniformLocation(shaderProgramObject_render, "u_ls");
		lighPositionUniform = glGetUniformLocation(shaderProgramObject_render, "u_lightPosition");

		kaUniform = glGetUniformLocation(shaderProgramObject_render, "u_ka");
		kdUniform = glGetUniformLocation(shaderProgramObject_render, "u_kd");
		ksUniform = glGetUniformLocation(shaderProgramObject_render, "u_ks");

		materialShininessUniform = glGetUniformLocation(shaderProgramObject_render, "u_materialShininnes");

		lightingEnabledUniform = glGetUniformLocation(shaderProgramObject_render, "u_lightingEnabled");

		bloom_thresh_min_scene_uniform = glGetUniformLocation(shaderProgramObject_render, "bloom_thresh_min");
		bloom_thresh_max_scene_uniform = glGetUniformLocation(shaderProgramObject_render, "bloom_thresh_max");
	}

	initialize_resolve();

	initialize_filter();

	// create render fbo
	{

		glGenFramebuffers(1, &render_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);

		glGenTextures(1, &tex_scene);
		glBindTexture(GL_TEXTURE_2D, tex_scene);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_scene, 0);
		glGenTextures(1, &tex_brightpass);
		glBindTexture(GL_TEXTURE_2D, tex_brightpass);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_brightpass, 0);
		glGenTextures(1, &tex_depth);
		glBindTexture(GL_TEXTURE_2D, tex_depth);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_depth, 0);
		glDrawBuffers(2, buffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Create filter
	{
		glGenFramebuffers(2, &filter_fbo[0]);
		glGenTextures(2, &tex_filter[0]);
		for (i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[i]);
			glBindTexture(GL_TEXTURE_2D, tex_filter[i]);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_filter[i], 0);
			glDrawBuffers(1, buffers);
		}

		glGenTextures(1, &tex_lut);
		glBindTexture(GL_TEXTURE_1D, tex_lut);
		glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
		glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// gVao_sphere and vba related code
	// declartions of vertex Data array
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao vbo reelated code

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

	winWidth = width;
	winHeight = height;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

void display(void)
{
	static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static const GLfloat one = 1.0f;

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, winWidth, winHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_COLOR, 1, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(shaderProgramObject_render);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	for (float i = -6.0f; i < 6.0f; i = i + 1.2f)
	{
		for (float j = -3.0f; j < 4.0f; j = j + 1.2f)
		{
			translationMatrix = vmath::translate(0.0f + i, 0.0f + j, -10.0f);

			modelMatrix = translationMatrix;

			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			if (bLight == TRUE)
			{

				glUniform1i(lightingEnabledUniform, 1);

				glUniform3fv(laUniform, 1, lightAmbiant);
				glUniform3fv(ldUniform, 1, lightDiffuse);
				glUniform3fv(lsUniform, 1, lightSpecular);
				glUniform4fv(lighPositionUniform, 1, lightPositions);

				glUniform3fv(kaUniform, 1, materialAmbiant);
				glUniform3fv(kdUniform, 1, meterialDeffuse);
				glUniform3fv(ksUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShineeness);

				glUniform1f(bloom_thresh_min_scene_uniform, bloom_thresh_min);
				glUniform1f(bloom_thresh_max_scene_uniform, bloom_thresh_max);
			}
			else
			{
				glUniform1i(lightingEnabledUniform, 0);
			}

			glBindVertexArray(gVao_sphere);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		}
	}

	{
		// White
		translationMatrix = vmath::translate(lightPositions[0], lightPositions[1], lightPositions[2]);
		scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
		modelMatrix = translationMatrix * scaleMatrix;

		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(lightingEnabledUniform, 0);

		glBindVertexArray(gVao_sphere);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	}

	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaderProgramObject_filter);

	glBindVertexArray(gVao_sphere); ////////////////need to mpdofie

	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[0]);
	glBindTexture(GL_TEXTURE_2D, tex_brightpass);
	glViewport(0, 0, winWidth, winHeight); ////////////////need to mpdofie

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); ////////////////need to mpdofie

	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[1]);
	glBindTexture(GL_TEXTURE_2D, tex_filter[0]);
	glViewport(0, 0, winWidth, winHeight); ////////////////need to mpdofie

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); ////////////////need to mpdofie

	glUseProgram(shaderProgramObject_resolve);

	glUniform1f(exposure_resolve_uniform, exposure);
	if (show_prefilter)
	{
		glUniform1f(bloom_factor_resolve_uniform, 0.0f);
		glUniform1f(scene_factor_resolve_uniform, 1.0f);
	}
	else
	{
		glUniform1f(bloom_factor_resolve_uniform, show_bloom ? bloom_factor : 0.0f);
		glUniform1f(scene_factor_resolve_uniform, show_scene ? 1.0f : 0.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_filter[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, show_prefilter ? tex_brightpass : tex_scene);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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

	UninitializeShaders(shaderProgramObject_render);
	UninitializeShaders(shaderProgramObject_resolve);
	UninitializeShaders(shaderProgramObject_filter);

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

void initialize_resolve()
{
	GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom_resolve.vs", VERTEX);
	GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom_resolve.fs", FRAGMENT);

	// Shader Program Object - Per Fragment
	shaderProgramObject_resolve = glCreateProgram();
	glAttachShader(shaderProgramObject_resolve, vertexShaderObject);
	glAttachShader(shaderProgramObject_resolve, fragmentShaderObject);

	// Pre Link

	// link
	BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_resolve);

	// Post Link
	exposure_resolve_uniform = glGetUniformLocation(shaderProgramObject_resolve, "exposure");
	bloom_factor_resolve_uniform = glGetUniformLocation(shaderProgramObject_resolve, "bloom_factor");
	scene_factor_resolve_uniform = glGetUniformLocation(shaderProgramObject_resolve, "scene_factor");
}

void display_resolve()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramObject_resolve);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1f(exposure_resolve_uniform, exposure);
	if (show_prefilter)
	{
		glUniform1f(bloom_factor_resolve_uniform, 0.0f);
		glUniform1f(scene_factor_resolve_uniform, 1.0f);
	}
	else
	{
		glUniform1f(bloom_factor_resolve_uniform, show_bloom ? bloom_factor : 0.0f);
		glUniform1f(scene_factor_resolve_uniform, show_scene ? 1.0f : 0.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, tex_filter[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, show_prefilter ? tex_brightpass : tex_scene);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void initialize_filter()
{
	GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom_filter.vs", VERTEX);
	GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\hdrbloom_filter.fs", FRAGMENT);

	// Shader Program Object - Per Fragment
	shaderProgramObject_filter = glCreateProgram();
	glAttachShader(shaderProgramObject_filter, vertexShaderObject);
	glAttachShader(shaderProgramObject_filter, fragmentShaderObject);

	// link
	BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_filter);
}

void display_filter()
{

	glUseProgram(shaderProgramObject_resolve);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, .0f, -4.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1f(exposure_resolve_uniform, exposure);
	if (show_prefilter)
	{
		glUniform1f(bloom_factor_resolve_uniform, 0.0f);
		glUniform1f(scene_factor_resolve_uniform, 1.0f);
	}
	else
	{
		glUniform1f(bloom_factor_resolve_uniform, show_bloom ? bloom_factor : 0.0f);
		glUniform1f(scene_factor_resolve_uniform, show_scene ? 1.0f : 0.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_filter[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, show_prefilter ? tex_brightpass : tex_scene);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

/*
bool CreateFBO_render(GLint textureWidth, GLint textureHeight)
{
	//
	int maxRenderbufferSize;

	// CODE
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// CHECK AVAILABLE BUFFER SIZE
	if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		// fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
		return (false);
	}

	// CREATE FRAME BUFFER OBJECT
	glGenFramebuffers(1, &render_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);

	// CREATE RENDER BUFFER OBJECT
	glGenRenderbuffers(1, &render_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, render_fbo);

	// WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
		textureWidth,
		textureHeight);

	// CREATE EMPTY TEXTURE
	glGenTextures(1, &tex_scene);
	glBindTexture(GL_TEXTURE_2D, tex_scene);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		textureWidth,
		textureHeight,
		0,
		GL_RGB,
		GL_UNSIGNED_SHORT_5_6_5,
		NULL // NULL - EMPTY TEXTURE
	);

	// GIVE ABOVE TEXTURE TO FBO
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		tex_scene,
		0 // MIPMAP LEVEL
	);

	// GIVE RENDER BUFFER TO FBO
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		render_fbo //
	);

	// CHECK WATHER FBO IS CREATED SUCCESSFULLY OR NOT
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		// fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

	return true;
}
*/
GLuint render_filter[2];
/*
bool CreateFBO_filter(GLint textureWidth, GLint textureHeight)
{
	//
	int maxRenderbufferSize;

	// CODE
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// CHECK AVAILABLE BUFFER SIZE
	if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		// fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
		return (false);
	}

	// CREATE FRAME BUFFER OBJECT 1
	glGenFramebuffers(1, &filter_fbo[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[0]);

	// CREATE RENDER BUFFER OBJECT 1
	glGenRenderbuffers(1, &render_filter[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, render_filter[0]);

	// CREATE FRAME BUFFER OBJECT 2
	glGenFramebuffers(1, &filter_fbo[1]);
	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[1]);

	// CREATE RENDER BUFFER OBJECT 2
	glGenRenderbuffers(1, &render_filter[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, render_filter[1]);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[i]);
		glBindTexture(GL_TEXTURE_2D, tex_filter[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_filter[i], 0);
		glDrawBuffers(1, buffers);
	}

	// WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
		textureWidth,
		textureHeight);

	// CREATE EMPTY TEXTURE
	glGenTextures(1, &tex_lut);
	glBindTexture(GL_TEXTURE_2D, tex_lut);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		textureWidth,
		textureHeight,
		0,
		GL_RGB,
		GL_UNSIGNED_SHORT_5_6_5,
		NULL // NULL - EMPTY TEXTURE
	);

	// GIVE ABOVE TEXTURE TO FBO
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		fbo_texture,
		0 // MIPMAP LEVEL
	);

	// GIVE RENDER BUFFER TO FBO
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		rbo //
	);

	// CHECK WATHER FBO IS CREATED SUCCESSFULLY OR NOT
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		// fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

	return true;
}
*/