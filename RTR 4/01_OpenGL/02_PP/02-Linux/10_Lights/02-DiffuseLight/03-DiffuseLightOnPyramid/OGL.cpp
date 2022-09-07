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

// Texture library header
#include <SOIL/SOIL.h>

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *display = NULL; // 77 memebrs
XVisualInfo *visualInfo; // 10 members
Colormap colormap;
Window window;
XEvent event;
Bool fullscreen = False;

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

GLuint vao_Pyramid;			 // Vertex Array Object - Pyramid
GLuint vbo_Pyramid_Position; // Vertex Buffer Object - Pyramid - Position
GLuint vbo_Pyramid_Normal;
GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;
mat4 perspectiveProjectionMatrix;
GLuint lDUniform; //
GLuint kDUniform; //
GLuint lightPositionUniform;
GLuint lightingEnableUniform;

Bool bLight = False;
GLfloat angleCube = 0.0f;

GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPositions[] = {0.0f, 0.0f, 2.0f, 1.0f};

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
	static int winWidth, winHeight;
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
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform vec3 u_ld;"
		"uniform vec3 u_kd;"
		"uniform vec4 u_lightPosition;"
		"uniform int u_lightingEnabled;"
		"out vec3 diffuse_light_color;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinate = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));"
		"vec3 transformedNormals = normalize(normalMatrix * a_normal);"
		"vec3 lightDirection = vec3(normalize(u_lightPosition - eyeCoordinate));"
		"diffuse_light_color = u_ld * u_kd * max(dot(lightDirection,transformedNormals),0.0);"
		"}"
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix  * a_position;"
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
		"FragColor = vec4(1.0f,1.0f,1.0f,1.0f);"
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
	// Binding Position Array
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

	lDUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	kDUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
	lightingEnableUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

	// vao_Pyramid and vba related code
	// declartions of vertex Data array
	const GLfloat PyramidPosition[] = {
		// front
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// right
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// back
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		// left
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f};

	GLfloat pyramidNormals[] = {

		0.0f, 0.447214f, 0.894427f, // front-top
		0.0f, 0.447214f, 0.894427f, // front-left
		0.0f, 0.447214f, 0.894427f, // front-right

		0.894427f, 0.447214f, 0.0f, // right-top
		0.894427f, 0.447214f, 0.0f, // right-left
		0.894427f, 0.447214f, 0.0f, // right-right

		0.0f, 0.447214f, -0.894427f, // back-top
		0.0f, 0.447214f, -0.894427f, // back-left
		0.0f, 0.447214f, -0.894427f, // back-right

		-0.894427f, 0.447214f, 0.0f, // left-top
		-0.894427f, 0.447214f, 0.0f, // left-left
		-0.894427f, 0.447214f, 0.0f	 // left-right
	};

	// vao and vbo related code
	// vao for Pyramid
	glGenVertexArrays(1, &vao_Pyramid);
	glBindVertexArray(vao_Pyramid);

	// vbo for position
	glGenBuffers(1, &vbo_Pyramid_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Pyramid_Position);

	glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidPosition), PyramidPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normals
	glGenBuffers(1, &vbo_Pyramid_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Pyramid_Normal);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

void update(void)
{
	// code
	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;
}

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();
	/*  */
	// delete vbo_normal
	if (vbo_Pyramid_Position)
	{
		glDeleteBuffers(1, &vbo_Pyramid_Position);
		vbo_Pyramid_Position = 0;
	}

	if (vbo_Pyramid_Normal)
	{
		glDeleteBuffers(1, &vbo_Pyramid_Normal);
		vbo_Pyramid_Normal = 0;
	}

	// deletion of vao_Pyramid
	if (vao_Pyramid)
	{
		glDeleteVertexArrays(1, &vao_Pyramid);
		vao_Pyramid = 0;
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
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Cube
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f); // glTranslatef() is replaced by this line

	rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

	rotationMatrix = rotationMatrix_Y;

	modelMatrix = translationMatrix * rotationMatrix;

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// Sending Light Related uniforms
	if (bLight == True)
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

	glBindVertexArray(vao_Pyramid);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

	glXSwapBuffers(display, window);
}
