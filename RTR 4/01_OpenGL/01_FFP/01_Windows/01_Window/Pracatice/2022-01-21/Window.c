#include <windows.h>

// CALLBACK FUCNTION DECLARTION
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM ,LPARAM);

// ENTRY POINT FUNTION
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine ,int iCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("WINDOW PRACTICE PROGRAM...");

	//code
	// intializing wndlcass struct variabled
	wndclass.cbSize = sizeof(WNDCLASSEX  ); //NEW After EX
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // NEW 

	// Register Above Class
	RegisterClassEx(&wndclass);


	//create window
	hwnd = CreateWindow(szAppName,TEXT("WINDOW PRACTICE PROGRAM - 2022-01-20"), WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,NULL,hInstance,NULL);

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	// Message Loop
	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (0);
}

// DEFINITION OF WndProc CALLBACK FUCNTION
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam,lParam));
}