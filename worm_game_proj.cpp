#include <windows.h>
#include <TCHAR.H>
#include <time.h> //srand() ����ϱ� ���ؼ� ����

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



#define ITEMNUM 10 //���� ����

int board[22][22]; //������ ����� ���� �迭
int wormX[30], wormY[30]; //�ֹ��� ��ü x��ǥ, �ֹ��� ��ü y��ǥ. wormX[0],wormY[0]�� �ֹ��� �Ӹ��� x,y ��ǥ��
int xDirect, yDirect, len, cnt; //x�� ����, y�� ����, �ֹ��� ����, ���� ���� ����


void ItemGenerator() //���� ��� �Լ�
{
	int i, x, y; //for�� ����, ���� ��� ��ġ�� ���� x, y ��ǥ�� ����
	for (i = 0; i < ITEMNUM; i++) //���� ������ŭ �ݺ�
	{
		srand(time(NULL)); //srandtime(NULL)�� ����Ͽ� ����� �ð��� ��ȯ �޾Ƽ� rand()�Լ��� ���� �õ尪 ����
		x = rand() % 20 + 1; //1~20 ������ ���� x��ǥ ����
		y = rand() % 20 + 1; //1~20 ������ ���� y��ǥ ����
		if (board[y][x] == 0) //�ش� ������ �ε��� ���� 0�̶��
			board[y][x] = 2; //���� ������ ��ǥ ǥ�ø� ���� 2 ����
		else //0�� �ƴ϶�� => �̹� ���̰� ������ ��ǥ�� ���
		{
			i = i - 1; //������ ���� ������ŭ ��ǥ�� �������ֱ� ���ؼ� �ٽ� �ݺ�
			continue; //for�� ��� ����
		}
	}
	return; //�Լ� ����
}


void DrawGameBoard(HDC hdc) //������ ��� �Լ�
{
	int i, x, y; //for�� ����, ������ ũ�� x, y
	for (y = 0; y < 22; y++) { //������ y�� ũ�⸸ŭ �ݺ�
		for (x = 0; x < 22; x++) //������ x�� ũ�⸸ŭ �ݺ�
		{
			switch (board[y][x]) //�������� ���� ���� case�� ����
			{
			case -1: //���� �ش��ϴ� ���
				Rectangle(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); //���� �׸��� ���ؼ� �簢�� �׸���
				break; //switch�� Ż��
			case 2: //���̿� �ش��ϴ� ���
				SelectObject(hdc, GetStockObject(BLACK_BRUSH)); //���� �귯�� ���
				Ellipse(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); //�ش� ��ġ�� �� �׸���
				SelectObject(hdc, GetStockObject(WHITE_BRUSH)); //�Ͼ� �귯�� ���
				break; //switch�� Ż��
			}
		}
	}

	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); //������ ���
	Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); //�ֹ��� �Ӹ��� ���� �� �׸���
	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); //�Ķ��� ��� => �ֹ��� ������ �׸��� ����

	for (i = 1; i < len; i++) //�ֹ��� ������ ���̸�ŭ �ݺ�
		Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //�ֹ��� ������ ���� �� �׸���
}


void GameInit() //�ʱ� ���� ���� �Լ�
{
	int i; //for�� ���� ����
	for (i = 0; i < 22; i++) //������ ���� �ش��ϴ� �����ڸ� �迭�� -1 ����
	{
		board[i][0] = -1; //���� ��
		board[i][21] = -1; //������ ��
		board[0][i] = -1; //���� ��
		board[21][i] = -1; //�Ʒ��� ��
	}
	wormX[0] = 2; wormY[0] = 1; //������ �� �ֹ��� �Ӹ� ��� x, y ��ǥ
	wormX[1] = 1; wormY[1] = 1; //������ �� �ֹ��� ���� ��� x, y ��ǥ
	board[wormY[0]][wormX[0]] = 3; //�ֹ����� �����ϴ� ��ġ�� 3 ����. board[2][1]
	board[wormY[1]][wormX[1]] = 3; //board[1][1]

	ItemGenerator(); //���� ��� �Լ� ȣ��

	len = 2; //�ֹ��� �Ӹ� 1 + ���� 1 => �ֹ��� ���� = 2
	xDirect = 1; yDirect = 0; //������ �� ���� ��ǥ ->
}


void DirectControl(int DirectKey) //���� ���� �Լ� 
{
	switch (DirectKey) { //Ű���� ���� case�� ����
	case VK_LEFT:  //���� ����Ű �Է�
		if (xDirect == 1) //x��ǥ�� ������ �̵� ���¶��
			break; //�ƹ� �������� �ʵ��� switch�� Ż��
		if (board[wormY[0]][wormX[0] - 1] != -1) //������ �ε������� -1�� �ƴϸ�
		{
			xDirect = -1; //x��ǥ�� -1�� ����(���� �̵�)
			yDirect = 0; //y��ǥ�� �״�� ����(��ȭ x)
		}
		break; //switch�� Ż��

	case VK_RIGHT: //������ ����Ű �Է�
		if (xDirect == -1) //x��ǥ�� ���� �̵� ���¶��
			break; //�ƹ� �������� �ʵ��� switch�� Ż��
		if (board[wormY[0]][wormX[0] + 1] != -1) //������ �ε������� -1�� �ƴϸ�
		{
			xDirect = 1; //x��ǥ�� 1�� ����(������ �̵�)
			yDirect = 0; //y��ǥ�� �״�� ����(��ȭ x)
		}
		break; //switch�� Ż��

	case VK_UP: //���� ����Ű �Է�
		if (yDirect == 1) //y��ǥ�� �Ʒ��� �̵� ���¶��
			break; //�ƹ� �������� �ʵ��� switch�� Ż��
		if (board[wormY[0] - 1][wormX[0]] != -1) //������ �ε������� -1�� �ƴϸ�
		{
			xDirect = 0; //x��ǥ�� �״�� ����(��ȭ x)
			yDirect = -1; //y��ǥ�� -1�� ����(���� �̵�)
		}
		break; //switch�� Ż��

	case VK_DOWN:
		if (yDirect == -1) //y��ǥ�� ���� �̵� ���¶��
			break; //�ƹ� �������� �ʵ��� switch�� Ż��
		if (board[wormY[0] + 1][wormX[0]] != -1) //������ �ε������� -1�� �ƴϸ�
		{
			xDirect = 0; //x��ǥ�� �״�� ����(��ȭ x)
			yDirect = 1; //y��ǥ�� 1�� ����(�Ʒ��� �̵�)
		}
		break; //switch�� Ż��
	}
}


void MovingWorm(HWND hwnd) //�ֹ��� ���� �Լ�
{
	int tmpx, tmpy, i; //�ֹ��� x, y ��ǥ, for�� ����

	tmpx = wormX[0]; //���� �ֹ��� �Ӹ� x ��ǥ ����
	tmpy = wormY[0]; //���� �ֹ��� �Ӹ� y ��ǥ ����
	wormX[0] += xDirect; wormY[0] += yDirect; //�ֹ��� �Ӹ��� x��ǥ�� x ���Ⱚ ����. �ֹ��� �Ӹ��� y��ǥ�� y ���Ⱚ ����

	if (board[wormY[0]][wormX[0]] == -1) //�ֹ����� ���� ������ ��
	{
		wormX[0] = tmpx; //�ֹ��� �Ӹ��� x ��ǥ�� tmpx
		wormY[0] = tmpy; //�ֹ��� �Ӹ��� y ��ǥ�� tmpy
		KillTimer(hwnd, 1); //Ÿ�̸� ����
		MessageBox(hwnd, _T("Game Over"), _T(""), MB_OK); //�޼���â ���
		PostQuitMessage(0); //WM_QUIT �߻� -> ���α׷� ����
	}
	else if (board[wormY[0]][wormX[0]] == 3) { //�ֹ����� �ڽ��� ������ ������ ��
		wormX[0] = tmpx; //�ֹ��� �Ӹ��� x ��ǥ�� tmpx
		wormY[0] = tmpy; //�ֹ��� �Ӹ��� y ��ǥ�� tmpy
	}
	else //-1�̳� 3�� �ƴ� �� ���� ���
	{
		if (board[wormY[0]][wormX[0]] == 2) //�ֹ��� �Ӹ� ��ǥ�� �ش��ϴ� ������ ���� 2�̸� => ���� �Ծ��� ���
		{
			len = len + 1; //�ֹ��� ���� 1 ����
			cnt++; //���� ���� �� 1 ����
			board[wormY[0]][wormX[0]] = 0; //�������� �Ծ����Ƿ� ���̰� �ִ� �ڸ��� 0���� ����
		}
		else //2�� �ƴ� �� ��
			board[wormY[len - 1]][wormX[len - 1]] = 0; //�ֹ����� �������鼭 3�� �� �ε������� 0���� ����
		//wormY[len-1], wormX[len-1] = �ֹ��� ������ y,x ��ǥ
		for (i = len - 1; i > 1; i--) //�ֹ��� �������� ������� 
		{
			wormX[i] = wormX[i - 1]; //x ��ǥ�� �� ĭ���� �̵�
			wormY[i] = wormY[i - 1]; //y ��ǥ�� �� ĭ���� �̵�
		}
		wormX[1] = tmpx; //�ֹ��� �Ӹ� �ٷ� �� ���� x ��ǥ�� tmpx ����
		wormY[1] = tmpy; //�ֹ��� �Ӹ� �ٷ� �� ���� y ��ǥ�� tmpy ����
		board[wormY[0]][wormX[0]] = 3; //�ֹ��� �Ӹ��� �ش��ϴ� ������ �ε��� = 3
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //����̽� ���ؽ�Ʈ�� ������ ����
	PAINTSTRUCT ps; //��� ����(����̽� ���ؽ�Ʈ)�� ���� �� ������ ������ ����
	HBRUSH hBrush, oldBrush; //�ֹ��� ��ĥ
	static BOOL flag; //�ֹ��� �̵��� ���� �Ǵ�
	switch (iMsg) {
	case WM_CREATE: //WM_CREATE �߻�
		GameInit(); //���� �ʱ� ���� �Լ� ȣ��
		flag = FALSE; //flag�� FALSE�� �ʱ�ȭ�Ͽ� �ٷ� �̵����� �ʵ��� ����
		break; //switch�� Ż��

	case WM_PAINT: //WM_PAINT �߻�
		hdc = BeginPaint(hwnd, &ps); //BeginPaint()�� ���Ͽ� ����� ������ �� hdc�� ����
		DrawGameBoard(hdc); //������ ��� �Լ� ȣ��

		TextOut(hdc, 100, 450, _T("Press any key to start the game"),
			_tcslen(_T("Press any key to start the game"))); //�����̽��ٸ� ������ ������ �����ϵ��� ���� ���
		if (flag) {
			TextOut(hdc, 100, 450, _T("                                                                              "),
				_tcslen(_T("                                                                              "))); //������ ��� ���� �� ���̵��� ���� ���
		}
		if (cnt == ITEMNUM) { //������ ������ cnt���� ������
			KillTimer(hwnd, 1); //Ÿ�̸� ����
			TextOut(hdc, 100, 450, _T("Caterpillar ate all its food!"),
				_tcslen(_T("Caterpillar ate all its food!"))); //���̸� �� �Ծ��ٰ� ȭ�鿡 ���

			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); //�ʷ��� ����
			hBrush = CreateSolidBrush(RGB(0, 255, 0)); //�ʷϻ����� ĥ���ֱ� ���� �귯�� ����
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); //���õ� ��� �귯�÷� �ֹ��� �Ӹ� �׸���
			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); //�ʷ��� ����
			for (int i = 1; i < len; i++) { //���� ���̸�ŭ �ݺ�
				Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //���õ� ��� �귯�÷� �ֹ��� ���� �׸���
			}
		}
		EndPaint(hwnd, &ps); //��� ��ģ �� ��� ���� ��ȯ
		break; //switch�� Ż��

	case WM_KEYDOWN: //WM_KEYDOWN �߻�
		DirectControl((int)wParam); //���� ���� �Լ� ȣ��
		if (wParam == VK_SPACE) { //wParam���� ���� ���� �����̽��ٶ��
			SetTimer(hwnd, 1, 100, NULL); //Ÿ�̸� ����
			flag = TRUE; //flag ���� true 
		}
		break; //switch�� Ż��

	case WM_TIMER: //WM_TIMER �߻�
		MovingWorm(hwnd); //�ֹ��� ���� �Լ� ȣ��
		InvalidateRgn(hwnd, NULL, TRUE); //������ ȭ�� ��ü�� ��ȿȭ�� ȭ���� ������ �����ϰ� WM_PAINT �޽��� �߻�
		break; //switch�� Ż��

	case WM_DESTROY: //WM_DESTROY �߻�
		KillTimer(hwnd, 1); //Ÿ�̸� ����
		PostQuitMessage(0); //WM_QUIT �߻� -> ���α׷� ����
		return 0; //switch�� Ż��
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam)); //������ Ŀ���� ó���ϵ��� �޽��� ����
}