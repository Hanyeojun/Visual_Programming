#include <windows.h>
#include <TCHAR.h>
#define LEFT 0
#define TOP 500
#define RIGHT 300
#define BOTTOM TOP+20
#define LINE 1
#define ELLIPSE 2
#define RECTANGLE 3

int ObjectKind; //LINE,ELLIPSE,RECTANGLE 중 하나 저장 -> 이 값을 보고 해당 도형 그림
//ex)Ellipase(10, 10, 100, 100) 입력시 ObjectKind=ELLIPSE(=2) 저장
int x1, y1, x2, y2;
void Parsing(TCHAR str[]) {
	TCHAR cmd[20], argNum[20]; // cmd = “Line” argNum = “(10, 10, 100, 100)” 으로 분리 저장
	int i = 0, j = 0;
	while (str[i] != '(')  cmd[j++] = str[i++]; // str 에서 “(“ 전까지 cmd 에 복사
	cmd[j] = NULL;
	j = 0;
	while (str[i] != NULL) argNum[j++] = str[i++]; // str 에서 “(“ 부터 끝까지 argNum 에 복사
	argNum[j] = NULL;
	// argNum 에서 숫자를 찾아 x1, y1, x2, y2 에 저장
	_stscanf_s(argNum, _T("(%d, %d, %d, %d)"), &x1, &y1, &x2, &y2); // 함수 각자 조사
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
	static TCHAR str[100]; //입력 문자열 저장
	static int cnt; //입력 문자열 저장을 위한 배열 인덱스
	static SIZE size; //caret 사용 위해 출력 문자열 사각형 크기 저장용
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
		Rectangle(hdc, LEFT, TOP, RIGHT, BOTTOM); //사용자 입력 받는 사각형
		TextOut(hdc, LEFT + 0, TOP+2, str, _tcslen(str)); //사각형에 입력 문자 출력
		GetTextExtentPoint(hdc, str, cnt, &size);
		SetCaretPos(LEFT + size.cx, TOP+2); //문자출력 다음 위치에 caret 출력

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