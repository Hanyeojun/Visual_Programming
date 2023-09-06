#include <windows.h>
#include <TCHAR.h>
#define LEFT 0
#define TOP 500
#define RIGHT 300
#define BOTTOM TOP+20
#define LINE 1
#define ELLIPSE 2
#define RECTANGLE 3

int ObjectKind; //LINE,ELLIPSE,RECTANGLE �� �ϳ� ���� -> �� ���� ���� �ش� ���� �׸�
//ex)Ellipase(10, 10, 100, 100) �Է½� ObjectKind=ELLIPSE(=2) ����
int x1, y1, x2, y2;
void Parsing(TCHAR str[]) {
	TCHAR cmd[20], argNum[20]; // cmd = ��Line�� argNum = ��(10, 10, 100, 100)�� ���� �и� ����
	int i = 0, j = 0;
	while (str[i] != '(')  cmd[j++] = str[i++]; // str ���� ��(�� ������ cmd �� ����
	cmd[j] = NULL;
	j = 0;
	while (str[i] != NULL) argNum[j++] = str[i++]; // str ���� ��(�� ���� ������ argNum �� ����
	argNum[j] = NULL;
	// argNum ���� ���ڸ� ã�� x1, y1, x2, y2 �� ����
	_stscanf_s(argNum, _T("(%d, %d, %d, %d)"), &x1, &y1, &x2, &y2); // �Լ� ���� ����
	if (!_tcscmp(cmd, _T("Line"))) ObjectKind = LINE; // ObjectKind = 1;
	else if (!_tcscmp(cmd, _T("Ellipse"))) ObjectKind = ELLIPSE; // ObjectKind = 2;
	else if (!_tcscmp(cmd, _T("Rectangle"))) ObjectKind = RECTANGLE; // ObjectKind = 3;
}

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[100]; //�Է� ���ڿ� ����
	static int cnt; //�Է� ���ڿ� ������ ���� �迭 �ε���
	static SIZE size; //caret ��� ���� ��� ���ڿ� �簢�� ũ�� �����
	switch (iMsg) {
	case WM_CREATE:
		cnt = 0;
		str[cnt] = NULL;
		ObjectKind = 0;
		CreateCaret(hwnd, NULL, 3, 15);
		ShowCaret(hwnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, LEFT, TOP, RIGHT, BOTTOM); //����� �Է� �޴� �簢��
		TextOut(hdc, LEFT + 0, TOP+2, str, _tcslen(str)); //�簢���� �Է� ���� ���
		GetTextExtentPoint(hdc, str, cnt, &size);
		SetCaretPos(LEFT + size.cx, TOP+2); //������� ���� ��ġ�� caret ���

		if (ObjectKind == 1) {
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2, y2);
		}
		else if (ObjectKind == 2) {
			Ellipse(hdc, x1, y1, x2, y2);
		}
		else if (ObjectKind == 3) {
			Rectangle(hdc, x1, y1, x2, y2);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			Parsing(str);
			cnt = 0;
			//str[cnt] = NULL;
		}
		else if (wParam == VK_BACK) {
			if (cnt > 0)cnt--;
		}
		else str[cnt++] = wParam;
		str[cnt] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}