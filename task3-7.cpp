#include <windows.h>
#include <TCHAR.h>
#include<math.h>
#define BSIZE 40

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

double LengthPts(int x1, int y1, int x2, int y2) // 두 점 거리 계산
{
	return(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
BOOL InCircle(int x, int y, int mx, int my) // 원 중심좌표(x, y), 마우스 좌표(mx, my)
{
	if (LengthPts(x, y, mx, my) < BSIZE)
		return TRUE;
	else
		return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush = NULL;
	HPEN hPen, oldPen = NULL;
	static int x, y, startX, startY, oldX, oldY;
	static BOOL Selection, Drag;
	int mx, my, endX, endY;
	switch (iMsg) {
	case WM_CREATE:
		x = 40;
		y = 40;
		startX = oldX = 40; //시작 좌표
		startY = oldY = 40;
		Selection = FALSE;
		Drag = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Selection) {
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		}
		Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
		/*if (Selection) {
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
		}*/
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN: // 왼쪽 버튼 누르면
		Drag = TRUE;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) // 원의 중심점, 마우스 좌표 이용 원 안인지 파악
			Selection = TRUE; // 원 안에 있으면 ‘참’
		oldX = startX;
		oldY = startY;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP: // 왼쪽 버튼 떼면
		Drag = FALSE;
		Selection = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag && Selection) {
			SetROP2(hdc, R2_XORPEN); //펜의 연산 XOR
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); //흰 펜

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL); //맨 처음에는 동일점
			LineTo(hdc, oldX, oldY); //지우기 : 검은 바탕(0) XOR 흰 펜(1) = 흰 선(1)

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);//그리기 : 흰 바탕(1) XOR 흰 펜(1) = 검은 선(0)

			oldX = endX; //현 위치를 이전 지점으로 저장
			oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}