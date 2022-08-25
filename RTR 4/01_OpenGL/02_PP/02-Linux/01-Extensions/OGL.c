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
		return (-1);
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

	/* Here start OpeGL Code */
	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	resize(WIN_WIDTH, WIN_HEIGHT); // WARMUP RESIZE CALL
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // To AVOID DEVIDED BY ZERO
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void update(void)
{
	// code
}

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();

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

	glMatrixMode(GL_MODELVIEW);

	glXSwapBuffers(display, window);
}

/*
Installatin glew
Install ->https://sourceforge.net/projects/glew/
Open Folder and enter  command -> make
the enter command -> make install
*/

/*
How to Convert FFP base codeTo PP Complient Base Code.
1. Add Programable PipeLien Atributes in to FrabeBufferAtribute
2. Retrieve Array of Avaialable frame buffer configurations from XWindows.
3. From of all those frame buffer config choose the best frame bufferconfigre(FFP does it itself)
4. Retrieve The (best XVisaulInfo)XVisaulInfo PP-Complient from this PP Compliet Frame buffer config.
5. Retrieve the address of that GLX context creating function which supports this Config Atributes(Rember this function not available by defualt , Get its from loaded OpenGL driver librarirs).
6. Create GLX context related PP supporting atributes. perticularly the version that you want and profile that you want.
	If OpenGL Fails to give yiou required context then fall back to deafult old context which support PP.
	but if requred context is found then check eather hardware rendering does supported or not.
*/
