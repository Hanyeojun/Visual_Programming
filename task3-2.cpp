#include <windows.h>
#include <TCHAR.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static int x, y, r;
	static RECT rectView;
	static int rect1, rect2;
	switch (iMsg) {
	case WM_CREATE:
		x = 20;
		y = 20;
		r = 20;
		break;
	case WM_PAINT:
		rect1 = rectView.right - rectView.right % 40;
		rect2 = rectView.bottom - rectView.bottom % 40;
		GetClientRect(hwnd, &rectView); //변화되는 창 크기 반환
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, rect1, rect2);
		Ellipse(hdc, x - r, y - r, x + r, y + r);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			x += r * 2;
			if (x + 20 > rect1) x = rect1-r;
		}
		else if (wParam == VK_LEFT) {
			x -= r * 2;
			if (x - 20 < rectView.left) x = rectView.left+r;
		}
		else if (wParam == VK_DOWN) {
			y += r * 2;
			if (y + 20 > rect2) y = rect2 - r;
		}
		else if (wParam == VK_UP) {
			y -= r * 2;
			if (y - 20 < rectView.top) y = rectView.top + r;
		}
		else if (wParam == VK_HOME) {
			x = 20;
			y = 20;
		}
		InvalidateRgn(hwnd, NULL, TRUE);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
//GetClientRect : 클라이언트영억의 사각형 크기 반환