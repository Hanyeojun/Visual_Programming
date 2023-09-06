#include <windows.h>
#include <TCHAR.h>
#include<stdio.h>
#include<math.h>
#define ITEMNUM 30

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

int itemX[ITEMNUM], itemY[ITEMNUM], radius[ITEMNUM];
int startX, startY, oldX, oldY;
BOOL finish;
float LengthPts(int x1, int y1, int x2, int y2) {
	return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my) {
	if (LengthPts(x, y, mx, my) < 50) return TRUE;
	else return FALSE;
}

BOOL Collision(int x1, int y1, int r1, int x2, int y2, int r2) {
	if (r1 + r2 > LengthPts(x1, y1, x2, y2)) return TRUE;
	else return FALSE;
}

void ItemGenerator(HWND hwnd) {
	int i, j;
	RECT rect;
	GetClientRect(hwnd, &rect);
	for (i = 0; i < ITEMNUM; i++) {
		itemX[i] = rand() % rect.right;
		itemY[i] = rand() % rect.bottom;
		radius[i] = rand() % 90 + 10;
		if (Collision(itemX[i], itemY[i], radius[i], startX, startY, 50) || Collision(itemX[i], itemY[i], radius[i], rect.right, rect.bottom, 50)){
			i -= 1; 
			continue;
		}
		for (j = 0; j < i; j++) {
			if (Collision(itemX[i], itemY[i], radius[i], itemX[j], itemY[j], radius[j])) {
				i -= 1; 
				break;
			}
		}
	}
	return;
}

BOOL CheckCollision(int x, int y, int r) {
	int i;
	BOOL Flag;
	Flag = FALSE;
	for (i = 0; i < ITEMNUM; i++) {
		if (radius[i] > 0 && Collision(itemX[i], itemY[i], radius[i], x, y, r)) {
			radius[i] *= -1;
			Flag = TRUE;
		}
	}
	return Flag;
}

void DrawItem(HDC hdc) {
	int i;
	HBRUSH rBrush, gBrush, oldBrush;
	rBrush = CreateSolidBrush(RGB(255, 0, 0));
	gBrush = CreateSolidBrush(RGB(0, 255, 0));
	for (i = 0; i < ITEMNUM; i++) {
		if (radius[i] > 0) {
			oldBrush = (HBRUSH)SelectObject(hdc, gBrush);
		}
		else oldBrush = (HBRUSH)SelectObject(hdc, rBrush);
		Ellipse(hdc, itemX[i] - radius[i], itemY[i] - radius[i], itemX[i] + radius[i], itemY[i] + radius[i]);
		SelectObject(hdc, oldBrush);
	}
	Ellipse(hdc, startX - 50, startY - 50, startX + 50, startY + 50);
	DeleteObject(rBrush);
	DeleteObject(gBrush);
	return;
}

void Finish(HWND hwnd) {
	HDC hdc;
	int i, count = 0;
	TCHAR message[100];
	RECT rect;
	hdc = GetDC(hwnd);
	for (i = 0; i < ITEMNUM; i++) {
		if (radius[i] < 0) count++;
	}
	_stprintf_s(message, _T("충돌한 공의 개수는 %d입니다."), count);
	GetClientRect(hwnd, &rect);
	DrawText(hdc, message, _tcslen(message), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	ReleaseDC(hwnd, hdc);
	finish = TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static BOOL Drag, Selection, Sync;
	int mx, my;
	RECT rect;
	switch (iMsg) {
	case WM_CREATE:
		startX = oldX = 0;
		startY = oldY = 0;
		Drag = FALSE;
		finish = FALSE;
		ItemGenerator(hwnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawItem(hdc);
		Ellipse(hdc, startX - 50, startY - 50, startX + 50, startY + 50);
		Sync = TRUE;
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(startX, startY, mx, my)) Selection = TRUE;
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		break;
	case WM_MOUSEMOVE:
		if (finish)break;
		hdc = GetDC(hwnd);
		if (Selection) {
			HBRUSH oldBrush;
			HPEN oldPen;
			oldPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if (CheckCollision(mx, my, 50)) InvalidateRgn(hwnd, NULL, TRUE);
			SetROP2(hdc, R2_XORPEN);
			Ellipse(hdc, startX - 50, startY - 50, startX + 50, startY + 50);
			Ellipse(hdc, mx - 50, my - 50, mx + 50, my + 50);
			SetROP2(hdc, R2_COPYPEN);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
			startX = mx;
			startY = my;
		}
		ReleaseDC(hwnd, hdc);
		GetClientRect(hwnd, &rect);
		if (InCircle(startX, startY, rect.right, rect.bottom)) Finish(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}