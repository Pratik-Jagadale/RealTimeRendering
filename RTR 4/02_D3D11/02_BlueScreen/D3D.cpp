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

// D3D11 RELATED GLOBAL VARIABLES
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;
float clearColor[4];

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
	HRESULT resize(int, int);
	void uninitialize(void);
	HRESULT hr = S_OK;

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
		if (gpID3D11DeviceContext)
		{
			hr = resize(LOWORD(lParam), HIWORD(lParam));
			if (FAILED(hr))
			{
				fopen_s(&gpFile, "Log.txt", "a+");
				fprintf(gpFile, "resize() Failed In WndProc()...\n");
				fclose(gpFile);
				return hr;
			}
		}
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
	HRESULT resize(int width, int height);

	// VARIABLE DECLARTIONS
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_DRIVER_TYPE d3dDriverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, // HARDWARE
		D3D_DRIVER_TYPE_WARP,	  // WARP = WINDOWS ADVANCED RASTERIZER FLATFORM
		D3D_DRIVER_TYPE_REFERENCE // USED FOR DEBUGGING
	};
	D3D_FEATURE_LEVEL d3dFeatureLevel_Required = D3D_FEATURE_LEVEL_11_0; // WANT FEATURE LEVEL 11.0
	D3D_FEATURE_LEVEL d3dFeatureLevel_Acquired = D3D_FEATURE_LEVEL_10_0;
	UINT numDrivers = 0;
	UINT createDeviceFlags = 0;
	UINT numFeatureLevels = 1;
	DXGI_SWAP_CHAIN_DESC dxgiSwapchainDescriptor; // SWAP CHAIN DESCREPTOR
	HRESULT hr;

	// CODE
	hr = printD3DInfo();

	// INITIALIZE SWAP CHAIN DESCREPTOR
	ZeroMemory((void *)&dxgiSwapchainDescriptor, sizeof(DXGI_SWAP_CHAIN_DESC));

	dxgiSwapchainDescriptor.BufferCount = 1; // BY DEFAULT DIRECTX GIVES 1 BUFFER
	dxgiSwapchainDescriptor.BufferDesc.Width = WINWIDTH;
	dxgiSwapchainDescriptor.BufferDesc.Height = WINHEIGHT;
	dxgiSwapchainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // UNORM == USGINED NORMALIZE
	dxgiSwapchainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapchainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapchainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // DXGI_RENDER_TARGET_OUTPUT
	dxgiSwapchainDescriptor.OutputWindow = ghwnd;
	dxgiSwapchainDescriptor.SampleDesc.Count = 1;
	dxgiSwapchainDescriptor.SampleDesc.Quality = 0; // DEFAULT
	dxgiSwapchainDescriptor.Windowed = TRUE;

	// CALL D3D11 CREATE DEICE AND SWAPCHAIN FOR REQUIRED DRIVER
	numDrivers = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);
	for (UINT i = 0; i < numDrivers; i++)
	{
		d3dDriverType = d3dDriverTypes[i];

		hr = D3D11CreateDeviceAndSwapChain(
			NULL, // PRIMARY ADAPTER
			d3dDriverType,
			NULL,
			createDeviceFlags,
			&d3dFeatureLevel_Required,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&dxgiSwapchainDescriptor, // IN PARAM
			&gpIDXGISwapChain,
			&gpID3D11Device,
			&d3dFeatureLevel_Acquired,
			&gpID3D11DeviceContext);

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "D3D11CreateDeviceAndSwapChain() Failed in resize()...\n");
		fclose(gpFile);

		return (hr);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");

		// PRINT OBTAINED TYPE
		if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(gpFile, "\nD3D11 OBTAINED HARDWARE DRIVER.\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(gpFile, "D3D11 OBTAINED WARP DRIVER.\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(gpFile, "D3D11 OBTAINED REFERENCE DRIVER.\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 OBTAINED UNKNOWN DRIVER.\n");
		}

		// PRINT OBTAINED D3D11 FEATURE LEVEL
		if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(gpFile, "D3D11 OBTAINED 11.0 FEATURE LEVEL.\n");
		}
		else if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(gpFile, "D3D11 OBTAINED 10.1 FEATURE LEVEL.\n");
		}
		else if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(gpFile, "D3D11 OBTAINED 10.0 FEATURE LEVEL.\n");
		}
		else
		{
			fprintf(gpFile, "D3D11 OBTAINED UNKNOWN FEATURE LEVEL.\n");
		}

		fclose(gpFile);
	}

	// INITIALIZE CLEAR COLOR ARRAY
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 1.0f; // BLUEW
	clearColor[3] = 1.0f;

	// WARMUP RESIZE
	hr = resize(WINWIDTH, WINHEIGHT);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "resize() Failed in initialize()...\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "resize() Successful in initialize()...\n");
		fclose(gpFile);
	}
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

HRESULT resize(int width, int height)
{
	// VARIABLE DECLARTIONS
	HRESULT hr = S_OK;

	// CODE
	if (height == 0)
		height = 1;

	// RELEASE EXISTING RTV(RENDER TARGET VIEW)
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	// TELL SWAP CHAIN TO RESIZE BUFFERS ACCORDING TO NEW WIDTH AND HEIGHT
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	// GET NEW RESIZED BUFFER FROM SWAP CHAIN INTO A DUMMY TEXTURE
	ID3D11Texture2D *pID3D11Texture2D_BackBuffer = NULL;

	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pID3D11Texture2D_BackBuffer);

	// NOW CREATE NEW RTV USING ABOVE BUFFER
	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer, NULL, &gpID3D11RenderTargetView);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "CreateRenderTargetView() Failed  ...\n");
		fclose(gpFile);
		return (hr);
	}

	// RELEASE DUMMY TEXTURE INTERFACE
	pID3D11Texture2D_BackBuffer->Release();
	pID3D11Texture2D_BackBuffer = NULL;

	// SET THIS NEW RTV IN PIPELINE
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

	// INITIALIZE VIEW PORT STRUCTURE
	D3D11_VIEWPORT d3d11ViewPort;
	d3d11ViewPort.TopLeftX = 0.0f;
	d3d11ViewPort.TopLeftY = 0.0f;
	d3d11ViewPort.Width = (float)width;
	d3d11ViewPort.Height = (float)height;
	d3d11ViewPort.MinDepth = 0.0f;
	d3d11ViewPort.MaxDepth = 0.0f;

	// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
	gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

	return (hr);
}

void display(void)
{
	// VARIABLE DECLARTIONS

	// CODE
	// CLEAR THE RTV USING CLEAR COLOR
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);

	// HERE SHOULD BE THE DRAWINF CODE (12 LAKH LIES)

	// SWAP BUFFFERS BY PRESENTING THEM
	gpIDXGISwapChain->Present(0, // NO NEED OF SYNCRONIZATION WITH MONITOR REFRESH RATE
							  0	 // SWAP WITH ALL BUFFERS IN SWAP CHAIN
	);
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
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	if (gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();
		gpIDXGISwapChain = NULL;
	}

	if (gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();
		gpID3D11DeviceContext = NULL;
	}

	if (gpID3D11Device)
	{
		gpID3D11Device->Release();
		gpID3D11Device = NULL;
	}

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