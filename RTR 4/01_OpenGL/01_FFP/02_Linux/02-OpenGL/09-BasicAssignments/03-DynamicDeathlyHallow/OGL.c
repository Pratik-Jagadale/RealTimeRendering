// Standard Headers
#include <stdio.h>	// For Standard IO
#include <stdlib.h> // For exit()
#include <memory.h> // For memset()
#include <math.h>	// For M_PI & cos(), sin()

// X11 Headers (11 Is 11 th Version of X)
#include <X11/Xlib.h>
#include <X11/Xutil.h> // For XVisualInfo
#include <X11/XKBlib.h>

// OpenGL Header Files
#include <GL/gl.h>	// for OpenGL Functionality
#include <GL/glx.h> // for briding API
#include <GL/glu.h> // for glu functions

// Texture library header
#include <SOIL/SOIL.h>

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
FILE *gpFile = NULL;

float r = 0.0f;

float a[2] = {0.0f, 0.0f};	 // apex
float b[2] = {-2.0f, -1.0f}; // left bottom
float c[2] = {2.0f, -1.0f};	 // Rigth bottom

float SIDE = 0.0f;
float height = 0.0f;
float center[2] = {0.0f, 0.0f};
float TraingleVertex = -9.0f;
float verCircle[2] = {9.0f, -9.0f};
float LineVertex = 6.0f;
float angleTrangle = 0.0f;

// OpenGL related Variable
GLXContext glxContext;
Bool bActiveWindow = False;

// Entry Point Function
int main(void)
{
	// Function declartions
	void toggleFullscreen(void);
	void initiallize(void);
	void resize(int, int);
	void draw(void);
	void update(void);
	void uninitiallize(void);

	// Local Variables
	int defaultScreen;
	int defaultDepth;
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
			GLX_RGBA,
			GLX_RED_SIZE, 8,
			GLX_GREEN_SIZE, 8,
			GLX_BLUE_SIZE, 8,
			GLX_ALPHA_SIZE, 8,
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

	// get XVisualInfo() by using XMatchVisualInfo() and do error cheacking
	visualInfo = glXChooseVisual(display, defaultScreen, frameBufferAtribute); // opengl change

	if (visualInfo == NULL)
	{
		printf("Error - glXChooseVisual() Failed...\n");
		uninitiallize();
		exit(1);
	}

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
	initiallize();

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

void initiallize(void)
{
	// 	Function Prototype
	void resize(int, int);
	Bool loadGLTexture(GLuint *, const char *);

	// code
	glxContext = glXCreateContext(display, visualInfo, NULL, True);

	glXMakeCurrent(display, window, glxContext);

	/* Here start OpeGL Code */
	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	resize(WIN_WIDTH, WIN_HEIGHT); // WARMUP RESIZE CALL
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // To AVOID DEVIDED BY ZERO
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void update(void)
{
	// code
	if (TraingleVertex < 0)
		TraingleVertex = TraingleVertex + 0.01f;
	else if (verCircle[0] > 0 && verCircle[1] < 0)
	{
		verCircle[0] = verCircle[0] - 0.01f;
		verCircle[1] = verCircle[1] + 0.01f;
	}
	else if (LineVertex > 0)
		LineVertex = LineVertex - 0.01;

	angleTrangle = angleTrangle + 1.0f;
	if (angleTrangle >= 360.0f)
		angleTrangle = -360.0f;
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

void draw(void)
{
	// function prototype
	void drawTriangle();
	void drawLine(void);
	void drawCircle(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -12.0f);

	glTranslatef(TraingleVertex, TraingleVertex, 0.0f);
	glRotatef(angleTrangle, 0.0f, 1.0f, 0.0f);
	drawTriangle();

	SIDE = sqrt(pow((b[0] - c[0]), 2) + pow((b[1] - c[1]), 2));
	height = (sqrt(3) / 2) * SIDE;
	r = (sqrt(3) / 6) * SIDE;

	a[1] = b[1] + height; // apex y position

	// center vertices of circle
	center[0] = (a[0] + b[0] + c[0]) / 3;
	center[1] = (a[1] + b[1] + c[1]) / 3;

	// Circle
	glLoadIdentity();
	glTranslatef(center[0] + verCircle[0], center[1] + verCircle[1], -12.0f);
	glRotatef(angleTrangle, 0.0f, 1.0f, 0.0f);
	drawCircle();

	// Line
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f + LineVertex, -12.0f);
	drawLine();

	glXSwapBuffers(display, window);
}

void drawTriangle()
{
	glLineWidth(3);

	glBegin(GL_LINE_LOOP);
	glVertex3f(a[0], a[1], 0.0f);
	glVertex3f(b[0], b[1], 0.0f);
	glVertex3f(c[0], c[1], 0.0f);
	glEnd();
}

void drawLine(void)
{
	glBegin(GL_LINES);
	glVertex3f(a[0], a[1], 0.0f);
	glVertex3f(a[0], a[1] - height, 0.0f);
	glEnd();
}

void drawCircle(void)
{
	glPointSize(3);
	glBegin(GL_POINTS);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.01f)
	{
		float a = (angle * M_PI) / 180.0f;

		float x = r * cos(a);
		float y = r * sin(a);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void setColor(float r, float g, float b)
{
	glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}
