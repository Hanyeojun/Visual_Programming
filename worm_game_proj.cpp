#include <windows.h>
#include <TCHAR.H>
#include <time.h> //srand() 사용하기 위해서 선언

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASS	WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
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



#define ITEMNUM 10 //먹이 개수

int board[22][22]; //게임판 출력을 위한 배열
int wormX[30], wormY[30]; //애벌레 본체 x좌표, 애벌레 본체 y좌표. wormX[0],wormY[0]는 애벌레 머리의 x,y 좌표값
int xDirect, yDirect, len, cnt; //x축 방향, y축 방향, 애벌레 길이, 먹은 먹이 개수


void ItemGenerator() //먹이 출력 함수
{
	int i, x, y; //for문 변수, 먹이 출력 위치를 위한 x, y 좌표값 변수
	for (i = 0; i < ITEMNUM; i++) //먹이 개수만큼 반복
	{
		srand(time(NULL)); //srandtime(NULL)을 사용하여 경과된 시간을 반환 받아서 rand()함수에 랜덤 시드값 전달
		x = rand() % 20 + 1; //1~20 사이의 랜덤 x좌표 설정
		y = rand() % 20 + 1; //1~20 사이의 랜덤 y좌표 설정
		if (board[y][x] == 0) //해당 게임판 인덱스 값이 0이라면
			board[y][x] = 2; //먹이 생성될 좌표 표시를 위해 2 저장
		else //0이 아니라면 => 이미 먹이가 생성된 좌표인 경우
		{
			i = i - 1; //지정된 먹이 개수만큼 좌표를 설정해주기 위해서 다시 반복
			continue; //for문 계속 실행
		}
	}
	return; //함수 종료
}


void DrawGameBoard(HDC hdc) //게임판 출력 함수
{
	int i, x, y; //for문 변수, 게임판 크기 x, y
	for (y = 0; y < 22; y++) { //게임판 y축 크기만큼 반복
		for (x = 0; x < 22; x++) //게임판 x축 크기만큼 반복
		{
			switch (board[y][x]) //게임판의 값에 따라 case문 실행
			{
			case -1: //벽에 해당하는 경우
				Rectangle(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); //벽을 그리기 위해서 사각형 그리기
				break; //switch문 탈출
			case 2: //먹이에 해당하는 경우
				SelectObject(hdc, GetStockObject(BLACK_BRUSH)); //검정 브러시 등록
				Ellipse(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); //해당 위치에 원 그리기
				SelectObject(hdc, GetStockObject(WHITE_BRUSH)); //하얀 브러시 등록
				break; //switch문 탈출
			}
		}
	}

	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); //빨간펜 등록
	Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); //애벌레 머리를 위한 원 그리기
	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); //파란펜 등록 => 애벌레 몸통을 그리기 위함

	for (i = 1; i < len; i++) //애벌레 몸통의 길이만큼 반복
		Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //애벌레 몸통을 위한 원 그리기
}


void GameInit() //초기 게임 셋팅 함수
{
	int i; //for문 위한 변수
	for (i = 0; i < 22; i++) //게임판 벽에 해당하는 가장자리 배열에 -1 저장
	{
		board[i][0] = -1; //왼쪽 벽
		board[i][21] = -1; //오른쪽 벽
		board[0][i] = -1; //위쪽 벽
		board[21][i] = -1; //아래쪽 벽
	}
	wormX[0] = 2; wormY[0] = 1; //시작할 때 애벌레 머리 출력 x, y 좌표
	wormX[1] = 1; wormY[1] = 1; //시작할 때 애벌레 몸통 출력 x, y 좌표
	board[wormY[0]][wormX[0]] = 3; //애벌레가 존재하는 위치에 3 저장. board[2][1]
	board[wormY[1]][wormX[1]] = 3; //board[1][1]

	ItemGenerator(); //먹이 출력 함수 호출

	len = 2; //애벌레 머리 1 + 몸통 1 => 애벌레 길이 = 2
	xDirect = 1; yDirect = 0; //시작할 때 방향 좌표 ->
}


void DirectControl(int DirectKey) //방향 설정 함수 
{
	switch (DirectKey) { //키값에 따라 case문 실행
	case VK_LEFT:  //왼쪽 방향키 입력
		if (xDirect == 1) //x좌표가 오른쪽 이동 상태라면
			break; //아무 실행하지 않도록 switch문 탈출
		if (board[wormY[0]][wormX[0] - 1] != -1) //게임판 인덱스값이 -1이 아니면
		{
			xDirect = -1; //x좌표를 -1로 지정(왼쪽 이동)
			yDirect = 0; //y좌표는 그대로 유지(변화 x)
		}
		break; //switch문 탈출

	case VK_RIGHT: //오른쪽 방향키 입력
		if (xDirect == -1) //x좌표가 왼쪽 이동 상태라면
			break; //아무 실행하지 않도록 switch문 탈출
		if (board[wormY[0]][wormX[0] + 1] != -1) //게임판 인덱스값이 -1이 아니면
		{
			xDirect = 1; //x좌표를 1로 지정(오른쪽 이동)
			yDirect = 0; //y좌표는 그대로 유지(변화 x)
		}
		break; //switch문 탈출

	case VK_UP: //위쪽 방향키 입력
		if (yDirect == 1) //y좌표가 아래쪽 이동 상태라면
			break; //아무 실행하지 않도록 switch문 탈출
		if (board[wormY[0] - 1][wormX[0]] != -1) //게임판 인덱스값이 -1이 아니면
		{
			xDirect = 0; //x좌표는 그대로 유지(변화 x)
			yDirect = -1; //y좌표를 -1로 지정(위쪽 이동)
		}
		break; //switch문 탈출

	case VK_DOWN:
		if (yDirect == -1) //y좌표가 위쪽 이동 상태라면
			break; //아무 실행하지 않도록 switch문 탈출
		if (board[wormY[0] + 1][wormX[0]] != -1) //게임판 인덱스값이 -1이 아니면
		{
			xDirect = 0; //x좌표는 그대로 유지(변화 x)
			yDirect = 1; //y좌표를 1로 지정(아래쪽 이동)
		}
		break; //switch문 탈출
	}
}


void MovingWorm(HWND hwnd) //애벌레 동작 함수
{
	int tmpx, tmpy, i; //애벌레 x, y 좌표, for문 변수

	tmpx = wormX[0]; //이전 애벌레 머리 x 좌표 저장
	tmpy = wormY[0]; //이전 애벌레 머리 y 좌표 저장
	wormX[0] += xDirect; wormY[0] += yDirect; //애벌레 머리의 x좌표에 x 방향값 더함. 애벌레 머리의 y좌표에 y 방향값 더함

	if (board[wormY[0]][wormX[0]] == -1) //애벌레가 벽을 만났을 때
	{
		wormX[0] = tmpx; //애벌레 머리의 x 좌표는 tmpx
		wormY[0] = tmpy; //애벌레 머리의 y 좌표는 tmpy
		KillTimer(hwnd, 1); //타이머 중지
		MessageBox(hwnd, _T("Game Over"), _T(""), MB_OK); //메세지창 띄움
		PostQuitMessage(0); //WM_QUIT 발생 -> 프로그램 종료
	}
	else if (board[wormY[0]][wormX[0]] == 3) { //애벌레가 자신의 몸통을 만났을 때
		wormX[0] = tmpx; //애벌레 머리의 x 좌표는 tmpx
		wormY[0] = tmpy; //애벌레 머리의 y 좌표는 tmpy
	}
	else //-1이나 3이 아닌 그 외의 경우
	{
		if (board[wormY[0]][wormX[0]] == 2) //애벌레 머리 좌표에 해당하는 게임판 값이 2이면 => 먹이 먹었을 경우
		{
			len = len + 1; //애벌레 길이 1 증가
			cnt++; //먹은 먹이 수 1 증가
			board[wormY[0]][wormX[0]] = 0; //아이템을 먹었으므로 먹이가 있던 자리를 0으로 지정
		}
		else //2가 아닌 그 외
			board[wormY[len - 1]][wormX[len - 1]] = 0; //애벌레가 지나가면서 3이 된 인덱스값을 0으로 변경
		//wormY[len-1], wormX[len-1] = 애벌레 꼬리의 y,x 좌표
		for (i = len - 1; i > 1; i--) //애벌레 꼬리부터 몸통까지 
		{
			wormX[i] = wormX[i - 1]; //x 좌표를 옆 칸으로 이동
			wormY[i] = wormY[i - 1]; //y 좌표를 옆 칸으로 이동
		}
		wormX[1] = tmpx; //애벌레 머리 바로 옆 몸통 x 좌표에 tmpx 저장
		wormY[1] = tmpy; //애벌레 머리 바로 옆 몸통 y 좌표에 tmpy 저장
		board[wormY[0]][wormX[0]] = 3; //애벌레 머리에 해당하는 게임판 인덱스 = 3
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //디바이스 콘텍스트를 저장할 변수
	PAINTSTRUCT ps; //출력 영역(디바이스 콘텍스트)에 대한 상세 정보를 저장할 변수
	HBRUSH hBrush, oldBrush; //애벌레 색칠
	static BOOL flag; //애벌레 이동을 시작 판단
	switch (iMsg) {
	case WM_CREATE: //WM_CREATE 발생
		GameInit(); //게임 초기 설정 함수 호출
		flag = FALSE; //flag를 FALSE로 초기화하여 바로 이동하지 않도록 설정
		break; //switch문 탈출

	case WM_PAINT: //WM_PAINT 발생
		hdc = BeginPaint(hwnd, &ps); //BeginPaint()를 통하여 출력할 영역을 얻어서 hdc에 저장
		DrawGameBoard(hdc); //게임판 출력 함수 호출

		TextOut(hdc, 100, 450, _T("Press any key to start the game"),
			_tcslen(_T("Press any key to start the game"))); //스페이스바를 눌러서 게임을 시작하도록 설명 출력
		if (flag) {
			TextOut(hdc, 100, 450, _T("                                                                              "),
				_tcslen(_T("                                                                              "))); //이전의 출력 내용 안 보이도록 공백 출력
		}
		if (cnt == ITEMNUM) { //먹이의 개수와 cnt값이 같으면
			KillTimer(hwnd, 1); //타이머 중지
			TextOut(hdc, 100, 450, _T("Caterpillar ate all its food!"),
				_tcslen(_T("Caterpillar ate all its food!"))); //먹이를 다 먹었다고 화면에 출력

			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); //초록펜 생성
			hBrush = CreateSolidBrush(RGB(0, 255, 0)); //초록색으로 칠해주기 위해 브러시 생성
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); //선택된 펜과 브러시로 애벌레 머리 그리기
			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); //초록펜 생성
			for (int i = 1; i < len; i++) { //몸통 길이만큼 반복
				Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //선택된 펜과 브러시로 애벌레 몸통 그리기
			}
		}
		EndPaint(hwnd, &ps); //출력 마친 후 출력 영역 반환
		break; //switch문 탈출

	case WM_KEYDOWN: //WM_KEYDOWN 발생
		DirectControl((int)wParam); //방향 설정 함수 호출
		if (wParam == VK_SPACE) { //wParam으로 받은 값이 스페이스바라면
			SetTimer(hwnd, 1, 100, NULL); //타이머 시작
			flag = TRUE; //flag 변수 true 
		}
		break; //switch문 탈출

	case WM_TIMER: //WM_TIMER 발생
		MovingWorm(hwnd); //애벌레 동작 함수 호출
		InvalidateRgn(hwnd, NULL, TRUE); //윈도우 화면 전체를 무효화해 화면의 내용을 삭제하고 WM_PAINT 메시지 발생
		break; //switch문 탈출

	case WM_DESTROY: //WM_DESTROY 발생
		KillTimer(hwnd, 1); //타이머 정지
		PostQuitMessage(0); //WM_QUIT 발생 -> 프로그램 종료
		return 0; //switch문 탈출
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam)); //나머지 커널이 처리하도록 메시지 전달
}