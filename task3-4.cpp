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
	HPEN hPen, oldPen;
	static int x1, y1, x2, y2, r, rect_r, rect_b, cnt, flag = 3, sflag;
	static RECT rectView;
	switch (iMsg) {
	case WM_CREATE:
		x1 = 20;
		y1 = 20;
		x2 = 60;
		y2 = 20;
		r = 20;
		cnt = 0;
		SetTimer(hwnd, 1, 70, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rectView); //창 크기
		rect_r = rectView.right - rectView.right % 40;
		rect_b = rectView.bottom - rectView.bottom % 40;

		Rectangle(hdc, 0, 0, rect_r, rect_b);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, x1 - r, y1 - r, x1 + r, y1 + r);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, x2 - r, y2 - r, x2 + r, y2 + r);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			flag = 1;
			sflag = 1;
		}
		else if (wParam == VK_UP) {
			flag = 2;
			sflag = 1;
		}
		else if (wParam == VK_RIGHT) {
			flag = 3;
			sflag = 1;
		}
		else if (wParam == VK_DOWN) {
			flag = 4;
			sflag = 1;
		}
		else if (wParam == VK_RETURN) {
			if (sflag == 0) sflag = 1;
			else sflag = 0;
		}
		break;
	case WM_TIMER:
		if (sflag) {
			switch (flag) {
			case 1: //left
				x2 -= r * 2;
				x1 = x2 + r * 2;
				y1 = y2;
				if (x2 - r < rectView.left) {
					x2 = rectView.left + r;
					x1 = x2 + r * 2;
				}
				break;
			case 2: //top
				y2 -= r * 2;
				y1 = y2 + r * 2;
				x1 = x2;
				if (y2 - r < rectView.top) {
					y2 = rectView.top + r;
					y1 = y2 + r * 2;
				}
				break;
			case 3: //right
				x2 += r * 2;
				x1 = x2 - r * 2;
				y1 = y2;
				if (x2 + r > rect_r) {
					x2 = rect_r - r;
					x1 = x2 - r * 2;
				}
				break;
			case 4: //bottom
				y2 += r * 2;
				y1 = y2 - r * 2;
				x1 = x2;
				if (y2 + r > rect_b) {
					y2 = rect_b - r;
					y1 = y2 - r * 2;
				}
				break;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}