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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	switch(iMsg)
	{
		case WM_CREATE:
			MessageBox(hwnd,TEXT("Welcome Pratik Jagadale Message !!!"), TEXT("Message") ,MB_OK);
			break;

		case WM_CHAR:
		MessageBox(hwnd,TEXT("This is WM_CHAR BUTTTON Message !!!"), TEXT("WM_CHAR"),MB_OK);
				switch(wParam)
				{
					case 'A':
						MessageBox(hwnd,TEXT("Case 'A' is Pressed !!!"), TEXT("WM_CHAR"),MB_OK);
						break;

					default:
						break;
				}	
				break;	
		
		case WM_KEYDOWN:
			MessageBox(hwnd,TEXT("This is WM_KEYDOWN BUTTTON Message !!!"), TEXT("WM_KEYDOWN"),MB_OK);
			
			switch(wParam)
			{
				 case 0x41://This is hexadecimal value of 65, 65 is ascci of character 'A'. Handles Capital and Small case Characters
					MessageBox(hwnd,TEXT("Case 'A' is Pressed !!!"), TEXT("WM_KEYDOWND"),MB_OK);
				 	
			}
			
			break;

		case WM_LBUTTONDOWN:
			MessageBox(hwnd,TEXT("THIS IS WM_LBUTTONDOWN Message  !!!"), TEXT("Message"), MB_OK);
			break;

		case WM_RBUTTONDOWN:
			MessageBox(hwnd,TEXT("This is WM_RBUTTONDOWN Message !!!"), TEXT("Message"), MB_OK);
			break;

		case WM_DESTROY:
			MessageBox(hwnd,TEXT("This is WM_DESTROY Message !!!"), TEXT("Message"),MB_OK);
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
