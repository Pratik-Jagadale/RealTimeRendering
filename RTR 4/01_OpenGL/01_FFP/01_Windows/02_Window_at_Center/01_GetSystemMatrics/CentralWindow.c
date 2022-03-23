// Header Files
#include <windows.h>

// CALLBACK FUNCTION
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

// GLOBAL VARIABLEDECLARTIONS
int iHeightOfWindow;
int iWidthOfWindow;
int iXMyWindow;
int iYMyWindow;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int iCmdShow )
{
	// variable declartions
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My First Window");
	

	// code
	// initialization of wndclass structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	// Register Above wndclass
	RegisterClassEx(&wndclass);

	iHeightOfWindow = GetSystemMetrics(SM_CYSCREEN ); // Height of Window Screen
	iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN ); // Width Of Window Screen

	iXMyWindow =  (iWidthOfWindow) / 4; // x coordinate for MyWindow
	iYMyWindow =  (iHeightOfWindow) / 4; //y coordinate for MyWindow

	// create window
	hwnd = CreateWindow(szAppName, TEXT("Pratik Jagadale WINDOW : Window At Center Of Screen"),WS_OVERLAPPEDWINDOW, iXMyWindow, iYMyWindow, (iWidthOfWindow) / 2,(iHeightOfWindow) / 2, NULL,NULL,hInstance, NULL);

	// show window 
	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	//message Loop
	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg, WPARAM wParam ,LPARAM lParam)
{
	TCHAR str[255];
	
	switch(iMsg)
	{
		case WM_RBUTTONDOWN:
		 wsprintf(str, TEXT("Height : %d and Width : %d "), iHeightOfWindow, iWidthOfWindow);
			MessageBox(hwnd,str,TEXT("Desktop Window Sizes"),MB_OK);
			break;

		case WM_LBUTTONDOWN:
		 wsprintf(str, TEXT("X : %d and Y : %d "), iXMyWindow, iYMyWindow);
			MessageBox(hwnd,str,TEXT("My Window X & Y Coordinates"),MB_OK);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
