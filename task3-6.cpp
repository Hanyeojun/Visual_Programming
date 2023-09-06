#include <windows.h>
#include <TCHAR.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// 커서 지정

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	static POINT mouse, cat;
	static BOOL Drag; //왼쪽 마우스 버튼 클릭 유무
	static int stepX, stepY, count;
	switch (iMsg){
	case WM_CREATE:
		cat.x = 100;
		cat.y = 100;
		Drag = FALSE;
		count = 0;
		break;

	case WM_LBUTTONUP:
		Drag = FALSE; //마우스 왼쪽 버튼 클릭 땠을 때
		KillTimer(hwnd, 1);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		Drag = TRUE; //마우스 왼쪽 버튼 클릭 시
		mouse.x = LOWORD(lParam); //커서 현재 위치 x
		mouse.y = HIWORD(lParam); //커서 현재 위치 y
		stepX = (mouse.x - cat.x) / 10;
		stepY = (mouse.y - cat.y) / 10;
		InvalidateRgn(hwnd, NULL, TRUE);
		count = 0;
		SetTimer(hwnd, 1, 200, NULL);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Drag) {
			TextOut(hdc, mouse.x + 10, mouse.y + 10, _T("쥐"), _tcslen(_T("쥐")));
		}
		TextOut(hdc, cat.x, cat.y, _T("고양이"), _tcslen(_T("고양이")));
		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		if (!Drag) break;
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		stepX = (mouse.x - cat.x) / 10;
		stepY = (mouse.y - cat.y) / 10;
		count = 0;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
		
	case WM_TIMER:
		count++;
		cat.x += stepX;
		cat.y += stepY;
		InvalidateRgn(hwnd, NULL, TRUE);
		if (count == 10) {
			stepX = 0;
			stepY = 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}