#include <windows.h>
#include <TCHAR.h>
#include<math.h>
#define BSIZE 50

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen = NULL, oldPen = NULL;
	static int x, y, cnt;
	static BOOL Select[4][8];
	RECT rect[4][8] = { {{0,0,100,100},{100,0,200,100},{200,0,300,100},{300,0,400,100},{400,0,500,100},{500,0,600,100},{600,0,700,100},{700,0,800,100}},
	{{0,100,100,200},{100,100,200,200},{200,100,300,200},{300,100,400,200},{400,100,500,200},{500,100,600,200},{600,100,700,200},{700,100,800,200}},
	{{0,200,100,300},{100,200,200,300},{200,200,300,300},{300,200,400,300},{400,200,500,300},{500,200,600,300},{600,200,700,300},{700,200,800,300} },
	{{0,300,100,400},{100,300,200,400},{200,300,300,400},{300,300,400,400},{400,300,500,400},{500,300,600,400},{600,300,700,400},{700,300,800,400}} };
	static int colors[4][8];
	int mx, my;
	switch (iMsg) {
	case WM_CREATE:
		cnt = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				Select[i][j] = FALSE;
				colors[i][j] = 0;
			}
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				Rectangle(hdc, rect[i][j].left, rect[i][j].top, rect[i][j].right, rect[i][j].bottom);
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				if (Select[i][j]) {
					if (colors[i][j]==1) {
						hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						Ellipse(hdc, rect[i][j].left, rect[i][j].top, rect[i][j].right, rect[i][j].bottom);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
					else {
						hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						Ellipse(hdc, rect[i][j].left, rect[i][j].top, rect[i][j].right, rect[i][j].bottom);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
				}
			}
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				if ((mx > rect[i][j].left)&& (my > rect[i][j].top)&& (mx < rect[i][j].right) && (my < rect[i][j].bottom)) {
					Select[i][j] = TRUE;
					colors[i][j] = cnt % 2;
					cnt++;
				}
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