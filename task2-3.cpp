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
	static TCHAR str[10][100]; //static으로 선언되어 이전 내용 그대로 유지 가능
	static int count, yPos;
	switch (iMsg) {
	case WM_CREATE:
		yPos = 0;
		count = 0;
		break;
	case WM_PAINT: //창을 내렸다가 다시 띄웠을 때 다시 출력하여 화면에 표시
		hdc = BeginPaint(hwnd, &ps); //BeginPaint함수를 이용하여 출력 영역을 얻어옴
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 100; x++) {
				if (str[y][x] == NULL) break;
				else TextOut(hdc, 0, y*20, str[y], lstrlen(str[y]));
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK) { //backspace 작동
			if (count != 0) count--;
			else if (count == 0 && yPos != 0) {
				yPos--;
				count = _tcslen(str[yPos]); //둘째줄에서 맨 처음에 backspace하면 줄 감소, count는 첫째줄의 문자열 길이
			}
		}
		else if (wParam == VK_RETURN) { //enter키 
			if (yPos <= 10) {
				count = 0; //
				yPos++; //
			}
		}
		else str[yPos][count++] = wParam; //
		str[yPos][count] = NULL; //
		InvalidateRgn(hwnd, NULL, TRUE); //저장만 하고 출력 x
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}