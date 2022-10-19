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

using namespace vmath;

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define FBO_WIDTH 512
#define FBO_HEIGHT 512

// Global Variables
Display *display = NULL; // 77 memebrs
XVisualInfo *visualInfo; // 10 members
Colormap colormap;
Window window;
XEvent event;
Bool fullscreen = False;

int winWidth;
int winHeight;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig glxFBConfig;
GLXContext glxContext;

FILE *gpFile = NULL;

// OpenGL related Variable
Bool bActiveWindow = False;

// PP Related Global Variables
GLuint shaderProgramObject;

enum
{
	PRJ_ATRIBUTE_POSITION = 0,
	PRJ_ATRIBUTE_COLOR,
	PRJ_ATRIBUTE_NORMAL,
	PRJ_ATRIBUTE_TEXTURE0
};

// VAO VBO RELATED VARIABLES
GLuint vao_Cube;		  // Vertex Array Object - Cube
GLuint vbo_Cube_Position; // Vertex Buffer Object - Cube- Position
GLuint vbo_Cube_TexCoord; // Vertex Buffer Object - Cube - Color

// UNIFORMS RELATED VARIABLES
GLuint mvpMatrixUniform; // model View Projection
mat4 perspectiveProjectionMatrix;

// TEXTURE CUBE RELATED VARIABLES
GLfloat angleCube = 0.0f;
GLuint textureSamplerUniform;

// FBO RELATED GLOBAL VARIABLES
GLuint fbo; // FRAME BUFFER OBJECT
GLuint rbo; // RENDER BUFFER OBJECT
GLuint fbo_texture;
bool bBFOResult = false;

// TEXTURE SCENE GLOBAL VARIABLES
GLuint shaderProgramObject_sphere;

// VAO VBO RELATED
GLuint gVao_sphere;			 // Vertex Array Object
GLuint gVbo_sphere_position; // Vertex Buffer Object
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

// UNIFORM RELATED VARIABLES
GLuint modelMatrixUniform_sphere;
GLuint viewMatrixUniform_sphere;
GLuint projectionMatrixUniform_sphere;

mat4 perspectiveProjectionMatrix_sphere;

// LIGHT RELATED VARIABLES
// For Three Light on Sphere
GLuint laUniform_sphere[3];			  // light Ambiant
GLuint ldUniform_sphere[3];			  // light Diffuse
GLuint lsUniform_sphere[3];			  // light Spec
GLuint lighPositionUniform_sphere[3]; // light Position

GLuint kaUniform_sphere; // material amb
GLuint kdUniform_sphere; // mat diff
GLuint ksUniform_sphere; // mat specular
GLuint materialShininessUniform_sphere;

GLuint lightingEnabledUniform_sphere;

struct Light
{
	vmath::vec4 lightAmbiant;
	vmath::vec4 lightDiffuse;
	vmath::vec4 lightSpecular;
	vmath::vec4 lightPositions;
};

struct Light lights_sphere[3];

GLfloat materialAmbiant_sphere[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat meterialDiffuse_sphere[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular_sphere[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShineeness_sphere = 124.0f;

GLfloat lightAngleOne_sphere = 0.0f;
GLfloat lightAngleTwo_sphere = 150.0f;
GLfloat lightAngleZero_sphere = 300.0f;

// SPHERE RELATED VARIABLES
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices_sphere;
int gNumElements_sphere;

bool bLight;

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
	bool CreateFBO(GLint, GLint);
	int initialize_sphere(int, int);

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
		"in vec2 a_texcoord;"
		"uniform mat4 u_mvpMatrix;"
		"out vec2 a_texcoord_out;"
		"void main(void)"
		"{"
		"gl_Position = u_mvpMatrix * a_position;"
		"a_texcoord_out = a_texcoord;"
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
		"in vec2 a_texcoord_out;"
		"uniform sampler2D u_textureSampler;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"FragColor = texture(u_textureSampler , a_texcoord_out);"
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
	// Binding texcoord Array
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_TEXTURE0, "a_texcoord");

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
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	const GLfloat CubePosition[] = {
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

		// front
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// back
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// right
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f};

	const GLfloat CubeTexCoord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f};

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

	// vbo for Texcoord
	glGenBuffers(1, &vbo_Cube_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_TexCoord);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTexCoord), CubeTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Cube

	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	perspectiveProjectionMatrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT); // WARMUP RESIZE CALL

	// FBO CODE
	bBFOResult = CreateFBO(FBO_WIDTH, FBO_HEIGHT);

	int iRetVal;
	if (bBFOResult == true)
	{
		iRetVal = initialize_sphere(FBO_WIDTH, FBO_HEIGHT);
		//    HERE SHOULD BE ERROR CHECKING
	}
	else
	{
		fprintf(gpFile, "CREATE FBO FAILED...\n");
	}

	return (0);
}

void resize(int width, int height)
{
	/* code */
	void resize_sphere(int, int);

	winWidth = width;
	winHeight = height;

	resize_sphere(width, height);

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
	angleCube = angleCube + 1.1f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;

	// Set Light Zero Position - rotating zero light - x Around
	float angle = lightAngleZero_sphere * (M_PI / 180.0f);
	float x = 10.0f * cos(angle);
	float y = 10.0f * sin(angle);
	lights_sphere[0].lightPositions[1] = x;
	lights_sphere[0].lightPositions[2] = y;

	// Set Light One Position  rotating One Light - Y Rotation
	angle = (lightAngleOne_sphere * M_PI) / 180.0f;
	x = 10.0f * cos(angle);
	y = 10.0f * sin(angle);
	lights_sphere[1].lightPositions[0] = x;
	lights_sphere[1].lightPositions[2] = y;

	// Set Light Two Position rotating Two Light Z Rotation
	angle = (lightAngleTwo_sphere * M_PI) / 180.0f;
	x = 10.0f * cos(angle);
	y = 10.0f * sin(angle);
	lights_sphere[2].lightPositions[0] = x;
	lights_sphere[2].lightPositions[1] = y;
}

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();
	void uninitialize_sphere(void);

	// UNINTIALLIZE SPHERE
	uninitialize_sphere();

	/*  */
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

	void resize_sphere(int, int);
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
	// FUNCTION DECL
	void update_sphere(void);
	void display_sphere(GLint, GLint);

	/* Code */
	if (bBFOResult == true)
	{
		display_sphere(FBO_WIDTH, FBO_HEIGHT);
		update_sphere();
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	resize(winWidth, winHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Cube
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix_X = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix_Z = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); // glTranslatef() is replaced by this line

	rotationMatrix_X = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_Z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(textureSamplerUniform, 0); //

	glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);
}


void update_sphere(void)
{
	/* code */
	lightAngleZero_sphere = lightAngleZero_sphere + 1.1f;
	if (lightAngleZero_sphere > 360.0f)
		lightAngleZero_sphere = lightAngleZero_sphere - 360.0f;

	lightAngleOne_sphere = lightAngleOne_sphere + 1.1f;
	if (lightAngleOne_sphere > 360.0f)
		lightAngleOne_sphere = lightAngleOne_sphere - 360.0f;

	lightAngleTwo_sphere = lightAngleTwo_sphere + 1.1f;
	if (lightAngleTwo_sphere > 360.0f)
		lightAngleTwo_sphere = lightAngleTwo_sphere - 360.0f;
}

int initialize_sphere(int width, int height)
{
	/* fucntion delcations */
	void resize_sphere(int, int);
	void uninitialize_sphere(void);

	// vartex Shader
	const GLchar *vertexShaderSourceCode =
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
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
				free(log);
				log = NULL;
				uninitialize_sphere();
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
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG FOR SPHERE: %s\n", log);
				free(log);
				log = NULL;
				uninitialize_sphere();
			}
		}
	}

	// Shader Program Object
	shaderProgramObject_sphere = glCreateProgram();

	glAttachShader(shaderProgramObject_sphere, vertexShaderObject);
	glAttachShader(shaderProgramObject_sphere, fragmentShaderObject);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject_sphere, PRJ_ATRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject_sphere, PRJ_ATRIBUTE_NORMAL, "a_normal");

	// link
	glLinkProgram(shaderProgramObject_sphere);

	status = 0;
	infoLogLength = 0;

	glGetProgramiv(shaderProgramObject_sphere, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject_sphere, infoLogLength, &written, log);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG FOR SPHER: %s \n", log);
				free(log);
				log = NULL;
				uninitialize_sphere();
			}
		}
	}

	// post link - getting
	modelMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");
	viewMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
	projectionMatrixUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");

	laUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
	ldUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
	lsUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
	lighPositionUniform_sphere[0] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

	laUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
	ldUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
	lsUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
	lighPositionUniform_sphere[1] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

	laUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
	ldUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
	lsUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
	lighPositionUniform_sphere[2] = glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

	kaUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
	kdUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
	ksUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
	materialShininessUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_materialShininnes");

	lightingEnabledUniform_sphere = glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

	// declartions of vertex Data array
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices_sphere = getNumberOfSphereVertices();
	gNumElements_sphere = getNumberOfSphereElements();

	// vao vbo reelated code
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

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* Clear the  screen using black color */

	lights_sphere[0].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[0].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[1].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[1].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights_sphere[1].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights_sphere[1].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights_sphere[2].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights_sphere[2].lightDiffuse = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights_sphere[2].lightSpecular = vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lights_sphere[2].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix_sphere = mat4::identity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// warmup resize call
	resize_sphere(FBO_WIDTH, FBO_HEIGHT);

	return (0);
}

void display_sphere(GLint textureWidth, GLint textureHeight)
{

	void resize_sphere(int, int);

	/* Code */
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	resize_sphere(textureWidth, textureHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(shaderProgramObject_sphere);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(modelMatrixUniform_sphere, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform_sphere, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform_sphere, 1, GL_FALSE, perspectiveProjectionMatrix_sphere);

	// Light Related Code

	if (bLight == True)
	{
		glUniform1i(lightingEnabledUniform_sphere, 1);

		for (int i = 0; i < 3; i++)
		{
			glUniform3fv(laUniform_sphere[i], 1, lights_sphere[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform_sphere[i], 1, lights_sphere[i].lightDiffuse);
			glUniform3fv(lsUniform_sphere[i], 1, lights_sphere[i].lightSpecular);
			glUniform4fv(lighPositionUniform_sphere[i], 1, lights_sphere[i].lightPositions);
		}

		glUniform3fv(kaUniform_sphere, 1, materialAmbiant_sphere);
		glUniform3fv(kdUniform_sphere, 1, meterialDiffuse_sphere);
		glUniform3fv(ksUniform_sphere, 1, materialSpecular_sphere);
		glUniform1f(materialShininessUniform_sphere, materialShineeness_sphere);
	}
	else
	{
		glUniform1i(lightingEnabledUniform_sphere, 0);
	}

	glBindVertexArray(gVao_sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements_sphere, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glXSwapBuffers(display, window);
}



void resize_sphere(int width, int height)
{
	/* code */
	if (height == 0) // to avoid devided by zero
		height = 1;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix_sphere = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

void uninitialize_sphere(void)
{
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

	if (shaderProgramObject_sphere)
	{
		glUseProgram(shaderProgramObject_sphere);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject_sphere, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObject = NULL;
		shaderObject = (GLuint *)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject_sphere, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject_sphere);
		shaderProgramObject_sphere = 0;
	}
}

bool CreateFBO(GLint textureWidth, GLint textureHeight)
{
	//
	int maxRenderbufferSize;

	// CODE
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	// CHECK AVAILABLE BUFFER SIZE
	if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight)
	{
		fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
		return (false);
	}

	// CREATE FRAME BUFFER OBJECT
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// CREATE RENDER BUFFER OBJECT
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	// WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
		textureWidth,
		textureHeight);

	// CREATE EMPTY TEXTURE
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);

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
		fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

	return true;
}
