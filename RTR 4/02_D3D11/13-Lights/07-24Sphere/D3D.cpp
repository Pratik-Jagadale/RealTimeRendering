// HEADER FILES
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// D3D11 RELATED HEADER FILES
#include <d3d11.h>
#include <dxgi.h> // DIRECTX GRAPHICS INFRA STRUCTURE
#include <d3dcompiler.h>

// OUR HEADER FILES
#include "D3D.h"
#pragma warning(disable : 4838)
#include ".\XNAMath_204\xnamath.h"

#include "Sphere.h"

// D3D RELATED LIBRARIES
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "Sphere.lib")

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

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer_Sphere = NULL;
ID3D11Buffer *gpID3D11Buffer_NormalBuffer_Sphere = NULL;
ID3D11Buffer *gpID3D11Buffer_IndexBuffer_Sphere = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11RasterizerState *gpID3D11RasterizerState = NULL;

ID3D11DepthStencilView *gpID3D11DepthStencilView = NULL;

float clearColor[4];

XMMATRIX perspectiveProjectionMatrix;

struct CBUFFER
{
	XMMATRIX wordMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMVECTOR la, ld, ls, lightPosition, ka, kd, ks;
	float materialShininess;
	unsigned int lightEnabled;
};

float lightAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
float lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightPositions[] = {100.0f, 100.0f, -100.0f, 1.0f};

float materialAmbiant[] = {0.0f, 0.0f, 0.0f, 1.0f};
float materialDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
float materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
float materialShinniness = 50.0f;
BOOL bLight = FALSE;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

int currentWindowWidth = 0;
int currentWindowHeight = 0;
float angleForRotation = 0.0f;
int keyPressed = 0;

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

		case 'x':
		case 'X':
			keyPressed = 1;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'y':
		case 'Y':
			keyPressed = 2;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'z':
		case 'Z':
			keyPressed = 3;
			angleForRotation = 0.0f; // reset
			lightPositions[0] = 0;
			lightPositions[1] = 0;
			lightPositions[2] = 0;
			break;

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
				fopen_s(&gpFile, "Log.txt", "a+");
				fprintf(gpFile, "bLight() Failed In WndProc()...\n");
				fclose(gpFile);
			}
			else
			{
				bLight = FALSE;
			}
			break;

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

	// VERTEX SHADER
	const char *vertexShaderSourceCode =
		"cbuffer ConstantBuffer"
		"{"
		"float4x4 worldMatrix;"
		"float4x4 viewMatrix;"
		"float4x4 projectionMatrix;"
		"float4 la;"
		"float4 ld;"
		"float4 ls;"
		"float4 lightPosition;"
		"float4 ka;"
		"float4 kd;"
		"float4 ks;"
		"float materialShininess;"
		"uint lightEnabled;"
		"}"
		"struct vertex"
		"{"
		"float4 position:SV_POSITION;"
		"float3 transformedNormals:NORMAL0;"
		"float3 lightDirection:NORMAL1;"
		"float3 viewerVector:NORMAL2;"
		"};"
		"vertex main(float4 position:POSITION, float4 normal:NORMAL)"
		"{"
		"vertex output;"
		"if(lightEnabled == 1)"
		"{"
		"float4 eyeCoordinate = mul(worldMatrix,position);"
		"eyeCoordinate = mul(viewMatrix,eyeCoordinate );"
		"float3x3 normalMatrix = (float3x3)worldMatrix;"
		"output.transformedNormals = mul(normalMatrix, (float3)normal);"
		"output.lightDirection = (float3)lightPosition - (float3)eyeCoordinate;"
		//"float3 ambiant = la * ks;"
		//"float3 diffuse = ld * kd * max(dot(lightDirection,transformedNormals), 0.0);"
		//"float3 reflectionVector = reflect(-lightDirection,transformedNormals);"
		"output.viewerVector = -eyeCoordinate.xyz;"
		//"float3 specular = ls * ks * pow(max(dot(reflectionVector,viewerVector), 0.0),materialShininess);"
		//"output.phong_ads_light = ambiant + diffuse + specular;"
		"}"
		"float4 pos = mul(worldMatrix, position);"
		"pos = mul(viewMatrix, pos);"
		"pos = mul(projectionMatrix, pos);"
		"output.position = pos;"
		"return output;"
		"}";

	ID3DBlob *pID3DBlob_VertexShaderCode = NULL;
	ID3DBlob *pID3DBlob_error = NULL;

	// COMPILE VERTEX SHADER
	hr = D3DCompile(vertexShaderSourceCode,
					lstrlenA(vertexShaderSourceCode) + 1,
					"VS",
					NULL,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					"main",
					"vs_5_0", // FEATURE LEVEL
					0,
					0,
					&pID3DBlob_VertexShaderCode,
					&pID3DBlob_error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		if (pID3DBlob_error)
		{
			fprintf(gpFile, "D3DCompile() Failed for Vertex Shader :\n%s.\n", (char *)pID3DBlob_error->GetBufferPointer());
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed for Vertex Shader : Unknown.\n");
		}
		fclose(gpFile);
		return (hr);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "D3DCompile() SUCCEEDED for Vertex Shader.\n");
		fclose(gpFile);
	}

	// CREATE VERTEX SHADER
	hr = gpID3D11Device->CreateVertexShader(
		pID3DBlob_VertexShaderCode->GetBufferPointer(),
		pID3DBlob_VertexShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11VertexShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::createVertexShader FAILED in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::createVertexShader SUCCEEDED in initialize().\n");
		fclose(gpFile);
	}

	// SET THIS VERTEX SHADER IN VERTEX SHADER STAGE OF PIPE LINE
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	// PIXEL SHADER
	const char *pixelShaderSourceCode =
		"cbuffer ConstantBuffer"
		"{"
		"float4x4 worldMatrix;"
		"float4x4 viewMatrix;"
		"float4x4 projectionMatrix;"
		"float4 la;"
		"float4 ld;"
		"float4 ls;"
		"float4 lightPosition;"
		"float4 ka;"
		"float4 kd;"
		"float4 ks;"
		"float materialShininess;"
		"uint lightEnabled;"
		"}"
		"struct vertex"
		"{"
		"float4 position:SV_POSITION;"
		"float3 transformedNormals:NORMAL0;"
		"float3 lightDirection:NORMAL1;"
		"float3 viewerVector:NORMAL2;"
		"};"
		"float4 main(vertex input):SV_TARGET"
		"{"
		"float3 phong_ads_light;"

		"if(lightEnabled == 1)"
		"{"
		"float3 normalized_transformed_normals= normalize(input.transformedNormals);"
		"float3 normalized_light_direction = normalize(input.lightDirection);"
		"float3 normalized_view_vector = normalize(input.viewerVector);"

		"float3 ambiant = la * ks;"
		"float3 diffuse = ld * kd * max(dot(normalized_light_direction,normalized_transformed_normals), 0.0);"
		"float3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);"
		"float3 specular = ls * ks * pow(max(dot(reflectionVector,normalized_view_vector), 0.0),materialShininess);"
		"phong_ads_light = ambiant + diffuse + specular;"
		"}"
		"else"
		"{"
		"phong_ads_light = float3(1.0,1.0,1.0);"
		"}"
		"return float4(phong_ads_light, 1.0);;"
		"}";

	ID3DBlob *pID3DBlob_PixelShaderCode = NULL;
	pID3DBlob_error = NULL;

	// COMPILE VERTEX SHADER
	hr = D3DCompile(pixelShaderSourceCode,
					lstrlenA(pixelShaderSourceCode) + 1,
					"PS",
					NULL,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					"main",
					"ps_5_0", // FEATURE LEVEL
					0,
					0,
					&pID3DBlob_PixelShaderCode,
					&pID3DBlob_error);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		if (pID3DBlob_error)
		{
			fprintf(gpFile, "D3DCompile() Failed for Pixel Shader :\n%s.\n", (char *)pID3DBlob_error->GetBufferPointer());
		}
		else
		{
			fprintf(gpFile, "D3DCompile() Failed for Pixel Shader : Unknown.\n");
		}
		fclose(gpFile);
		return (hr);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "D3DCompile() SUCCEEDED for Pixel Shader.\n");
		fclose(gpFile);
	}

	// CREATE VERTEX SHADER
	hr = gpID3D11Device->CreatePixelShader(
		pID3DBlob_PixelShaderCode->GetBufferPointer(),
		pID3DBlob_PixelShaderCode->GetBufferSize(),
		NULL,
		&gpID3D11PixelShader);

	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::createPixelShader FAILED in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::createPixelShader SUCCEEDED in initialize().\n");
		fclose(gpFile);
	}

	// SET THIS VERTEX SHADER IN VERTEX SHADER STAGE OF PIPE LINE
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// CREATE INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC d3d11InputElementDesc[2];

	ZeroMemory((void *)d3d11InputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * _ARRAYSIZE(d3d11InputElementDesc));

	d3d11InputElementDesc[0].SemanticName = "POSITION";
	d3d11InputElementDesc[0].SemanticIndex = 0;
	d3d11InputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDesc[0].AlignedByteOffset = 0;
	d3d11InputElementDesc[0].InputSlot = 0;
	d3d11InputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDesc[0].InstanceDataStepRate = 0;

	d3d11InputElementDesc[1].SemanticName = "NORMAL";
	d3d11InputElementDesc[1].SemanticIndex = 0;
	d3d11InputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3d11InputElementDesc[1].AlignedByteOffset = 0;
	d3d11InputElementDesc[1].InputSlot = 1;
	d3d11InputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3d11InputElementDesc[1].InstanceDataStepRate = 0;

	// CREATE INPUT LAYOUT
	// INITIALIZE INPUT LAYOUT STRUCTURE
	hr = gpID3D11Device->CreateInputLayout(d3d11InputElementDesc, _ARRAYSIZE(d3d11InputElementDesc), pID3DBlob_VertexShaderCode->GetBufferPointer(), pID3DBlob_VertexShaderCode->GetBufferSize(), &gpID3D11InputLayout);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::CreateInputLayout FAILED in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ID3D11Device::CreateInputLayout SUCCEEDED in initialize().\n");
		fclose(gpFile);
	}

	// SET THIS INPUT LAYOUT IN INPUT ASSEMBLY STET OF PIPELIE
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	pID3DBlob_VertexShaderCode->Release();
	pID3DBlob_VertexShaderCode = NULL;

	pID3DBlob_PixelShaderCode->Release();
	pID3DBlob_PixelShaderCode = NULL;

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// POSITION
	// CREATE VERTEX BUFFER FOR ABOVE VERTEX POSITIONS
	// A. INITIALIZE  BUFFER DESC
	D3D11_BUFFER_DESC d3d11BufferDescriptor;
	ZeroMemory((void *)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B. INITIALIZE SUB RESOURCE DATA STRUCTURE TO PUT DATA INTO THE BUFFER
	D3D11_SUBRESOURCE_DATA d3d11SubResourceData;
	ZeroMemory((void *)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubResourceData.pSysMem = sphere_vertices;

	// C. CREATE THE BUFFER
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubResourceData, &gpID3D11Buffer_PositionBuffer_Sphere);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer FAILED for Position in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer SUCCEEDED for Position in initialize().\n");
		fclose(gpFile);
	}

	// NORMAL
	// CREATE VERTEX BUFFER FOR ABOVE VERTEX NORMALS
	// A. INITIALIZE  BUFFER DESC
	ZeroMemory((void *)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(float) * _ARRAYSIZE(sphere_normals);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B. INITIALIZE SUB RESOURCE DATA STRUCTURE TO PUT DATA INTO THE BUFFER
	ZeroMemory((void *)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubResourceData.pSysMem = sphere_normals;

	// C. CREATE THE BUFFER
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubResourceData, &gpID3D11Buffer_NormalBuffer_Sphere);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer FAILED for NORMAL in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer SUCCEEDED for NORMAL in initialize().\n");
		fclose(gpFile);
	}

	// INDEX
	// CREATE VERTEX BUFFER FOR ABOVE VERTEX INDEX
	// A. INITIALIZE  BUFFER DESC
	ZeroMemory((void *)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = gNumElements * 3 * sizeof(float);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// B. INITIALIZE SUB RESOURCE DATA STRUCTURE TO PUT DATA INTO THE BUFFER
	ZeroMemory((void *)&d3d11SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubResourceData.pSysMem = sphere_elements;

	// C. CREATE THE BUFFER
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, &d3d11SubResourceData, &gpID3D11Buffer_IndexBuffer_Sphere);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer FAILED for NORMAL in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer SUCCEEDED for NORMAL in initialize().\n");
		fclose(gpFile);
	}

	// CONSTANT BUFFER
	// A. INITIALIZE  BUFFER DESC
	ZeroMemory((void *)&d3d11BufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDescriptor.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// B. CREATE THE BUFFER
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDescriptor, NULL, &gpID3D11Buffer_ConstantBuffer);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer FAILED Constant in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateBuffer SUCCEEDED for Constant in initialize().\n");
		fclose(gpFile);
	}

	// C. SET CONSTANT BUFFER IN VERTEX SHADER STAGE OF PIPELINE
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
	// ENABALING RATSERIZER STATE
	// A. INITAILIZE RASTERIZER INTERFACE
	D3D11_RASTERIZER_DESC d3d11RasterizerDesc;

	ZeroMemory((void *)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID; // Polygon Mode
	d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
	d3d11RasterizerDesc.MultisampleEnable = FALSE;
	d3d11RasterizerDesc.ScissorEnable = FALSE;
	d3d11RasterizerDesc.DepthClipEnable = TRUE;
	d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
	d3d11RasterizerDesc.DepthBias = 0;
	d3d11RasterizerDesc.DepthBiasClamp = 0.0;
	d3d11RasterizerDesc.SlopeScaledDepthBias = FALSE;

	// B. CREATE RASTERIZER STATE ACCORDINGLY
	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpID3D11RasterizerState);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateRasterizerState FAILED  in initialize().\n");
		fclose(gpFile);
	}
	else
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateRasterizerState SUCCEEDED in initialize().\n");
		fclose(gpFile);
	}

	// C. SET THIS STATE INTO RASTERIZE STAGE OF PIPE LINE
	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

	// INITIALIZE CLEAR COLOR ARRAY
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f; // BLUE
	clearColor[3] = 1.0f;

	// INITIALIZE perspectiveProjectionMatrix
	perspectiveProjectionMatrix = XMMatrixIdentity();

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

	currentWindowWidth = width;
	currentWindowHeight = height;

	// RELEASE EXISTING RTV(RENDER TARGET VIEW)
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
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

	// Initialize Texture 2D Desc
	D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
	ZeroMemory((void *)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));

	d3d11Texture2DDesc.Width = (UINT)width;
	d3d11Texture2DDesc.Height = (UINT)height;
	d3d11Texture2DDesc.ArraySize = 1;
	d3d11Texture2DDesc.MipLevels = 1;
	d3d11Texture2DDesc.SampleDesc.Count = 1;
	d3d11Texture2DDesc.SampleDesc.Quality = 0;
	d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2DDesc.CPUAccessFlags = 0;
	d3d11Texture2DDesc.MiscFlags = 0;

	// Declare a 2d texture which is going to converted into depth buffer
	ID3D11Texture2D *pid3d11Texture2D_DepthBuffer = NULL;

	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pid3d11Texture2D_DepthBuffer);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateTexture2D() Failed  ...\n");
		fclose(gpFile);
		return (hr);
	}

	// Initilize dsv
	D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
	ZeroMemory((void *)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = gpID3D11Device->CreateDepthStencilView(pid3d11Texture2D_DepthBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);
	if (FAILED(hr))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "gpID3D11Device::CreateDepthStencilView() Failed  ...\n");
		fclose(gpFile);
		pid3d11Texture2D_DepthBuffer->Release();
		pid3d11Texture2D_DepthBuffer = NULL;
		return (hr);
	}

	// Release the local Depth Buffer
	pid3d11Texture2D_DepthBuffer->Release();
	pid3d11Texture2D_DepthBuffer = NULL;

	// SET THIS NEW RTV IN PIPELINE
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

	// INITIALIZE VIEW PORT STRUCTURE
	D3D11_VIEWPORT d3d11ViewPort;
	d3d11ViewPort.TopLeftX = 0.0f;
	d3d11ViewPort.TopLeftY = 0.0f;
	d3d11ViewPort.Width = (float)width;
	d3d11ViewPort.Height = (float)height;
	d3d11ViewPort.MinDepth = 0.0f;
	d3d11ViewPort.MaxDepth = 1.0f;

	// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
	gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

	// SET PROJECTION MATRIX
	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	return (hr);
}

void display(void)
{
	void draw24Sphere(void);

	// CODE

	// CLEAR THE RTV USING CLEAR COLOR
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);

	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// SET POSITION BUFFER INTO IA STAGE IN PIPELINE
	UINT stride = sizeof(float) * 3;
	UINT offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Sphere, &stride, &offset);

	// SET COLOR BUFFER INTO IA STAGE IN PIPELINE
	stride = sizeof(float) * 3;
	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer_Sphere, &stride, &offset);

	// SET ELEMENT BUFFER INTO IA STAGE IN PIPELINE
	stride = sizeof(float) * 3;
	offset = 0;

	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer_Sphere, DXGI_FORMAT_R16_UINT /* RESP. SHORT */, 0);

	// SET PRIMITIVE TROPOLOGY IN IA STAGE
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	draw24Sphere();

	// SWAP BUFFFERS BY PRESENTING THEM
	gpIDXGISwapChain->Present(0, // NO NEED OF SYNCRONIZATION WITH MONITOR REFRESH RATE
							  0	 // SWAP WITH ALL BUFFERS IN SWAP CHAIN
	);
}

void update(void)
{
	// CODE
	if (keyPressed == 1 || keyPressed == 2 || keyPressed == 3)
	{
		angleForRotation = angleForRotation + 2.0f;
		if (angleForRotation > 360.0f)
			angleForRotation = angleForRotation - 360.0f;

		// Set Light Zero Position - rotating  light
		float angle = XMConvertToRadians(angleForRotation);
		float x = 10.0f * cos(angle);
		float y = 10.0f * sin(angle);

		if (keyPressed == 1)
		{
			lightPositions[1] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 2)
		{
			lightPositions[0] = x;
			lightPositions[2] = y;
		}
		if (keyPressed == 3)
		{
			lightPositions[0] = x;
			lightPositions[1] = y;
		}
	}
}

void uninitialize(void)
{
	// FUNCTION DECLARATIONS
	void ToggleFullScreen(void);

	// CODE
	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}

	if (gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
	}

	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}

	if (gpID3D11Buffer_NormalBuffer_Sphere)
	{
		gpID3D11Buffer_NormalBuffer_Sphere->Release();
		gpID3D11Buffer_NormalBuffer_Sphere = NULL;
	}

	if (gpID3D11Buffer_PositionBuffer_Sphere)
	{
		gpID3D11Buffer_PositionBuffer_Sphere->Release();
		gpID3D11Buffer_PositionBuffer_Sphere = NULL;
	}

	if (gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}

	if (gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}

	if (gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}

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

void draw24Sphere(void)
{

	// Variable Declartions
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX translationMatrix = XMMatrixIdentity();
	CBUFFER constantBuffer;

	int horDistance = currentWindowWidth / 6;
	int verDistance = currentWindowHeight / 7 * 1.1f;

	// TRANSFORMATION
	// A.INITIALIZE MATRICES
	worldMatrix = XMMatrixIdentity();
	viewMatrix = XMMatrixIdentity();
	translationMatrix = XMMatrixIdentity();
	translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);
	worldMatrix = translationMatrix;

	// B. PUT THEM INTO CONSTANT BUFFERS
	ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.projectionMatrix = perspectiveProjectionMatrix;
	constantBuffer.viewMatrix = viewMatrix;
	constantBuffer.wordMatrix = worldMatrix;

	// INITIALIZE VIEW PORT STRUCTURE

	D3D11_VIEWPORT d3d11ViewPort;
	// ***** 1st sphere on 1st column, emerald *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = 0.0f;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// Setting Light Color
		// ambient material
		materialAmbiant[0] = 0.0215; // r
		materialAmbiant[1] = 0.1745; // g
		materialAmbiant[2] = 0.0215; // b
		materialAmbiant[3] = 1.0f;	 // a

		// diffuse material
		materialDiffuse[0] = 0.07568; // r
		materialDiffuse[1] = 0.61424; // g
		materialDiffuse[2] = 0.07568; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.633;	// r
		materialSpecular[1] = 0.727811; // g
		materialSpecular[2] = 0.633;	// b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.6 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 2nd sphere on 1st column, jade *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = verDistance;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// Setting Light Color
		// ambient material
		materialAmbiant[0] = 0.135;	 // r
		materialAmbiant[1] = 0.2225; // g
		materialAmbiant[2] = 0.1575; // b
		materialAmbiant[3] = 1.0f;	 // a

		// diffuse material
		materialDiffuse[0] = 0.54; // r
		materialDiffuse[1] = 0.89; // g
		materialDiffuse[2] = 0.63; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.316228; // r
		materialSpecular[1] = 0.316228; // g
		materialSpecular[2] = 0.316228; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.1 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	//***** 3rd sphere on 1st column, obsidian *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = verDistance * 2;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// Setting Light Color
		// ambient material
		materialAmbiant[0] = 0.05375; // r
		materialAmbiant[1] = 0.05;	  // g
		materialAmbiant[2] = 0.06625; // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.18275; // r
		materialDiffuse[1] = 0.17;	  // g
		materialDiffuse[2] = 0.22525; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.332741; // r
		materialSpecular[1] = 0.328634; // g
		materialSpecular[2] = 0.346435; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.3 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 4th sphere on 1st column, pearl *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = verDistance * 3;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// Setting Light Color
		// ambient material
		materialAmbiant[0] = 0.25;	  // r
		materialAmbiant[1] = 0.20725; // g
		materialAmbiant[2] = 0.20725; // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 1.0;	// r
		materialDiffuse[1] = 0.829; // g
		materialDiffuse[2] = 0.829; // b
		materialDiffuse[3] = 1.0f;	// a

		// specular material
		materialSpecular[0] = 0.296648; // r
		materialSpecular[1] = 0.296648; // g
		materialSpecular[2] = 0.296648; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.088 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 5th sphere on 1st column, ruby *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = verDistance * 4;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// Setting Light Color
		materialAmbiant[0] = 0.1745;  // r
		materialAmbiant[1] = 0.01175; // g
		materialAmbiant[2] = 0.01175; // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.61424; // r
		materialDiffuse[1] = 0.04136; // g
		materialDiffuse[2] = 0.04136; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.727811; // r
		materialSpecular[1] = 0.626959; // g
		materialSpecular[2] = 0.626959; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.6 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 6th sphere on 1st column, turquoise *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = 0.0f;
		d3d11ViewPort.TopLeftY = verDistance * 5;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.1;	  // r
		materialAmbiant[1] = 0.18725; // g
		materialAmbiant[2] = 0.1745;  // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.396;	  // r
		materialDiffuse[1] = 0.74151; // g
		materialDiffuse[2] = 0.69102; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.297254; // r
		materialSpecular[1] = 0.30829;	// g
		materialSpecular[2] = 0.306678; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.1 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 2nd column, brass *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = 0.0f;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		materialAmbiant[0] = 0.329412; // r
		materialAmbiant[1] = 0.223529; // g
		materialAmbiant[2] = 0.027451; // b
		materialAmbiant[3] = 1.0f;	   // a

		// diffuse material
		materialDiffuse[0] = 0.780392; // r
		materialDiffuse[1] = 0.568627; // g
		materialDiffuse[2] = 0.113725; // b
		materialDiffuse[3] = 1.0f;	   // a

		// specular material
		materialSpecular[0] = 0.992157; // r
		materialSpecular[1] = 0.941176; // g
		materialSpecular[2] = 0.807843; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.21794872 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 2nd sphere on 2nd column, bronze *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = verDistance;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.2125; // r
		materialAmbiant[1] = 0.1275; // g
		materialAmbiant[2] = 0.054;	 // b
		materialAmbiant[3] = 1.0f;	 // a

		// diffuse material
		materialDiffuse[0] = 0.714;	  // r
		materialDiffuse[1] = 0.4284;  // g
		materialDiffuse[2] = 0.18144; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.393548; // r
		materialSpecular[1] = 0.271906; // g
		materialSpecular[2] = 0.166721; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.2 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 3rd sphere on 2nd column, chrome *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = verDistance * 2;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.25; // r
		materialAmbiant[1] = 0.25; // g
		materialAmbiant[2] = 0.25; // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4;  // r
		materialDiffuse[1] = 0.4;  // g
		materialDiffuse[2] = 0.4;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.774597; // r
		materialSpecular[1] = 0.774597; // g
		materialSpecular[2] = 0.774597; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.6 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 4th sphere on 2nd column, copper *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = verDistance * 3;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.19125; // r
		materialAmbiant[1] = 0.0735;  // g
		materialAmbiant[2] = 0.0225;  // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.7038;  // r
		materialDiffuse[1] = 0.27048; // g
		materialDiffuse[2] = 0.0828;  // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.256777; // r
		materialSpecular[1] = 0.137622; // g
		materialSpecular[2] = 0.086014; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.1 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 5th sphere on 2nd column, gold *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = verDistance * 4;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.24725; // r
		materialAmbiant[1] = 0.1995;  // g
		materialAmbiant[2] = 0.0745;  // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.75164; // r
		materialDiffuse[1] = 0.60648; // g
		materialDiffuse[2] = 0.22648; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.628281; // r
		materialSpecular[1] = 0.555802; // g
		materialSpecular[2] = 0.366065; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.4 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 6th sphere on 2nd column, silver *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 1.5;
		d3d11ViewPort.TopLeftY = verDistance * 5;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.19225; // r
		materialAmbiant[1] = 0.19225; // g
		materialAmbiant[2] = 0.19225; // b
		materialAmbiant[3] = 1.0f;	  // a

		// diffuse material
		materialDiffuse[0] = 0.50754; // r
		materialDiffuse[1] = 0.50754; // g
		materialDiffuse[2] = 0.50754; // b
		materialDiffuse[3] = 1.0f;	  // a

		// specular material
		materialSpecular[0] = 0.508273; // r
		materialSpecular[1] = 0.508273; // g
		materialSpecular[2] = 0.508273; // b
		materialSpecular[3] = 1.0f;		// a

		// shininess
		materialShinniness = 0.4 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 3rd column, black *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = 0.0f;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.01; // r
		materialDiffuse[1] = 0.01; // g
		materialDiffuse[2] = 0.01; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.50; // r
		materialSpecular[1] = 0.50; // g
		materialSpecular[2] = 0.50; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 2nd sphere on 3rd column, cyan *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = verDistance;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.1;  // g
		materialAmbiant[2] = 0.06; // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.0;		 // r
		materialDiffuse[1] = 0.50980392; // g
		materialDiffuse[2] = 0.50980392; // b
		materialDiffuse[3] = 1.0f;		 // a

		// specular material
		materialSpecular[0] = 0.50185078; // r
		materialSpecular[1] = 0.50185078; // g
		materialSpecular[2] = 0.50185078; // b
		materialSpecular[3] = 1.0f;		  // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 3rd sphere on 2nd column, green *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = verDistance * 2;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.1;  // r
		materialDiffuse[1] = 0.35; // g
		materialDiffuse[2] = 0.1;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.45; // r
		materialSpecular[1] = 0.55; // g
		materialSpecular[2] = 0.45; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 4th sphere on 3rd column, red *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = verDistance * 3;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5;  // r
		materialDiffuse[1] = 0.0;  // g
		materialDiffuse[2] = 0.0;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7;	// r
		materialSpecular[1] = 0.6;	// g
		materialSpecular[2] = 0.6;	// b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 5th sphere on 3rd column, white *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = verDistance * 4;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.55; // r
		materialDiffuse[1] = 0.55; // g
		materialDiffuse[2] = 0.55; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.70; // r
		materialSpecular[1] = 0.70; // g
		materialSpecular[2] = 0.70; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 6th sphere on 3rd column, yellow plastic *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 3;
		d3d11ViewPort.TopLeftY = verDistance * 5;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5;  // r
		materialDiffuse[1] = 0.5;  // g
		materialDiffuse[2] = 0.0;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.60; // r
		materialSpecular[1] = 0.60; // g
		materialSpecular[2] = 0.50; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.25 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// *******************************************************
	// *******************************************************
	// *******************************************************

	// ***** 1st sphere on 4th column, black *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = 0.0f;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		materialAmbiant[0] = 0.02; // r
		materialAmbiant[1] = 0.02; // g
		materialAmbiant[2] = 0.02; // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.01; // r
		materialDiffuse[1] = 0.01; // g
		materialDiffuse[2] = 0.01; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.4;	// r
		materialSpecular[1] = 0.4;	// g
		materialSpecular[2] = 0.4;	// b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 2nd sphere on 4th column, cyan *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = verDistance;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.05; // g
		materialAmbiant[2] = 0.05; // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4;  // r
		materialDiffuse[1] = 0.5;  // g
		materialDiffuse[2] = 0.5;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.04; // r
		materialSpecular[1] = 0.7;	// g
		materialSpecular[2] = 0.7;	// b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// // ***** 3rd sphere on 4th column, green *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = verDistance * 2;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.0;  // r
		materialAmbiant[1] = 0.05; // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4;  // r
		materialDiffuse[1] = 0.5;  // g
		materialDiffuse[2] = 0.4;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.04; // r
		materialSpecular[1] = 0.7;	// g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 4th sphere on 4th column, red *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = verDistance * 3;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.05; // r
		materialAmbiant[1] = 0.0;  // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5;  // r
		materialDiffuse[1] = 0.4;  // g
		materialDiffuse[2] = 0.4;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7;	// r
		materialSpecular[1] = 0.04; // g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 5th sphere on 4th column, white *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = verDistance * 4;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.05; // r
		materialAmbiant[1] = 0.05; // g
		materialAmbiant[2] = 0.05; // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5;  // r
		materialDiffuse[1] = 0.5;  // g
		materialDiffuse[2] = 0.5;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7;	// r
		materialSpecular[1] = 0.7;	// g
		materialSpecular[2] = 0.7;	// b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// ***** 6th sphere on 4th column, yellow rubber *****
	{
		// Setting View Port
		d3d11ViewPort.TopLeftX = horDistance * 4.5;
		d3d11ViewPort.TopLeftY = verDistance * 5;
		d3d11ViewPort.Width = horDistance;
		d3d11ViewPort.Height = verDistance;
		d3d11ViewPort.MinDepth = 0.0f;
		d3d11ViewPort.MaxDepth = 1.0f;

		// TELL DEVICE CONTEXT TO SET THIS VIEWPORT IN PIPELINE
		gpID3D11DeviceContext->RSSetViewports(1, &d3d11ViewPort);

		// ambient material
		materialAmbiant[0] = 0.05; // r
		materialAmbiant[1] = 0.05; // g
		materialAmbiant[2] = 0.0;  // b
		materialAmbiant[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5;  // r
		materialDiffuse[1] = 0.5;  // g
		materialDiffuse[2] = 0.4;  // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7;	// r
		materialSpecular[1] = 0.7;	// g
		materialSpecular[2] = 0.04; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShinniness = 0.078125 * 128;

		// LIGHTING RELATED CODE
		if (bLight == TRUE)
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "(bLight == TRUE...\n");
			fclose(gpFile);

			constantBuffer.la = XMVectorSet(lightAmbiant[0], lightAmbiant[1], lightAmbiant[2], lightAmbiant[3]);
			constantBuffer.ld = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
			constantBuffer.ls = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
			constantBuffer.lightPosition = XMVectorSet(lightPositions[0], lightPositions[1], lightPositions[2], lightPositions[3]);

			constantBuffer.ka = XMVectorSet(materialAmbiant[0], materialAmbiant[1], materialAmbiant[2], materialAmbiant[3]);
			constantBuffer.kd = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], materialDiffuse[3]);
			constantBuffer.ks = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], materialSpecular[3]);
			constantBuffer.materialShininess = materialShinniness;

			constantBuffer.lightEnabled = 1;
		}
		else
		{
			constantBuffer.lightEnabled = 0;
		}

		// C. PUSH THEM INTO THE SHADER
		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// DRAW THE PRIMITIVE
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
}