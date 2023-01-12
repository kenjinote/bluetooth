#define UNICODE

#pragma comment(lib,"Bthprops")

#include<Windows.h>
#include<bluetoothapis.h>

TCHAR szClassName[]=TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hList;
	switch(msg)
	{
	case WM_CREATE:
		hList=CreateWindow(TEXT("LISTBOX"),0,WS_VISIBLE|WS_CHILD,0,0,256,512,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		SetTimer(hWnd,0x1234,0,0);
		break;
	case WM_TIMER:
		KillTimer(hWnd,0x1234);
		SendMessage(hList,LB_RESETCONTENT,0,0);
		{
			BLUETOOTH_DEVICE_SEARCH_PARAMS params={0};
			params.fReturnAuthenticated=TRUE;
			params.fReturnRemembered=TRUE;
			params.fReturnUnknown=TRUE;
			params.fReturnConnected=TRUE;
			params.fIssueInquiry=TRUE;
			params.cTimeoutMultiplier=5;
			params.dwSize=sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
			BLUETOOTH_DEVICE_INFO info={0};
			info.dwSize=sizeof(BLUETOOTH_DEVICE_INFO);
			HBLUETOOTH_DEVICE_FIND find=BluetoothFindFirstDevice(&params,&info);
			if(find)
			{
				do
				{
					SendMessage(hList,LB_ADDSTRING,0,(LPARAM)info.szName);
				}
				while(BluetoothFindNextDevice(find,&info));
				BluetoothFindDeviceClose(find);
			}
		}
		SetTimer(hWnd,0x1234,10000,0);
		break;
	case WM_DESTROY:
		KillTimer(hWnd,0x1234);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance=GetModuleHandle(0);
	WNDCLASS wndclass={
		CS_HREDRAW|CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd=CreateWindow(
			szClassName,
			TEXT("Window"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			0,
			CW_USEDEFAULT,
			0,
			0,
			0,
			hInstance,
			0
		);
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(msg.wParam);
}

#if _DEBUG
void main(){}
#endif
