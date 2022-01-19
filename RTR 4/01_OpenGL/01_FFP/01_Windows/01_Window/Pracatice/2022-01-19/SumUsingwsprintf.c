#include <windows.h>

//Global function delcations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // variable declartions
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("My App");

    // code
    // initialization of WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register above class
    RegisterClassEx(&wndclass);

    //create window
    hwnd = CreateWindow(szAppName, TEXT("Window Revision Program -2022-01-20."), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    // message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static int iNum1 = 55, iNum2 = 45, iNum3 = 0;
    TCHAR str[255];

    // code
    switch (iMsg)
    {
    case WM_CREATE:
        iNum3 = iNum1 + iNum2;
        wsprintf(str, TEXT("The Sum of %d and %d is %d"), iNum1, iNum2, iNum3);
        MessageBox(hwnd, str, TEXT("Sum"), MB_OK);
        break;

    case WM_LBUTTONDOWN:
        MessageBox(hwnd, TEXT("WM_LBUTTONDOWN Is recieved"), TEXT("Message"), MB_OK);
        break;

    case WM_RBUTTONDOWN:
        MessageBox(hwnd, TEXT("WM_RBUTTONDOWN Is recieved"), TEXT("Message"), MB_OK);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
