#include <windows.h>

/* CALLBACK FUNCTION DECLARTION*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM,LPARAM);

/* ENTRY POINT FUNCTION */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//LOCAL VARIABLE DECLARTIONS
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = ("Pratik Rajendra Jagadale : MATRIX GROUP");

	// code
	// Initialization of wndclass structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// RegsterClass wndclass
	RegisterClassEx(&wndclass);

	//create Window
	hwnd = CreateWindow(szAppName, TEXT("Messages"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	// showWindow
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// Message Loop
	while(GetMessage(&msg, NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}	
 
/* CALL BACK FUNCTION */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT Ps;
	static RECT rc;
	TCHAR str[] = TEXT("Wake Up, Neo... \n The Matrix Has You...\n Follow The White Rabbit. \n\n Knock, knock, Neo. !!!");

	switch(iMsg)
	{
		case WM_CREATE:	
			break;		

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &Ps);
			SetBkColor(hdc, RGB(0,0,0));
			SetTextColor(hdc, RGB(0,255,0));
			DrawText(hdc, str, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			EndPaint(hwnd,&Ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			GetClientRect(hwnd, &rc);
			break;
		default:
			break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
