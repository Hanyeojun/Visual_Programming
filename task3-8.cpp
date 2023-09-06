#include <windows.h>
#include <TCHAR.h>
#include<math.h>
#define BSIZE 40
#define DIS 10.0

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
	static double startX, startY, oldX, oldY, stepX, stepY, count;
	static BOOL Selection, Drag, DLock;
	int mx, my, endX, endY;
	switch (iMsg) {
	case WM_CREATE:
		startX = 40; //���� ��ǥ
		startY = 40;
		Selection = FALSE;
		count = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Selection) { //�� ���� ��ȯ
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		}
		Ellipse(hdc, startX - BSIZE, startY - BSIZE, startX + BSIZE, startY + BSIZE);
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN: // ���� ��ư ������
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(startX, startY, mx, my)) { // ���� �߽���, ���콺 ��ǥ �̿� �� ������ �ľ�
			Selection = TRUE; // �� �ȿ� ������ ������
			Drag = TRUE;
			DLock = TRUE; //ó�� ���� ������ ��
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP: // ���� ��ư ����
		Drag = FALSE;
		Selection = FALSE;

		stepX = (oldX - startX) / DIS; // �� �̵��� ���� x ��ǥ �Ÿ� ���
		stepY = (oldY - startY) / DIS; //�� �̵��� ���� y ��ǥ �Ÿ� ���

		SetTimer(hwnd, 1, 50, NULL); //Ÿ�̸� ����
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag && Selection) {
			SetROP2(hdc, R2_XORPEN); //���� ���� XOR
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); //�� ��

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			if (!DLock) { //ó�� ������ �׸��� ��� old�� ����� �ڵ� ����
				MoveToEx(hdc, startX, startY, NULL); //�� ó������ ������
				LineTo(hdc, oldX, oldY); //����� : ���� ����(0) XOR �� ��(1) = �� ��(1)
			}
			else DLock = 0;
			//ó�� �׸� �� old�� ���õǰ� �� ��°���� 
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);//�׸��� : �� ����(1) XOR �� ��(1) = ���� ��(0)

			oldX = endX; //�� ��ġ�� ���� �������� ����
			oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_TIMER:
		if (count == DIS) { //count��ŭ �����̸� Ÿ�̸� ����
			KillTimer(hwnd, 1);
			count = 0;
		}
		else {
			startX += stepX;
			startY += stepY;
			count++;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}