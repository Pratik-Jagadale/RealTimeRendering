#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpszCmdLine,int iCmsShow)
{

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My Application");

	//code
	// wndclass struct initialzation
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register Above wndclass
	RegisterClassEx(&wndclass);

	// cerate window
	hwnd = CreateWindow(szAppName, TEXT("Window Practice Program - 2022-01-22"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT , CW_USEDEFAULT , CW_USEDEFAULT, NULL,NULL, hInstance, NULL);

	//show window
	ShowWindow(hwnd, iCmsShow);
	UpdateWindow(hwnd);

	// Message Loop
	while(GetMessage(&msg, NULL, 0 , 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//code
	switch(iMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return (DefWindowProc(hwnd,iMsg, wParam,lParam));
}
