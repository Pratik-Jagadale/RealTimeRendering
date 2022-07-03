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

// OpenAl Related hader Files
#include <AL/alut.h>

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

// OpelAl- realted variables declartions
// Global so that we can access them in intizlizeAudi() and UninitizaizeAudio()
ALCenum error; // ALcenum typedef of enum, error checking of ALUT and OpenGL functions
ALuint audioBuffer, audioSource;

// for lettres movements
float pos_I_ONE = -5.0f;
float pos_N = 9.0f;
float pos_D = 0.0f;
float pos_I_TWO = -9.0f;
float pos_A = 7.0f;

// for A "-" blend effect
float horizontalLine_A = 0.0f;

// for jet movements
float arpl_A = -10.5f;
float arpl_B = -10.5f;
float arpl_C = -10.5f;
float arpl_A_Y = 5.0f;
float arpl_C_Y = -5.0f;
float jetAngleA = 270.0f;
float jetAngleC = 90.0f;

// for jet booster
float y = 0.67f;
float y_one = 0.67f;
float y_TWO = 0.300f;

// for Z coordinate of all letters
float zCord = 0.0f;

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

void initializationAudio(void)
{
	// function prototype
	void uninitializeAudio(void);

	// code
	// STEP-1 Initizalizing ALUT
	alutInit(NULL, NULL);
	error = alutGetError(); // for capture error related to alutgetError()
	if (error != ALUT_ERROR_NO_ERROR)
	{
		printf("AlutInit() Failed.\n");
		uninitializeAudio();
		return;
	}

	// STEP-2 Create Audio Buffer From Audio File
	audioBuffer = alutCreateBufferFromFile("Ye_Mere_Watan_Ke_Logo.wav");
	error = alutGetError();
	if (error != ALUT_ERROR_NO_ERROR)
	{
		printf("alutGetError() Failed. Creation of Audio Buffer Failed.\n");
		uninitializeAudio();
		return;
	}

	// STEP-3 Create A Audio Source
	alGenSources(1, &audioSource); // First OpenAl function - Openl Al audio Source Genrate kar // jar muiltiple audio create karyche astil tar sources cha array daycha
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("alGenSources(Failed : Genertaing audio Source failed.\n");
		uninitializeAudio();
		return;
	}

	// STEP-4 attach the above created audio buffer to above audio source
	alSourcei(audioSource, AL_BUFFER, audioBuffer); // i for integer if(integer then iv)( 1st - > which  audio soucre , 2 nd - what have to attach , 3 rd - which buffer )
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("alSourcei()  Failed : attaching audio buffer to audio source failed.\n");
		uninitializeAudio();
		return;
	}

	// STEP-5 play audio from the source
	alSourcePlay(audioSource);
	if (error != AL_NO_ERROR)
	{
		printf("alSourcePlay() Failed.\n");
		uninitializeAudio();
		return;
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

void initiallize(void)
{
	// 	Function Prototype
	void resize(int, int);
	Bool loadGLTexture(GLuint *, const char *);
	void initializationAudio(void);

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

	// intiallize audio
	initializationAudio();

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
	/* code */
	y = y - 0.05;
	if (y < 0.300)
		y = 0.67f;

	y_one = y_one - 0.001;
	if (y_one < 0.300)
		y_one = 0.67f;

	y_TWO = y_TWO + 0.001;
	if (y_TWO > 0.67f)
		y_TWO = 0.300f;

	if (pos_I_ONE < 0.0f)
		pos_I_ONE = pos_I_ONE + 0.01f;
	else if (pos_N > 0.0f)
		pos_N = pos_N - 0.01f;
	else if (pos_D < 255.0f)
		pos_D = pos_D + 1.1f;
	else if (pos_I_TWO < 0.0f)
		pos_I_TWO = pos_I_TWO + 0.01f;
	else if (pos_A > 0.0f)
		pos_A = pos_A - 0.01f;
	else if (arpl_B < 12.5f)
	{
		arpl_B = arpl_B + 0.01f;
		arpl_A = arpl_A + 0.01f;
		arpl_C = arpl_C + 0.01f;

		if (arpl_B < -4.0f)
		{
			if (arpl_A_Y > 0.0f)
				arpl_A_Y = arpl_A_Y - 0.01f;

			if (jetAngleA < 360.0f)
				jetAngleA = jetAngleA + 0.17f;

			if (jetAngleC > 0.0f)
				jetAngleC = jetAngleC - 0.17f;

			if (arpl_C_Y < 0.0f)
				arpl_C_Y = arpl_C_Y + 0.01;
		}
		else if (arpl_B > 7.5f)
		{

			if (arpl_A_Y < 5.0f)
				arpl_A_Y = arpl_A_Y + 0.01f;

			if (jetAngleA < 90.0f)
				jetAngleA = jetAngleA + 0.17f;
			else
				jetAngleA = 0.0f;

			if (jetAngleC > 270.0f)
				jetAngleC = jetAngleC - 0.17f;
			else
				jetAngleC = 360.0f;

			if (arpl_C_Y > -5.0)
				arpl_C_Y = arpl_C_Y - 0.01;
		}
		else if (arpl_B > 6.3f)
			horizontalLine_A = horizontalLine_A + 2.0f;
	}
	else
	{
		zCord = zCord + 0.01f;
	}
}

void uninitiallize(void)
{

	void uninitializeAudio(void);

	// code
	GLXContext currentContext;
	currentContext = glXGetCurrentContext();

	uninitializeAudio();

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

void uninitializeAudio(void)
{
	// variabl declartions
	ALint state;

	// code
	// stop Playing the Audio From the Source
	alGetSourcei(audioSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
	{
		alSourceStop(audioSource);
		error = alGetError();
		if (error != AL_NO_ERROR)
		{
			printf("alSourceStop() Failed.\n");
		}
	}

	// Detach The Audio Buffer From the Audio Souce
	alSourcei(audioSource, AL_BUFFER, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("alSourcei() Failed : attaching Audio Buffer To Audio Source Failed.\n");
	}

	// Delete The Audio Source
	alDeleteSources(1, &audioSource);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("alDeleteSources() Failed : Deleting Audio Source Failed.\n");
	}
	else
		audioSource = 0;

	// delete the audio Buffer
	alDeleteBuffers(1, &audioBuffer);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("alDeleteBuffers() Failed : Deleting Audio Buffer Failed.\n");
	}
	else
		audioSource = 0;

	// Uninitilize ALUT
	alutExit();
}

void draw(void)
{
	/* Function protype */
	void drawIndia(void);
	void fighterJet(void);
	void drawIAF(void);

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -12.0f);

	drawIndia();

	// Plane B
	glLoadIdentity();
	glTranslatef(0.0f + arpl_B, -0.1f, -12.0f);
	glScalef(0.30f, 0.30f, 0.30f);

	fighterJet();

	glLoadIdentity();
	glTranslatef(-0.8f + arpl_B, -0.1f, -12.0f);
	glScalef(0.15f, 0.15f, 0.15f);
	drawIAF();

	// Plane A
	glLoadIdentity();

	glTranslatef(-1.0f + arpl_A, 1.5f + arpl_A_Y, -12.0f);
	glRotatef(jetAngleA, 0.0f, 0.0f, 1.0f);
	glScalef(0.20f, 0.20f, 0.20f);

	fighterJet();

	glLoadIdentity();

	glTranslatef(-1.0f + arpl_A, 1.5f + arpl_A_Y, -12.0f);
	glRotatef(jetAngleA, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, 0.0f, 0.0f);
	glScalef(0.10f, 0.10f, 0.10f);
	drawIAF();

	// Plane C
	glLoadIdentity();
	glTranslatef(-1.0f + arpl_C, -1.65f + arpl_C_Y, -12.0f);
	glRotatef(jetAngleC, 0.0f, 0.0f, 1.0f);
	glScalef(0.20f, 0.20f, 0.20f);

	fighterJet();

	glLoadIdentity();
	glTranslatef(-1.0f + arpl_C, -1.65f + arpl_C_Y, -12.0f);
	glRotatef(jetAngleC, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, 0.0f, 0.0f);
	glScalef(0.10f, 0.10f, 0.10f);
	drawIAF();

	glXSwapBuffers(display, window);
}

void setColor(float r, float g, float b)
{
	glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void setColor4f(float r, float g, float b, float alpha)
{
	glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f);
}

void drawIndia(void)
{
	// I
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(-8.0f + pos_I_ONE, 0.0f, 0.0f + zCord);

	glBegin(GL_QUADS);
	setColor(255, 153, 51);
	glVertex3f(-1.5f, 2.0f, 0.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 2.0f, 0.0f);

	setColor(19, 136, 8);
	glVertex3f(-1.5f, -2.0f, 0.0f);
	glVertex3f(-1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -2.0f, 0.0f);

	setColor(255, 153, 51);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	// 	N
	// glTranslatef(4.0f, 0.0f + pos_N, 0.0f);
	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(-4.0f, 0.0f + pos_N, -12.0f + zCord);

	glBegin(GL_QUADS);

	// "\"
	// bck
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	// bck
	setColor(19, 136, 8);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glVertex3f(2.0f, -2.0f, 0.0f);

	// frnt
	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(2.0f, -1.0f, 0.0f);

	// "| -" 1
	// bck
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// 2
	// bck
	setColor(19, 136, 8);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// fnt
	setColor(255, 255, 255);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	// "- |"
	// bcknd
	setColor(255, 153, 51);
	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 2.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	// frnt
	setColor(255, 153, 51);

	glVertex3f(2.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	// bkdn
	setColor(19, 136, 8);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glVertex3f(2.0f, -2.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(2.0f, -1.0f, 0.0f);

	glEnd();

	//	D
	//	glTranslatef(4.5f, 0.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(0.5f, 0.0f, -12.0f + zCord);

	glBegin(GL_QUADS);
	// bck
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-1.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// frnt
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// bck
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// frnt
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 0.0f);
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	// "-" TOP HORIZONTAL LINE
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(-2.0f, 2.0f, 0.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 2.0f, 0.0f);

	// "_" BOTOM HORIZONTAL LINE
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -2.0f, 0.0f);

	//"-|" RIGHT VERTICAL LINE
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	setColor4f(255, 255, 255, pos_D);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, 0.0f, 0.0f);

	setColor4f(255, 255, 255, pos_D);
	glVertex3f(1.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);

	// TOP TRAINGLE FOR D using QUAD
	setColor4f(255, 153, 51, pos_D);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);

	// BOTTOM TRAINGLE FOR D using QUAD
	setColor4f(19, 136, 8, pos_D);
	glVertex3f(0.5f, -2.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

	// I
	//	glTranslatef(3.3f, 0.0f + pos_I_TWO, 0.0f);

	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(3.8f, 0.0f + pos_I_TWO, -12.0f + zCord);

	glBegin(GL_QUADS);
	setColor(255, 153, 51);

	glVertex3f(-1.5f, 2.0f, 0.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 2.0f, 0.0f);

	setColor(19, 136, 8);
	glVertex3f(-1.5f, -2.0f, 0.0f);
	glVertex3f(-1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, 0.0f);
	glVertex3f(1.5f, -2.0f, 0.0f);

	setColor(255, 153, 51);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 1.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-0.5f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();

	// A
	//	glTranslatef(3.6f + pos_A, 0.0f, 0.0f);
	glLoadIdentity();
	glScalef(0.75f, 0.75f, 1.0f);
	glTranslatef(7.4f + pos_A, 0.0f, -12.0f + zCord);

	// " - "
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	setColor4f(255, 153, 51, horizontalLine_A);
	glVertex3f(0.25f, 0.0f + 0.4f, 0.0f);
	glVertex3f(-0.25f, 0.0f + 0.4f, 0.0f);
	setColor4f(255, 2555, 255, horizontalLine_A);
	glVertex3f(-0.45f, -0.5f + 0.4f, 0.0f);
	glVertex3f(0.45f, -0.5f + 0.4f, 0.0f);

	glVertex3f(0.45f, -0.5f + 0.4f, 0.0f);
	glVertex3f(-0.45f, -0.5f + 0.4f, 0.0f);
	setColor4f(152, 251, 152, horizontalLine_A);
	glVertex3f(-0.65f, -1.0f + 0.4f, 0.0f);
	glVertex3f(0.65f, -1.0f + 0.4f, 0.0f);

	glEnd();
	glDisable(GL_BLEND);

	glBegin(GL_QUADS);
	// "/"
	setColor4f(255, 153, 51, 255);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 2.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(-1.25f, 0.0f, 0.0f);
	glVertex3f(-0.25f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(-0.25f, 0.0f, 0.0f);
	glVertex3f(-1.25f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(-2.0f, -2.0f, 0.0f);
	glVertex3f(-1.0f, -2.0f, 0.0f);

	// "\"
	setColor(255, 153, 51);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glVertex3f(-0.5f, 2.0f, 0.0f);
	setColor(255, 255, 255);
	glVertex3f(0.25f, 0.0f, 0.0f);
	glVertex3f(1.25f, 0.0f, 0.0f);

	setColor(255, 255, 255);
	glVertex3f(0.25f, 0.0f, 0.0f);
	glVertex3f(1.25f, 0.0f, 0.0f);
	setColor(19, 136, 8);
	glVertex3f(2.0f, -2.0f, 0.0f);
	glVertex3f(1.0f, -2.0f, 0.0f);
	glEnd();
}

void fighterJet(void)
{

	// top nitro
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3);
	glBegin(GL_LINES);
	setColor4f(255, 153, 51, 255.0f);
	glVertex3f(-4.20, y, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y, 0.0f);

	setColor4f(255, 153, 51, 255.5f);
	glVertex3f(-4.200, y_one, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y_one, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.200, y_TWO, 0.0f);
	setColor4f(19, 136, 8, 0.5f);
	glVertex3f(-8.0, y_TWO, 0.0f);

	glEnd();

	// bottom nitro
	glBegin(GL_LINES);
	setColor4f(255, 153, 51, 255.5f);
	glVertex3f(-4.200, -y_one, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, -y_one, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.20, y - 0.9f, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y - 0.9f, 0.0f);

	setColor4f(19, 136, 8, 255);
	glVertex3f(-4.20, y_one - 0.9f, 0.0f);
	setColor4f(255, 153, 51, 0.5f);
	glVertex3f(-8.0, y_one - 0.9f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

	///////////////// top wing
	glBegin(GL_POLYGON);
	setColor(186, 226, 238);
	// setColor(0, 212, 250);
	glVertex3f(1.36f, 0.760f, 0.0f);
	glVertex3f(0.6f, 1.597f, 0.0f);
	glVertex3f(0.24f, 1.613f, 0.0f);
	glVertex3f(0.56f, 0.818f, 0.0f);
	glEnd();

	// bottom wing
	glBegin(GL_POLYGON);
	glVertex3f(1.36, -0.733, 0.0f);
	glVertex3f(0.443, -0.9, 0.0f);
	glVertex3f(0.251, -1.565, 0.0f);
	glVertex3f(0.568, -1.5664, 0.0f);
	glEnd();

	////////////////
	// tip
	glBegin(GL_POLYGON);
	glVertex3f(4.394, 0.014, 0.0f);
	glVertex3f(3.618, 0.224, 0.0f);
	glVertex3f(2.962, 0.331, 0.0f);
	glVertex3f(2.175, 0.5325, 0.0f);
	glVertex3f(1.512, 0.640, 0.0f);
	glVertex3f(1.138, 0.650, 0.0f);
	glVertex3f(1.138, -0.650, 0.0f);
	glVertex3f(1.437, -0.640, 0.0f);
	glVertex3f(1.726, -0.600, 0.0f);
	glVertex3f(2.423, -0.404, 0.0f);
	glVertex3f(2.823, -0.312, 0.0f);
	glVertex3f(3.513, -0.206, 0.0f);
	glVertex3f(3.931, -0.125, 0.0f);
	glVertex3f(4.39, 0.008000, 0.0f);
	glEnd();

	// back
	glBegin(GL_POLYGON);
	glVertex3f(-0.256, 1.155, 0.0f);
	glVertex3f(-2.242, 3.127, 0.0f);
	glVertex3f(-2.279, 3.1546, 0.0f);
	glVertex3f(-3.194, 3.183, 0.0f);
	glVertex3f(-3.351, 0.93, 0.0f);
	glVertex3f(-3.7883, 0.9295, 0.0f);
	glVertex3f(-3.7955, 0.7366, 0.0f);
	glVertex3f(-4.1017, 0.672, 0.0f);
	glVertex3f(-4.098, 0.2597, 0.0f);
	glVertex3f(-3.808, 0.211, 0.0f);
	glVertex3f(-4.091, 0.193, 0.0f);
	glVertex3f(-4.093, 0.141, 0.0f);
	glVertex3f(-4.248, 0.1155, 0.0f);
	glVertex3f(-4.258, 0.078, 0.0f);
	glVertex3f(-4.732, 0.0725, 0.0f);
	glVertex3f(-4.74, -0.026, 0.0f);
	glVertex3f(-4.267, -0.0294, 0.0f);
	glVertex3f(-4.2404, -0.0754, 0.0f);
	glVertex3f(-4.0995, -0.092, 0.0f);
	glVertex3f(-4.0966, -0.1646, 0.0f);
	glVertex3f(-3.8896, -0.1804, 0.0f);
	glVertex3f(-4.096, -0.229, 0.0f);
	glVertex3f(-4.0976, -0.642, 0.0f);
	glVertex3f(-3.798, -0.6916, 0.0f);
	glVertex3f(-3.7935, -0.884, 0.0f);
	glVertex3f(-3.348, -0.887, 0.0f);
	glVertex3f(-3.189, -3.0945, 0.0f);
	glVertex3f(-2.2505, -3.109, 0.0f);
	glVertex3f(-0.256, -1.11300, 0.0f);
	glEnd();

	// middle
	glBegin(GL_POLYGON);
	glVertex3f(1.39, 0.76, 0.0f);
	glVertex3f(-0.256, 1.155, 0.0f);
	glVertex3f(-0.256, -1.11300, 0.0f);
	glVertex3f(1.39, -0.733, 0.0f);

	glEnd();

	glLineWidth(1);

	glBegin(GL_LINE_LOOP);
	setColor(0, 0, 0);
	glVertex3f(2.972, 0.15500, 0.0f);
	glVertex3f(2.276, 0.283, 0.0f);
	glVertex3f(0.772, 0.298, 0.0f);
	glVertex3f(0.651, 0.21, 0.0f);
	glVertex3f(0.563, 0.112, 0.0f);
	glVertex3f(0.552, -0.034, 0.0f);
	glVertex3f(0.627, -0.16, 0.0f);
	glVertex3f(0.778, -0.264, 0.0f);
	glVertex3f(2.21, -0.253, 0.0f);
	glVertex3f(2.979, -0.121, 0.0f);
	glVertex3f(3.007, 0, 0.0f);
	glEnd();

	setColor(0, 0, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.556, 0.803, 0.0f);
	glVertex3f(-0.26, 1.15, 0.0f);
	glVertex3f(-2.826, 1.143, 0.0f);
	glVertex3f(-2.82, 0.94, 0.0f);
	glVertex3f(-3.788, 0.918, 0.0f);
	glVertex3f(-3.788, -0.87, 0.0f);
	glVertex3f(-2.81, -0.888, 0.0f);
	glVertex3f(-2.804, -1.109, 0.0f);
	glVertex3f(-0.242, -1.106, 0.0f);
	glVertex3f(0.569, -0.77, 0.0f);

	glEnd();

	glLineWidth(10);
	glBegin(GL_LINES);
	setColor(186, 226, 238);
	glVertex3f(-1.754, 3.2, 0.0f);
	glVertex3f(-3.379, 3.2, 0.0f);

	glVertex3f(-1.754 + 1.0f, 3.2 - 1.0f, 0.0f);
	glVertex3f(-3.379 + 1.0f, 3.2 - 1.0f, 0.0f);

	glVertex3f(-1.754, -3.1, 0.0f);
	glVertex3f(-3.379, -3.1, 0.0f);

	glVertex3f(-1.754 + 1.0f, -3.1 + 1.0f, 0.0f);
	glVertex3f(-3.379 + 1.0f, -3.1 + 1.0f, 0.0f);

	glEnd();

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);

	glVertex3f(-2.803, 3.128, 0.0f);
	glVertex3f(-3.14, 3.132, 0.0f);
	glVertex3f(-3.308, 0.926, 0.0f);
	glVertex3f(-2.808, 0.932, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glVertex3f(-2.803, -3.128, 0.0f);
	glVertex3f(-3.14, -3.132, 0.0f);
	glVertex3f(-3.308, -0.926, 0.0f);
	glVertex3f(-2.808, -0.932, 0.0f);
	glEnd();

	glScalef(0.75f, 0.75f, 0.75f);
	glTranslatef(0.3f, 0.0f, 0.0f);

	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	setColor(0, 212, 250);
	glVertex3f(2.972, 0.15500, 0.0f);
	glVertex3f(2.276, 0.283, 0.0f);
	glVertex3f(0.772, 0.298, 0.0f);
	glVertex3f(0.651, 0.21, 0.0f);
	glVertex3f(0.563, 0.112, 0.0f);
	glVertex3f(0.552, -0.034, 0.0f);
	glVertex3f(0.627, -0.16, 0.0f);
	glVertex3f(0.778, -0.264, 0.0f);
	glVertex3f(2.21, -0.253, 0.0f);
	glVertex3f(2.979, -0.121, 0.0f);
	glVertex3f(3.007, 0, 0.0f);
	glEnd();
}

void drawIAF(void)
{
	void drawI(void);
	void drawA(void);
	void drawF(void);

	glLineWidth(3);
	drawI();
	glTranslatef(2.2f, 0.0f, 0.0f);
	drawA();

	glTranslatef(2.5f, 0.0f, 0.0f);
	drawF();
}

void drawI(void)
{
	glBegin(GL_LINES);
	setColor(255, 153, 51);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}
void drawA(void)
{
	glBegin(GL_LINES);
	setColor(255, 255, 255);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();
}

void drawF(void)
{
	glBegin(GL_LINES);
	setColor(19, 136, 8);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}
