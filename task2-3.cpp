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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[10][100]; //static���� ����Ǿ� ���� ���� �״�� ���� ����
	static int count, yPos;
	switch (iMsg) {
	case WM_CREATE:
		yPos = 0;
		count = 0;
		break;
	case WM_PAINT: //â�� ���ȴٰ� �ٽ� ����� �� �ٽ� ����Ͽ� ȭ�鿡 ǥ��
		hdc = BeginPaint(hwnd, &ps); //BeginPaint�Լ��� �̿��Ͽ� ��� ������ ����
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 100; x++) {
				if (str[y][x] == NULL) break;
				else TextOut(hdc, 0, y*20, str[y], lstrlen(str[y]));
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK) { //backspace �۵�
			if (count != 0) count--;
			else if (count == 0 && yPos != 0) {
				yPos--;
				count = _tcslen(str[yPos]); //��°�ٿ��� �� ó���� backspace�ϸ� �� ����, count�� ù°���� ���ڿ� ����
			}
		}
		else if (wParam == VK_RETURN) { //enterŰ 
			if (yPos <= 10) {
				count = 0; //
				yPos++; //
			}
		}
		else str[yPos][count++] = wParam; //
		str[yPos][count] = NULL; //
		InvalidateRgn(hwnd, NULL, TRUE); //���常 �ϰ� ��� x
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}