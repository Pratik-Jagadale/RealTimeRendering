// Standard Headers
#include <stdio.h>	// For Standard IO
#include <stdlib.h> // For exit()
#include <memory.h> // For memset()
#include <math.h>

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

// OpenGL related Variable
GLXContext glxContext;
Bool bActiveWindow = False;

GLUquadric *quadric = NULL;

Bool bLight = False;
GLfloat lightAmbiantZero[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightDiffuseZero[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecularZero[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPositionZero[] = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat light_Model_Ambiant[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_Model_Local_Viewer[] = {0.0f};

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLint keyPressed = 0;

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

				case 'L':
				case 'l':
					if (bLight == False)
					{
						glEnable(GL_LIGHTING);
						bLight = True;
					}
					else
					{
						glDisable(GL_LIGHTING);
						bLight = False;
					}

					break;

				case 'x':
				case 'X':
					keyPressed = 1;
					angleForXRotation = 0.0f; // reset
					break;

				case 'y':
				case 'Y':
					keyPressed = 2;
					angleForYRotation = 0.0f; // reset
					break;

				case 'z':
				case 'Z':
					keyPressed = 3;
					angleForZRotation = 0.0f; // reset
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

	// light related changes
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_Model_Ambiant);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, light_Model_Local_Viewer);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbiantZero);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseZero);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularZero);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

	glEnable(GL_LIGHT0);

	// quadric intialliza
	// create quadric
	quadric = gluNewQuadric();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 1.0f;
		if (angleForXRotation > 360.0f)
			angleForXRotation = angleForXRotation - 360.0f;
	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 1.00f;
		if (angleForYRotation > 360.0f)
			angleForYRotation = angleForYRotation - 360.0f;
	}

	if (keyPressed == 3)
	{
		angleForZRotation = angleForZRotation + 1.0f;
		if (angleForZRotation > 360.0f)
			angleForZRotation = angleForZRotation - 360.0f;
	}
}

void uninitiallize(void)
{
	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
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

void draw(void)
{
	// function prototype
	void colorSetcolor(int r, int g, int b);
	void draw24Sphere(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat x, y;
	GLfloat angle;

	if (keyPressed == 1)
	{
		angle = (angleForXRotation * M_PI) / 180.0f;
		x = 45.0f * cos(angle);
		y = 45.0f * sin(angle);

		lightPositionZero[1] = x;
		lightPositionZero[2] = y;
	}
	else if (keyPressed == 2)
	{
		angle = angleForYRotation * (M_PI / 180.0f);
		x = 55.0f * cos(angle);
		y = 55.0f * sin(angle);

		lightPositionZero[0] = x;
		lightPositionZero[2] = y;
	}
	else if (keyPressed == 3)
	{
		angle = angleForZRotation * (M_PI / 180.0f);
		x = 45.0f * cos(angle);
		y = 45.0f * sin(angle);

		lightPositionZero[1] = x;
		lightPositionZero[0] = y;
	}
	else
	{
		lightPositionZero[0] = 0.0f;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//    glTranslatef(0.0f, 0.0f, -4.0f);

	draw24Sphere();

	glXSwapBuffers(display, window);
}

void draw24Sphere(void)
{
	// variable declartions
	GLfloat materialAmbiant[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShinniness;

	// ***** 1st sphere on 1st column, emerald *****
	// ambient material
	materialAmbiant[0] = 0.0215; // r
	materialAmbiant[1] = 0.1745; // g
	materialAmbiant[2] = 0.0215; // b
	materialAmbiant[3] = 1.0f;	 // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.633;	// r
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;	// b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, 6.0f, -19.0f);
	// glTranslatef(1.5f, 14.0f, 0.0f); // x + 6  ,
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 1st column, jade *****
	// ambient material
	materialAmbiant[0] = 0.135;	 // r
	materialAmbiant[1] = 0.2225; // g
	materialAmbiant[2] = 0.1575; // b
	materialAmbiant[3] = 1.0f;	 // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, 3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbiant[0] = 0.05375; // r
	materialAmbiant[1] = 0.05;	  // g
	materialAmbiant[2] = 0.06625; // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;	  // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.3 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, 1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 1st column, pearl *****
	// ambient material
	materialAmbiant[0] = 0.25;	  // r
	materialAmbiant[1] = 0.20725; // g
	materialAmbiant[2] = 0.20725; // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 1.0;	// r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;	// a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.088 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, -1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 1st column, ruby *****
	// ambient material
	materialAmbiant[0] = 0.1745;  // r
	materialAmbiant[1] = 0.01175; // g
	materialAmbiant[2] = 0.01175; // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, -3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 1st column, turquoise *****
	// ambient material
	materialAmbiant[0] = 0.1;	  // r
	materialAmbiant[1] = 0.18725; // g
	materialAmbiant[2] = 0.1745;  // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.396;	  // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;	// g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-9.0f, -6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	// ambient material
	materialAmbiant[0] = 0.329412; // r
	materialAmbiant[1] = 0.223529; // g
	materialAmbiant[2] = 0.027451; // b
	materialAmbiant[3] = 1.0f;	   // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;	   // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.21794872 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, 6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 2nd column, bronze *****
	// ambient material
	materialAmbiant[0] = 0.2125; // r
	materialAmbiant[1] = 0.1275; // g
	materialAmbiant[2] = 0.054;	 // b
	materialAmbiant[3] = 1.0f;	 // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.714;	  // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.2 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, 3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, chrome *****
	// ambient material
	materialAmbiant[0] = 0.25; // r
	materialAmbiant[1] = 0.25; // g
	materialAmbiant[2] = 0.25; // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, 1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 2nd column, copper *****
	// ambient material
	materialAmbiant[0] = 0.19125; // r
	materialAmbiant[1] = 0.0735;  // g
	materialAmbiant[2] = 0.0225;  // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, -1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 2nd column, gold *****
	// ambient material
	materialAmbiant[0] = 0.24725; // r
	materialAmbiant[1] = 0.1995;  // g
	materialAmbiant[2] = 0.0745;  // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.4 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, -3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 2nd column, silver *****
	// ambient material
	materialAmbiant[0] = 0.19225; // r
	materialAmbiant[1] = 0.19225; // g
	materialAmbiant[2] = 0.19225; // b
	materialAmbiant[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;		// a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.4 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-3.0f, -6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, 6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 3rd column, cyan *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.1;  // g
	materialAmbiant[2] = 0.06; // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.0;		 // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;		 // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50196078; // r
	materialSpecular[1] = 0.50196078; // g
	materialSpecular[2] = 0.50196078; // b
	materialSpecular[3] = 1.0f;		  // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, 3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, green *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, 1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 3rd column, red *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.6;	// g
	materialSpecular[2] = 0.6;	// b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, -1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 3rd column, white *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, -3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 3rd column, yellow plastic *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(3.0f, -6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	// ambient material
	materialAmbiant[0] = 0.02; // r
	materialAmbiant[1] = 0.02; // g
	materialAmbiant[2] = 0.02; // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.4;	// r
	materialSpecular[1] = 0.4;	// g
	materialSpecular[2] = 0.4;	// b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, 6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 4th column, cyan *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, 3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 4th column, green *****
	// ambient material
	materialAmbiant[0] = 0.0;  // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, 1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 4th column, red *****
	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.0;  // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, -1.2f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 4th column, white *****
	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.05; // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.7;	// b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, -3.6f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 4th column, yellow rubber *****
	// ambient material
	materialAmbiant[0] = 0.05; // r
	materialAmbiant[1] = 0.05; // g
	materialAmbiant[2] = 0.0;  // b
	materialAmbiant[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiant);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;	// r
	materialSpecular[1] = 0.7;	// g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShinniness = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShinniness);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(9.0f, -6.0f, -19.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************
	// *******************************************************
	// *******************************************************
}
