// Standard Headers
#include <stdio.h>	// For Standard IO
#include <stdlib.h> // For exit()
#include <memory.h> // For memset()

// X11 Headers (11 Is 11 th Version of X)
#include <X11/Xlib.h>
#include <X11/Xutil.h> // For XVisualInfo
#include <X11/XKBlib.h>

// OpenGL Header Files
#include <GL/glew.h>
#include <GL/gl.h>	// for OpenGL Functionality
#include <GL/glx.h> // for briding API
#include "vmath.h"
#include "Sphere.h"

// Texture library header
#include <SOIL/SOIL.h>

using namespace vmath;

// MACROS
#define WIN_WIDTH 960
#define WIN_HEIGHT 540

#define MODEL_VIEW_MATRIX_STACK 32
mat4 matrixStack[MODEL_VIEW_MATRIX_STACK]; //
int matrixStackTop = -1;

// Global Variables
Display *display = NULL; // 77 memebrs
XVisualInfo *visualInfo; // 10 members
Colormap colormap;
Window window;
XEvent event;
Bool fullscreen = False;
int winWidth, winHeight;

int currentWindowWidth = 0;
int currentWindowHeight = 0;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;
GLXContext glxContext;

FILE *gpFile = NULL;

// OpenGL related Variable
Bool bActiveWindow = False;

// PP related global variables
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

Bool bLight = False;

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

// Entry Point Function
int main(void)
{
	// Function declartions
	void toggleFullscreen(void);
	int initiallize(void);
	void resize(int, int);
	void draw(void);
	void update(void);
	void uninitiallize(void);

	// Local Variables
	int defaultScreen;
	int defaultDepth;
	GLXFBConfig *glxFbConfigs = NULL;
	GLXFBConfig bestGlxFbConfig;
	XVisualInfo *tempXvisualInfo = NULL;
	int numFBConfigs;
	XSetWindowAttributes windowAttribute;
	int styleMask;
	Atom wm_delete_window_atom;
	KeySym keysym;
	int screenWidth;
	int screenHeight;
	char keys[26];

	static int frameBufferAtribute[] =
		{
			GLX_DOUBLEBUFFER, True,
			GLX_X_RENDERABLE, True,
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
			GLX_RENDER_TYPE, GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
			GLX_RED_SIZE, 8,
			GLX_GREEN_SIZE, 8,
			GLX_BLUE_SIZE, 8,
			GLX_ALPHA_SIZE, 8,
			GLX_STENCIL_SIZE, 8,
			GLX_DEPTH_SIZE, 24,
			None};

	Bool bDone = False;

	// Code
	gpFile = fopen("Log.txt", "w");

	if (gpFile == NULL) // fopen_s -> #include<stdio.h>
	{
		printf("Creation of Log File Failed..!!! Exiting...\n");
		exit(0);
	}
	else
	{
		printf("Log File is Successfuly Created\n");
	}

	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("Error - XOpenDisplay() Failed...\n");
		uninitiallize();
		exit(1);
	}

	// Get default Screen of display from display
	defaultScreen = XDefaultScreen(display);

	// Get default Depth from display and defaultScreen
	defaultDepth = XDefaultDepth(display, defaultScreen);

	//
	glxFbConfigs = glXChooseFBConfig(display, defaultScreen, frameBufferAtribute, &numFBConfigs);

	if (glxFbConfigs == NULL)
	{
		fprintf(gpFile, "Error -glXChooseFBConfig() Failed...\n");
		uninitiallize();
		exit(-1);
	}
	else
	{
		fprintf(gpFile, "Found Number of FB Configs = %d\n", numFBConfigs);
	}

	// Find Best Frame Buffer from Array
	int bestFrameBufferConfig = -1;
	int worstFrameBufferConfig = -1;
	int bestNumberOfSamples = -1;
	int WorstNumberOfSamples = 999;

	for (int i = 0; i < numFBConfigs; i++)
	{

		tempXvisualInfo = glXGetVisualFromFBConfig(display, glxFbConfigs[i]);
		if (tempXvisualInfo != NULL)
		{

			int samples, sampleBuffer;
			glXGetFBConfigAttrib(display, glxFbConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
			glXGetFBConfigAttrib(display, glxFbConfigs[i], GLX_SAMPLES, &samples);
			fprintf(gpFile, "VisaulInfo = 0x%lu Found Samples buffer = %d and Samples = %d \n", tempXvisualInfo->visualid, sampleBuffer, samples);
			if (bestFrameBufferConfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
			{
				bestFrameBufferConfig = i;
				bestNumberOfSamples = samples;
			}
			if (worstFrameBufferConfig < 0 || !sampleBuffer || samples < WorstNumberOfSamples)
			{
				worstFrameBufferConfig = i;
				WorstNumberOfSamples = samples;
			}
		}
		XFree(tempXvisualInfo);
		tempXvisualInfo = NULL;
	}
	bestGlxFbConfig = glxFbConfigs[bestFrameBufferConfig];
	glxFBConfig = bestGlxFbConfig;
	XFree(glxFbConfigs);
	glxFbConfigs = NULL;

	visualInfo = glXGetVisualFromFBConfig(display, bestGlxFbConfig);
	fprintf(gpFile, "Visaul ID of best visual Info is : 0x%lu \n ", visualInfo->visualid);

	memset(&windowAttribute, 0, sizeof(XSetWindowAttributes));

	windowAttribute.border_pixel = 0;
	windowAttribute.background_pixel = XBlackPixel(display, defaultScreen);
	windowAttribute.background_pixmap = 0;
	windowAttribute.colormap = XCreateColormap(
		display,
		RootWindow(display, visualInfo->screen),
		visualInfo->visual,
		AllocNone);

	windowAttribute.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;

	colormap = windowAttribute.colormap;

	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

	window = XCreateWindow(display,
						   RootWindow(display, visualInfo->screen),
						   0,
						   0,
						   WIN_WIDTH,
						   WIN_HEIGHT,
						   0,
						   visualInfo->depth,
						   InputOutput,
						   visualInfo->visual,
						   styleMask,
						   &windowAttribute);

	if (!window)
	{
		printf("Error - XCreateWindow() Failed...\n");
		uninitiallize();
		exit(1);
	}

	XStoreName(display, window, "PRJ - OpenGL");

	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True);

	XSetWMProtocols(display, window, &wm_delete_window_atom, 1);

	XMapWindow(display, window);

	// Centering of Window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display, defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaultScreen));

	XMoveWindow(display, window, (screenWidth / 2) - (WIN_WIDTH / 2), (screenHeight / 2) - (WIN_HEIGHT / 2));
	toggleFullscreen();
	fullscreen = True;

	// intiallizw
	int ret = initiallize();
	if (ret == -1)
	{
		fprintf(gpFile, "Error - glXCreateContextAttribsARB Failed..\n");
		uninitiallize();
		exit(0);
	}
	else if (ret == -2)
	{
		fprintf(gpFile, "Error - glewInit() Failed..\n");
		uninitiallize();
		exit(0);
	}

	// Game Loop
	while (bDone == False)
	{
		while (XPending(display)) // XPending() == PeekMessage()
		{
			XNextEvent(display, &event); // Like GetMessage()

			switch (event.type)
			{
			case MapNotify:
				break;

			case FocusIn:
				bActiveWindow = True;
				break;

			case FocusOut:
				bActiveWindow = False;
				break;

			case KeyPress:
				keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);

				switch (keysym)
				{
				case XK_Escape:
					bDone = True;
					break;
				}

				XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL); // like WM_CHAR
				switch (keys[0])
				{
				case 'F':
				case 'f':
					if (fullscreen == False)
					{
						toggleFullscreen();
						fullscreen = True;
					}
					else
					{
						toggleFullscreen();
						fullscreen = False;
					}
					break;

				case 'L':
				case 'l':
					if (bLight == False)
					{
						bLight = True;
					}
					else
					{
						bLight = False;
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
				}
				break;

			case ConfigureNotify:
				winWidth = event.xconfigure.width;
				winHeight = event.xconfigure.height;
				resize(winWidth, winHeight);
				break;

			case 33:
				bDone = True;
				break;
			}
		}
		if (bActiveWindow == True)
		{
			update();
			draw();
		}
	}

	uninitiallize();

	return (0);
}

void toggleFullscreen(void)
{
	// Local Variables
	Atom wm_current_state_atom;
	Atom wm_fullscreen_state_atom;
	XEvent event;

	// code
	wm_current_state_atom = XInternAtom(display, "_NET_WM_STATE", False);

	wm_fullscreen_state_atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

	memset(&event, 0, sizeof(XEvent));

	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = wm_current_state_atom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = fullscreen ? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;

	XSendEvent(display, RootWindow(display, visualInfo->screen), False, SubstructureNotifyMask, &event);
}

int initiallize(void)
{
	// 	Function Prototype
	void resize(int, int);
	void uninitiallize(void);
	void printfGLInfo(void);
	void initializeMatrixStack(void);

	// code
	// glxContext = glXCreateContext(display, visualInfo, NULL, True);

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

	if (glXCreateContextAttribsARB == NULL)
	{
		return -1;
	}

	GLint contextAttributes[] = {GLX_CONTEXT_MAJOR_VERSION_ARB,
								 4,
								 GLX_CONTEXT_MINOR_VERSION_ARB,
								 6,
								 GLX_CONTEXT_PROFILE_MASK_ARB,
								 GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
								 None};

	glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttributes);

	if (!glxContext)
	{
		GLint contextAttributes[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
									 GLX_CONTEXT_MINOR_VERSION_ARB, 0,
									 None};
		glxContext = glXCreateContextAttribsARB(display, glxFBConfig, NULL, True, contextAttributes);

		fprintf(gpFile, "CAN NOT SUPPORT 4.6, HENCE FALLING BACK DEFAULT VERSION.\n");
	}
	else
	{
		fprintf(gpFile, "SUCCESS - FOUND THE SUPPORT TO 4.6\n");
	}

	// weather the Direct Rendering
	if (!glXIsDirect(display, glxContext))
	{
		fprintf(gpFile, "HW/DIRECT RENDERING IS SUPPORTS.\n");
	}
	else
	{
		fprintf(gpFile, "HW/DIRECT RENDERING NOT SUPPORTS.\n");
	}

	glXMakeCurrent(display, window, glxContext);

	/* Here start OpeGL Code */
	// GLEW INITIALIZATION
	if (glewInit() != GLEW_OK)
		return -2;

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
				uninitiallize();
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
				uninitiallize();
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
				uninitiallize();
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

	/* Here start OpeGL Code */
	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	perspectiveProjectionMatrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT); // WARMUP RESIZE CALL

	return (0);
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // To AVOID DEVIDED BY ZERO
		height = 1;

	glViewport(0, 0, (GLsizei)(width), (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);

	currentWindowWidth = width;
	currentWindowHeight = height;
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

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();
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

	if (currentContext && currentContext == glxContext)
	{
		glXMakeCurrent(display, 0, 0);
	}

	if (glxContext)
	{
		glXDestroyContext(display, glxContext);
		glxContext = NULL;
	}

	if (visualInfo)
	{
		free(visualInfo);
		visualInfo = NULL;
	}

	if (window)
	{
		XDestroyWindow(display, window);
	}

	if (colormap)
	{
		XFreeColormap(display, colormap);
	}

	if (display)
	{
		XCloseDisplay(display);
		display = NULL;
	}

	if (gpFile)
	{
		printf("Log File Successfully Closed\n");
		fclose(gpFile);
		gpFile = NULL;
	}
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

void draw(void)
{

	void draw24Sphere(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use shader program obejct
	glUseProgram(shaderProgramObject);

	draw24Sphere();

	// unuse the shader program object
	glUseProgram(0);
	glXSwapBuffers(display, window);
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

	if (bLight == True)
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

/*
void draw24Sphere(void)
{
	// Variable Declartions
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	// Code
	scaleMatrix = vmath::scale(0.7f, 0.7f, 0.7f);

	// ***** 1st sphere on 1st column, emerald *****
	// translationMatrix = vmath::translate(-9.0f, 6.0f, -21.0f); // glTranslatef() is replaced by this line
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f); // glTranslatef() is replaced by this line
	modelMatrix = translationMatrix * scaleMatrix;

	glViewport(0.0f, 120 * 5.5, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));
	printf("%d\n", winWidth);

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

	// Set Light Zero Position - rotating  light
	GLfloat angle = (angleForRotation * M_PI) / 180.0f;
	GLfloat x = 30.0f * cos(angle);
	GLfloat y = 30.0f * sin(angle);
	GLfloat z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	//	glViewport(0.0f, (GLsizei)(WIN_HEIGHT) - (WIN_HEIGHT) / 4, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));
	glViewport(0.0f, 120 * 4.4, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	// glViewport(0.0f, (GLsizei)(WIN_HEIGHT) - ((WIN_HEIGHT) / 2), (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));
	glViewport(0.0f, 120 * 3.3, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	// glViewport(0.0f, (GLsizei)(WIN_HEIGHT) - ((WIN_HEIGHT) / 1.3f), (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));
	glViewport(0.0f, 120 * 2.2, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(0.0f, 120 * 1.1, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(0.0f, 0.0f, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 120 * 5.5, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 120 * 4.4, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 120 * 3.3, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 120 * 2.2, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 120 * 1.1, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(425.0f, 0, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 120 * 5.5, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 120 * 4.4, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 120 * 3.3, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 120 * 2.2, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 120 * 1.1, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(850.0f, 0, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 120 * 5.5, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 120 * 4.4, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 120 * 3.3, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 120 * 2.2, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 120 * 1.1, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
	glViewport(1300.0f, 0, (GLsizei)(1920 / 3), (GLsizei)(1080 / 3));

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

	// Set Light Zero Position - rotating  light
	angle = (angleForRotation * M_PI) / 180.0f;
	x = 30.0f * cos(angle);
	y = 30.0f * sin(angle);
	z = -3.0f;

	if (keyPressed == 1)
	{
		lightPositions[1] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 2)
	{
		lightPositions[0] = x;
		lightPositions[2] = y + z;
	}
	if (keyPressed == 3)
	{
		lightPositions[0] = x;
		lightPositions[1] = y;
		lightPositions[2] = z;
	}

	if (bLight == True)
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
*/