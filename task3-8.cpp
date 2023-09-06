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
	static double startX, startY, oldX, oldY, stepX, stepY, count;
	static BOOL Selection, Drag, DLock;
	int mx, my, endX, endY;
	switch (iMsg) {
	case WM_CREATE:
		startX = 40; //시작 좌표
		startY = 40;
		Selection = FALSE;
		count = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Selection) { //원 색깔 변환
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		}
		Ellipse(hdc, startX - BSIZE, startY - BSIZE, startX + BSIZE, startY + BSIZE);
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN: // 왼쪽 버튼 누르면
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(startX, startY, mx, my)) { // 원의 중심점, 마우스 좌표 이용 원 안인지 파악
			Selection = TRUE; // 원 안에 있으면 ‘참’
			Drag = TRUE;
			DLock = TRUE; //처음 원이 눌렸을 때
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP: // 왼쪽 버튼 떼면
		Drag = FALSE;
		Selection = FALSE;

		stepX = (oldX - startX) / DIS; // 원 이동을 위한 x 좌표 거리 계산
		stepY = (oldY - startY) / DIS; //원 이동을 위한 y 좌표 거리 계산

		SetTimer(hwnd, 1, 50, NULL); //타이머 설정
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag && Selection) {
			SetROP2(hdc, R2_XORPEN); //펜의 연산 XOR
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN)); //흰 펜

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			if (!DLock) { //처음 직선을 그리는 경우 old로 지우는 코드 무시
				MoveToEx(hdc, startX, startY, NULL); //맨 처음에는 동일점
				LineTo(hdc, oldX, oldY); //지우기 : 검은 바탕(0) XOR 흰 펜(1) = 흰 선(1)
			}
			else DLock = 0;
			//처음 그릴 때 old는 무시되고 두 번째부터 
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);//그리기 : 흰 바탕(1) XOR 흰 펜(1) = 검은 선(0)

			oldX = endX; //현 위치를 이전 지점으로 저장
			oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_TIMER:
		if (count == DIS) { //count만큼 움직이면 타이머 종료
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