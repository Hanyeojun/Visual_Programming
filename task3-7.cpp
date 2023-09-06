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

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// Ŀ�� ����

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

double LengthPts(int x1, int y1, int x2, int y2) // �� �� �Ÿ� ���
{
	return(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
BOOL InCircle(int x, int y, int mx, int my) // �� �߽���ǥ(x, y), ���콺 ��ǥ(mx, my)
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
		startX = oldX = 40; //���� ��ǥ
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
	case WM_LBUTTONDOWN: // ���� ��ư ������
		Drag = TRUE;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) // ���� �߽���, ���콺 ��ǥ �̿� �� ������ �ľ�
			Selection = TRUE; // �� �ȿ� ������ ������
		oldX = startX;
		oldY = startY;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP: // ���� ��ư ����
		Drag = FALSE;
		Selection = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag && Selection) {
			SetROP2(hdc, R2_XORPEN); //���� ���� XOR
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); //�� ��

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL); //�� ó������ ������
			LineTo(hdc, oldX, oldY); //����� : ���� ����(0) XOR �� ��(1) = �� ��(1)

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);//�׸��� : �� ����(1) XOR �� ��(1) = ���� ��(0)

			oldX = endX; //�� ��ġ�� ���� �������� ����
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