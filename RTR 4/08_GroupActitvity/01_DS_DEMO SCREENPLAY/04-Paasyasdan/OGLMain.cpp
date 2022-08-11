/* Header Files */
#include <windows.h>
#include "OGLMain.h"
#include <stdio.h>
#include <stdlib.h>
#include "helper_timer.h"

#include "./Models/OGLSCENE1.h"
#include "./Models/OGLSCENE2.h"
#include "./Models/OGLSCENE3.h"

/* OpenGL Header files */
#include <GL/gl.h>
#include <GL/glu.h> //graphics library utillity

/* OpenAL Header Files */
#include <al.h>
#include <alc.h>

/* OpenGL libraries */
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

/* OpenAL libraries*/
#pragma comment(lib, "OpenAL32.lib")

#define WINWIDTH 800
#define WINHEIGHT 600

#define SCENE_01_DURATION 45
#define SCENE_02_DURATION 39
#define SCENE_03_DURATION 32

BOOL InitializeAudio(void);
void UninitializeAudio(void);

// global variable declarations

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

BOOL gbFullScreen = FALSE;
int iHeightOfWindow;
int iWidthOfWindow;
FILE *gpFile = NULL; // FILE* -> #include<stdio.h>
BOOL gbActiveWindow = FALSE;

StopWatchInterface *pTimer;

// OpenAL realted global Variables
ALuint audioBuffer;
ALuint audioSource;

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();

BOOL bScene01Done = TRUE;
BOOL bScene02Done = TRUE;
BOOL bScene03Done = TRUE;
BOOL bScene04Done = TRUE;
BOOL bScene05Done = TRUE;

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);
	void ToggleFullScreen();

	/* variable declarations */
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	// Code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
	{
		MessageBox(NULL, TEXT("Creation of Log File Faile..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
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
						  TEXT("Pratik Jagdale : FFP DS Demo - Paayasdan"),
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
		fprintf(gpFile, "Makeing OpnGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "loadGLTexture Failed for texture_Smiley ...\n");
		uninitialize();
	}
	else if (iRetVal == -15)
	{
		fprintf(gpFile, "initialzieAudio() Failed ...\n");
		uninitialize();
	}
	else if (iRetVal == -16)
	{
		fprintf(gpFile, "initialzieAudio() Succesffull ");
	}
	else
	{
		fprintf(gpFile, "Initialize Successfull ...\n");
	}

	ShowWindow(hwnd, iCmdShow);

	/* fore grounding and focusing window */
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	ToggleFullScreen();

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
				/* Render the scene */
				display();
				// update the scene
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
			break;
		}
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
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
	BOOL LoadGLTexture(GLuint *, TCHAR[]);
	void uninitialize(void);

	/* variable declartions */
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	/* code */
	/* initialization of pixelformatdesciptor structure */
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32; // 24 also can done

	/* GetDC */
	ghdc = GetDC(ghwnd);

	/* Choose Pixel Format */
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
		return -1;

	/* Set The choosen Puxel Format */
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return -2;

	/* briding API */
	/* Create OpenGL Rendering Context */

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return -3;

	/* make the rendering as current cintext */
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return -4;

	/* Here start OpeGL Code */
	/* Clear the  screen using black color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enabaling the texture
	glEnable(GL_TEXTURE_2D);

	// Depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (initializeSceneOne() == FALSE)
	{
		fprintf(gpFile, "Initizaliztion SCENE ONE FAILED...\n");
		uninitializeSceneOneSceneOne();
		uninitialize();
		return -5;
	}

	if (initializeSceneTwo() == FALSE)
	{
		fprintf(gpFile, "Initizaliztion initializeSceneTwo FAILED...\n");
		uninitializeSceneTwo();
		uninitialize();
		return -6;
	}

	if (initializeSceneThree() == FALSE)
	{
		fprintf(gpFile, "Initizaliztion initializeSceneThree FAILED...\n");
		uninitialize();
		return -6;
	}

	if (InitializeAudio() == FALSE)
	{
		return -16;
	}

	alSourcePlay(audioSource);
	if (alGetError() != AL_NO_ERROR)
		return -17;	

	resize(WINWIDTH, WINHEIGHT); // WARMUP RESIZE CALL

	return (0);
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // To AVOID DEVIDED BY ZERO
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

void display(void)
{
	// Variable declarations

	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (bScene01Done == TRUE)
	{
		displaySceneOne();
	}
	else if (bScene02Done == TRUE)
	{
		displaySceneTwo();
	}
	else if (bScene03Done == TRUE)
	{
		displaySceneThree();
	}
	else if (bScene04Done == TRUE)
	{
		Credits();
	}
	else if (bScene05Done == TRUE)
	{
	}

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */

	if (bScene01Done == TRUE)
		updateSceneOne();
	else if (bScene02Done == TRUE)
	{
		updateSceneTwo();
	}
	else if (bScene03Done == TRUE)
	{
		updateSceneThree();
	}
	else if (bScene04Done == TRUE)
	{
		updateSceneOne();
	}
	else if (bScene05Done == TRUE)
	{
	}
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	uninitializeSceneTwo();

	uninitializeSceneOneSceneOne();

	UninitializeAudio();

	if (pTimer)
	{
		sdkStopTimer(&pTimer);
		sdkDeleteTimer(&pTimer);
		pTimer = NULL;
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

BOOL InitializeAudio(void)
{
	// variable declartions
	ALCdevice *audioDevice;
	ALCcontext *alContext;

	HRSRC hResource = NULL;
	HANDLE hAudioBuffer = NULL;

	LPVOID audioBufferData = NULL;
	DWORD sizeOfAudioDatainBytes = 0;

	// code
	audioDevice = alcOpenDevice(NULL); // Open deafult Audio device
	if (alcGetError(audioDevice) != ALC_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alCOpenDevice() Failed.\n");
		return FALSE;
		// alcGetError() checks Errors for 'alc' functions call just above
	}

	// Step - 2 # CreateOpenAL context.
	alContext = alcCreateContext(audioDevice, NULL); // NULL for default
	if (alcGetError(audioDevice) != ALC_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alcCreateContext() Failed.\n");
		return FALSE;
	}

	// Step - 3 # similar to wglMakeCurrentcontext(). Make above context as current context
	alcMakeContextCurrent(alContext);
	if (alcGetError(audioDevice) != ALC_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alcMakeContextCurrent() Failed.\n");
		return FALSE;
	}

	// STEP - # createAudioBuffer
	alGenBuffers(1, &audioBuffer);
	if (alcGetError(audioDevice) != ALC_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alGenBuffer() Failed.\n");
		return FALSE;
		// alGetError() for error check funcion for above 'al' function.
	}

	// STEP # 5 LOAD WAV RESOURCE DATA
	// [A]: Find WAV Resource, and get its handle
	// Load WAV Resource
	hResource = FindResource(NULL, MAKEINTRESOURCE(ID_MUSIC), TEXT("WAVE")); // kontya exe madhe, resource, type of respurce
	if (hResource == NULL)
	{
		fprintf(gpFile, "InitalizeAudio() : FindResource() Failed.\n");
		return FALSE;
	}

	// 5 [B] Use the Resourec data in memory and returns the handle.
	hAudioBuffer = LoadResource(NULL, hResource);
	if (hAudioBuffer == NULL)
	{
		fprintf(gpFile, "InitalizeAudio() : LoadResource() Failed.\n");
		return FALSE;
	}

	// C : Get starting byte-offset of this data in memory"Starting address bhetato"
	audioBufferData = LockResource(hAudioBuffer);
	if (audioBufferData == NULL)
	{
		fprintf(gpFile, "InitalizeAudio() : LockResource() Failed.\n");
		return FALSE;
	}

	// D: Get the size of  our Audio data in Bytes.
	sizeOfAudioDatainBytes = SizeofResource(NULL, hResource);
	if (sizeOfAudioDatainBytes == 0)
	{
		fprintf(gpFile, "InitalizeAudio() : SizeofResource() Failed.\n");
		return FALSE;
	}

	// # STEP # 6: load the wave data into audio buffer
	// Loadf Buffer Data Into Audio Buffer
	alBufferData(audioBuffer, AL_FORMAT_STEREO16, audioBufferData, sizeOfAudioDatainBytes, 48000); // frequency hrtz
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alBufferData() Failed.\n");
		return FALSE;
	}

	// set 7 create audio source
	alGenSources(1, &audioSource);
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alGenSource() Failed.\n");
		return FALSE;
	}

	// step 8 - give audio buffer to audio source
	alSourcei(audioSource, AL_BUFFER, audioBuffer);
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(gpFile, "InitalizeAudio() : alSourcei() Failed.\n");
		return FALSE;
	}

	// UNLOAD WAV RESOURCE
	// UnlockResource(hResource);
	// FreeResource(hAudioBuffer);

	return (TRUE);
}

void UninitializeAudio(void)
{
	// variable declartions
	ALCdevice *audioDevice;
	ALCcontext *alContext;

	ALint state;

	// code
	// step - get current status of audio playing or not
	alGetSourcei(audioSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
	{
		alSourceStop(audioSource);
		if (alGetError() != AL_NO_ERROR)
			fprintf(gpFile, "UninitializeAudio() : alSourceStop() Failed.\n");
	}

	alSourcei(audioSource, AL_BUFFER, 0);
	if (alGetError() != AL_NO_ERROR)
		fprintf(gpFile, "UninitializeAudio() : alSourcei() Failed.\n");

	alDeleteSources(1, &audioSource);
	if (alGetError() != AL_NO_ERROR)
		fprintf(gpFile, "UninitializeAudio() : alDeleteSpurces() Failed.\n");
	else
		audioSource = 0;

	alDeleteBuffers(1, &audioBuffer);
	if (alGetError() != AL_NO_ERROR)
		fprintf(gpFile, "UninitializeAudio() : alDeleteBuffers() Failed.\n");
	else
		audioBuffer = 0;

	alContext = alcGetCurrentContext();
	if (alContext == NULL)
		fprintf(gpFile, "UninitializeAudio() : alcGetCurrentContext() Failed.\n");
	else
	{
		audioDevice = alcGetContextsDevice(alContext);
		if (audioDevice == NULL)
			fprintf(gpFile, "UninitializeAudio() : alcGetContextDevice() Failed.\n");
	}

	if (alContext)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(alContext);
		alContext = NULL;
	}

	if (audioDevice)
	{
		alcCloseDevice(audioDevice);
		audioDevice = NULL;
	}
}
