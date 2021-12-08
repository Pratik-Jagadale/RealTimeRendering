#define UNICODE 

#include <windows.h>
#include <tlHelp32.h> /* for process snapshot related apis and structures */
#include "ExeServerWithRegFile.h"

/* global fucntion declartions */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/* class declarations */
class CSumSubtract : public ISum, ISubstract
{
private:
	long m_cRef;
public:
	/* constructor method declartions */
	CSumSubtract(void);

	/* destructor method declartions */
	~CSumSubtract(void);

	/*IUnknown specific method declarations (inherited) */
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific method declrtions (inherited)
	HRESULT __stdcall SumOfTwointegers(int, int, int*);

	//ISubstract specific mehtod declartions (inherited)
	HRESULT __stdcall SubstractionOfTwoIntegers(int, int, int*);
};

class CSumSubstarctClassFactory: public IClassFactory
{
private:
	long m_cRef;
public:
	// constructor method declarations
	CSumSubstarctClassFactory(void);

	// destructor method declarations
	~CSumSubstarctClassFactory(void);

	// IUnkown specific method declarations (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	/* IClassFactory specific methid declarations (inherited) */
	HRESULT __stdcall CreateInstanace(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};



/* global declartions */
long glNumberOfActiveComponent = 0; //numbeer of active component
long glNumberOfServerLocks = 0; //number of locks on this dll
IClassFactory* gpIClassFactory = NULL;
HWND ghwnd = NULL;
DWORD dwRegister;

/* WinMain */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* fucnction declrations */
	HRESULT StartMyClassFactories(void);
	void StopMyClassFactories(void);
	DWORD GetParentProcessID(void);

	/* variable declartions */
	WNDCLASSEX wndClass;
	MSG msg;
	HWND hwnd;
	HRESULT hr;
	int iDontShowWindow = 0; // 0 means show the window
	TCHAR AppName[] = TEXT("ExeServer");
	TCHAR szTokens[] = TEXT("-/");
	TCHAR* pszTokens;
	TCHAR lpszCmdLine[255];
	wchar_t* next_token = NULL;

	/* COM library initialization */
	GetParentProcessID();

	hr = CoInitialize(NULL);

	if(FAILED(hr));
		return 0;

	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, 255, lpszCmdLine, 255);

	pszTokens = wcstok_s(lpszCmdLine, szTokens, &next_token);

	while (pszTokens != NULL)
	{
		if (_wcsicmp(pszTokens, TEXT("Embedding")) == 0) //i.e. COM is calling me
		{
			iDontShowWindow = 1; // dont show window but message loop must
			break;
		}
		else
		{
			MessageBox(NULL, TEXT("Bad command Line Arguments.\nExitting the application."),TEXT("Error"), MB_OK);
			exit(0);
		}
		pszTokens = wcstok_s(NULL, szTokens, &next_token);
	}
	//window code
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = AppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	/* register window class  */
	RegisterClassEx(&wndClass);

	/* create window */
	hwnd = CreateWindow(AppName, TEXT("Exe Server With RegFile"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	/* initailize global window handle */
	ghwnd = hwnd;
	if (iDontShowWindow != 1)//true if server is not called by COM engine
	{
		ShwoWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);

		//increament server lock
		++glNumberOfServerLocks;
	}

	if (iDontShowWindow == 1)//only when COM calls this program
	{
		//start class factory
		hr = StartMyClassFactories();

		if (FAILED(hr))
		{
			DestroyWindow(hwnd);
			exit(0);
		}
	}

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMeesage(&msg);
	}

	if (iDontShowWindow == 1)//only when COM calls this program
	{
		//stop class factory
		StopMyClassFactories();
	}

	//com library un-initialization
	CoUnitialize();

	return ((int)msg - wParam);
}

/* Window Procedure */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//variable declartion
	HDC hdc;
	RECT rc;
	PAINTSTRUCTps;

	switch (iMsg)
	{
	case WM_PAINT:
		GetCleintRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(0, 255, 0));
		DrawText(hdc, TEXT("This is COM Exe Server Program. Not foe You!!!"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		if (glNumberOfActiveComponent == 0 glNumberOfActiveServerLocks == 0)
			PostQuitMessage(0);
		break;
	case WM_CLOSE:
		--glNumberOfActiveComponent;
		ShowWindow(hwnd, SW_HIDE);
		//fail through , hence no break
	deault:
		return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	}
	return (0);///////////////////////////////not confirm return value see in notes
}

/*Implementation of constructor methods */
CSumSubtract::CSumSubtract(void)
{
	m_cRef = 1;//hardcoded initialization to anticipate possible failure of QueryInterface() 
	InterlockedIncrement(&glNumberOfActiveComponent);//increament global cpunter
}

/*Implementation of destructor method*/
CSumSubtract::~CSumSubtract(void)
{
	InterLockedDecrement(&glNumberOfActiveComponent);//decrement global counter
}

/*Implementation of IUnknown specific method */
HRESULT __stdcall CSumSubtract::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISum)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISubtract)
		*ppv = static_cast<ISubstract>(this);
	else {
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG __stdcall CSumSubtract::AddRef(void)
{
	InterLockedIncrement(m_cRef);
	return(m_cRef);
}

ULONG __stdcall CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete(this); //delete before psoting WM_Quite message

		if (glNumberOfActiveComponent == 0 && glNumberOfServerLocks == 0)
			PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L); //type casting 0 and 0L to WPARAM and LPARAM

		return (0);
	}
}

// ISum specific method definition (inherited)
HRESULT __stdcall CSumSubtract::SumOfTwointegers(int num1, int num2, int* pSum)
{
	*pSum = num1 + num2; 
	return (S_OK);
}

//ISubstract specific mehtod declartions (inherited)
HRESULT __stdcall CSumSubtract::SubstractionOfTwoIntegers(int num1, int num2, int* pSubstract)
{
	*pSubstract = num1 - num2;
	return (S_OK);
}

// constructor method declarations
CSumSubstarctClassFactory::CSumSubstarctClassFactory(void)
{
	m_cRef = 1; //hardcoded initialization to anticipate possible failure of QueryInterface()

}

// destructor method declarations
CSumSubstarctClassFactory::~CSumSubstarctClassFactory(void)
{
	//no code
}

// IUnkown specific method declarations (inherited)
HRESULT __stdcall CSumSubstarctClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else {
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cats<IUnknown*>(*ppv)->AddRef();

	return (S_OK);
}

ULONG __stdcall CSumSubstarctClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);

	return (m_cRef);
}

ULONG __stdcall CSumSubstarctClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);

	if (m_cRef == 0)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

/*Implementation of IClassFactory specific method */
HRESULT __stdcall CSumSubstarctClassFactory::CreateInstanace(IUnknown* pUnkOuter, REFIID riid, void**ppv)
{
	//variable declartions
	CSumSubtract* pCSumSubstarct = NULL;
	HRESULT hr;

	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	//create the instance of component i.e of CSumSubstact class
	pCSumSubstarct = new CSumSubtract();
	if (pCSumSubstarct == NULL)
		return (E_OUTOFMEMORY);

	//get the requested interface
	hr = pCSumSubstarct->QueryInterface(riid, ppv);

	pCSumSubstarct->Release();//anticipate possible failure of QueryInterface()

	return (hr);
}

HRESULT __stdcall CSumSubstarctClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);

	if (glNumberOfActiveComponent == 0 && glNumberOfServerLocks == 0)
		PostMessage(ghwnd, WM_QUIT, (WPARAM)0, (LPARAM)0L);

	return (S_OK);
}


HRESULT StartMyClassFactories(void)
{
	//variable declarations
	HRESULT hr;

	gpIClassFactory = new CSumSubstarctClassFactory;
	if (gpIClassFactory == NULL)
		return(E_OUTOFMEMORY);

	gpIClassFactory->AddRef();

	// register the class factory in COM's private databse
	hr = CoRegisterClassObject(CLSID_SumSubtract, static_cast<IUnknown*>(gpIClassFactory), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSEm & dwRegister);

	/* 
	CoRegisterClassObject registers the CLSID for the sserver in what is called the class
	table (a different table than the running object table). when a server is registered in the
	necessary to launch the serveer associated the given CLSID
	*/
	if (FAILED(hr))
	{
		gpIClassFactory->Release();
		return(E_FAIL);
	}
	return (S_OK);
}

void StopMyClassFactories(void)
{
	//un-register the class factory
	if (dwRegister != 0)
		CoRevokeClassObject(dwRegister);
	/*
		CoRevokeClassObject revokes the class object (removes its registartion) when all of 
		all of the following are treue
		-There are no existing instance of the object defination
		-There are no locks on the class object
		-The apploaction providing services to the class object is not under user control
		 (not visible to the  user on the display)
	*/

	if (gpIClassFactory != NULL)
		gpIClassFactory->Release();
}

DWORD GetParentProcessID(void)
{
	// variable declarations
	HANDLE hProcessSnapShot = NULL;
	BOOL bRetCode = FALSE;
	PROCESSENTRY32 processEntry = { 0 };
	DWORD dwPPID;
	TCHAR szNameOfThisProcess[_MAX_PATH], szNameOFParentProcess(_MAX_PATH);
	TCHAR szTemp[_MAX_PATH],/*szTemp2[_MAX_PATH],*/str[_MAX_PATH], * ptr = NULL;

	//first atke current system snapshot
	hProcessSnapShot = CreateToolhelp32Snapshow(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapShot == INVALID_HANDLE_VALUE)
		return (-1);

	processEntry.dwSize = sizeof(PROCESSENTRY32);

	// walk process hierarchy 
	if (Process32First(hProcessSnapShot, &processEntry))
	{
		GetModuleFileName(NULLL, szTemp, _MAX_PATH);
		ptr = wcsrchr(szNameOfThisProcess, ptr + 1);
		do
		{
			errno_t err;

			err = _wcslwr_s(szNameOfThisProcess, wcslen(szNameOfThisProcess) + 1);
			err = _wcsupr_s(processEntry.szExeFile, wcslen(processEntry.szExeFile) + 1);

			if (wcsstr(szNameOfThisProcess, processEntry.szExeFile ) != NULL)
			{
				wsprintf(str, TEXT("Current Process Name = %s \nCurrent Process ID = % ld \n Parent Process ID = % ld\n Parent Name = % s"),szNameOfThisProcess, processEntry.th32ProcessID, processEntry.th32ParentProcessID, processEntry.szExeFile);

				MessageBox(NULL, str, TEXT("Parent Info"), MB_OK | MB_TOPMOST);

				dwPPID = processEntry.th32ParentProcessID;
			}
		}whiel(Process32Next(hProcessSnapShot, &processEntry));
	}
	CloseHandle(hProcessSnapShot);
	return(dwPPID);
}
