// Standard Headers
#include <stdio.h>	// For Standard IO
#include <stdlib.h> // For exit()
#include <memory.h> // For memset()

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

// Model Header Fie
#include "OGL.h"

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Variables
Display *display = NULL; // 77 memebrs
XVisualInfo *visualInfo; // 10 members
Colormap colormap;
Window window;
XEvent event;
KeySym keysym;

GLfloat gfLightAmbiant[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat gfLightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat gfLightPositions[] = {0.0f, 0.0f, 2.0f, 1.0f};

FILE *gpFile = NULL; // FILE* -> #include<stdio.h>
float angle;
Bool fullscreen = False;

// OpenGL related Variable
GLuint texture;
GLXContext glxContext;
Bool bActiveWindow = False;
Bool bTexture = False;
Bool gbLight = False;

// Entry Point Function
int main(void)
{
	// Function declartions
	void initiallize(void);
	void resize(int, int);
	void draw(void);
	void uninitiallize(void);
	void toggleFullscreen(void);
	void update(void);

	// Local Variables
	int defaultScreen;
	int defaultDepth;
	XSetWindowAttributes windowAttribute;
	int styleMask;
	Atom wm_delete_window_atom;
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
		printf("Creation of Log File Failed..!!! Exiting...");
		exit(0);
	}
	else
	{
		printf("Log File is Successfuly Created");
	}

	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("Error - XOpenDisplay() Failed...\n");
		uninitiallize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(display);

	defaultDepth = XDefaultDepth(display, defaultScreen);

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
	windowAttribute.colormap = XCreateColormap(display,
											   RootWindow(display, visualInfo->screen),
											   visualInfo->visual,
											   AllocNone);
	windowAttribute.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;

	colormap = windowAttribute.colormap;

	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;

	window = XCreateWindow(display,
						   RootWindow(display, visualInfo->screen),
						   100,
						   100,
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

	XStoreName(display, window, "PRJ - XWINDOW");

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
				keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0); // like WM_KEYDOWN
				switch (keysym)
				{
				case XK_Escape:
					uninitiallize();
					exit(0);
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
					if (gbLight == False)
					{
						glEnable(GL_LIGHTING);
						gbLight = True;
					}
					else
					{
						glDisable(GL_LIGHTING);
						gbLight = False;
					}
					break;

				case 'T':
				case 't':
					if (bTexture == False)
					{
						glEnable(GL_TEXTURE_2D);
						bTexture = True;
					}
					else
					{
						glDisable(GL_TEXTURE_2D);
						bTexture = False;
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
			// call update()
			update();
			draw();
		}
	}

	uninitiallize();

	return (0);
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

	if (loadGLTexture(&texture, "marble.bmp") == False)
		return; // write log in wndproc

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Ligth related Changes
	glLightfv(GL_LIGHT1, GL_AMBIENT, gfLightAmbiant);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, gfLightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, gfLightPositions);
	glEnable(GL_LIGHT1);

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

void draw(void)
{
	// variable declartions
	int vi, ni, ti;
	int i, j;

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.0f);

	if (bTexture == True)
		glBindTexture(GL_TEXTURE_2D, texture);
	else
		glBindTexture(GL_TEXTURE_2D, 0);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);

	for (i = 0; i < sizeof(face_indicies) / sizeof(face_indicies[0]); i++)
	{
		for (j = 0; j < 3; j++)
		{

			vi = face_indicies[i][j];
			ni = face_indicies[i][j + 3];
			ti = face_indicies[i][j + 6];

			glNormal3f(normals[ni][0], normals[ni][1], normals[ni][2]);
			glTexCoord2f(textures[ti][0], textures[ti][1]);
			glVertex3f(vertices[vi][0], vertices[vi][1], vertices[vi][2]);
		}
	}

	glEnd();

	glXSwapBuffers(display, window);
}

void update(void)
{
	angle = angle + 1.0f;
	if (angle > 360.0f)
		angle = angle - 360.0f;
}

Bool loadGLTexture(GLuint *texture, const char *path)
{
	// Variables declartions
	int width, height;
	unsigned char *imageData = NULL;

	// code
	imageData = SOIL_load_image(path, &width, &height, NULL, SOIL_LOAD_RGB);
	if (imageData == NULL)
		return False;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// create the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	SOIL_free_image_data(imageData);

	return True;
}

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();

	if (texture)
	{
		glDeleteTextures(1, &texture);
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
		printf("Log File Successfully Closed");
		fclose(gpFile);
		gpFile = NULL;
	}
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
