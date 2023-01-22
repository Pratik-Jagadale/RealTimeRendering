// HEADER FILES
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// OUR HEADER FILES
#include "D3D.h"

// D3D11 RELATED HEADER FILES
#include <d3d11.h>
#include <dxgi.h> // DIRECTX GRAPHICS INFRA STRUCTURE

// D3D RELATED LIBRARIES
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// GLOBAL CONSTANTS (MACROS)
#define WINWIDTH 800
#define WINHEIGHT 600

// GLOBAL FUNCTION DECLARATIONS
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

// GLOBAL VARIABLE DECLARTIONS
HWND ghwnd = NULL;
BOOL gbFullScreen = FALSE;
int iHeightOfWindow;
int iWidthOfWindow;
int iXMyWindow;
int iYMyWindow;
FILE *gpFile = NULL;

// ENTRY POINT FUNCTION
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// FUNCTION DECLARATIONS
	HRESULT initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	// VARIABLE DECLARTIONS
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	HRESULT hr = S_OK;

	// CODE
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
	{
		MessageBox(NULL, TEXT("Creation of Log File Faild..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File is Successfuly Created\n");
		fclose(gpFile);
	}

	// INITIALIZATION OF wndclassex STRUCTURE
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// REGISTER ABOVE wndclass
	RegisterClassEx(&wndclass);

	iHeightOfWindow = GetSystemMetrics(SM_CYSCREEN); // Height of Window Screen
	iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN);	 // Width Of Window Screen

	iXMyWindow = (iWidthOfWindow) / 4;	// x coordinate for MyWindow
	iYMyWindow = (iHeightOfWindow) / 4; // y coordinate for MyWindow

	// CREATE WINDOW
	hwnd = CreateWindow(szAppName,
						TEXT("Pratik Rajendra Jagadale"),
						WS_OVERLAPPEDWINDOW,
						iXMyWindow,
						iYMyWindow,
						(iWidthOfWindow) / 2,
						(iHeightOfWindow) / 2,
						NULL,
						NULL,
						hInstance,
						NULL);

	ghwnd = hwnd;

	// INITIALIZE
	hr = initialize();

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "initialize() Failed...\n");
		fclose(gpFile);
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "initialize() Successful...\n");
		fclose(gpFile);
	}

	ShowWindow(hwnd, iCmdShow);

	// FOR GROUNDING AND FOCUSING WINDOW
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// GAME LOOP
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
				// RENDER THE SCENE
				display();

				// UPDATE THE SCENE
				update();
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);
}

// CALLBACK FUNCTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// FUNCTION DECLARATIONS
	void ToggleFullScreen();
	void resize(int, int);
	void uninitialize(void);

	// CODE
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		break; // AS THIS IS RETAINED MODE GRAPHICS THERE IS WM_PAINNT TO PAINT

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
		}
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	// VARIABLE DECLARTIONS
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	//	CODE
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

HRESULT initialize(void)
{
	// FUNCTION DECLARATIONS
	HRESULT printD3DInfo();

	// VARIABLE DECLARTIONS
	HRESULT hr = S_OK;

	// CODE
	hr = printD3DInfo();

	return (hr);
}

HRESULT printD3DInfo()
{
	// VARIABLE DECLARTIONS
	HRESULT hr = S_OK;
	IDXGIFactory *pIDXGIFactory = NULL;
	IDXGIAdapter *pIDXGIAdapter = NULL;
	DXGI_ADAPTER_DESC dxgiAdapterDescriptor;
	char str[255];

	// CODE
	// CREATE DXGIFACTORY
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "CreateDXGIFactory() Failed...\n");
		fclose(gpFile);
		return (hr);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "CreateDXGIFactory() Succeeded...\n");
		fclose(gpFile);
	}

	// GET IDXGIADAPTER
	if (pIDXGIFactory->EnumAdapters(0, /* 0 TH GRAPHICS CARD ADPTER*/ &pIDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		// GET DESCREPTION OF THE FOUND ADAPTER
		ZeroMemory((void *)&dxgiAdapterDescriptor, sizeof(DXGI_ADAPTER_DESC));

		pIDXGIAdapter->GetDesc(&dxgiAdapterDescriptor);

		WideCharToMultiByte(CP_ACP,							   /* ANCII */
							0,								   /* NO NEED OF CONVERSION OF LANG*/
							dxgiAdapterDescriptor.Description, /* INPUT (WHICH) */
							255,							   /* CONVERSION INPUT SIZE */
							str,							   /* OUTPUT */
							255,							   /* OUTPUT SIZE*/
							NULL,							   /* DEPEND ON 2 ND PARAM  */
							NULL							   /* DEPEND ON 2 ND PARAM */
		);

		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "===================================================================\n");
		fprintf(gpFile, "GRAPHICS CARD NAME 	= %s\n", str);
		fprintf(gpFile, "GRAPHICS CARD MEMORY 	= %dGB\n", (int)ceil(dxgiAdapterDescriptor.DedicatedVideoMemory / 1024.0 /* FOR KB */ / 1024.0 /* FOR MB */ / 1024.0 /* FOR GB*/));
		fprintf(gpFile, "===================================================================\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "pIDXGIFactory->EnumAdapters() Failed...\n");
		fclose(gpFile);
	}

	if (pIDXGIAdapter)
	{
		pIDXGIAdapter->Release();
		pIDXGIAdapter = NULL;
	}

	if (pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}

	return hr;
}

void resize(int width, int height)
{
	// CODE
	if (height == 0)
		height = 1;
}

void display(void)
{
	// CODE
}

void update(void)
{
	// CODE
}

void uninitialize(void)
{
	// FUNCTION DECLARATIONS
	void ToggleFullScreen(void);

	// CODE
	if (gbFullScreen)
		ToggleFullScreen();

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

//////////////////////////////////////////////////////
// Books
// Introduction to 3D Game Programming with DirectX 11
// MSDN
/////////////////////////////////////////////////////