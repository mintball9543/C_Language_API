#include<Windows.h>
#include<tchar.h>
#include<time.h>
#include<stdlib.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
int CheckCrash(RECT* ptr, RECT* user);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = _T("rubber");
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("rubber"), _T("rubber"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

#define circle_size 32
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT win_size;
	static RECT user_rt;
	static RECT rt[circle_size];
	int temp = 0;
	static int size_var = 20;
	HBRUSH hbrush, oldbrush;
	int i = 0;
	static int flag = 0;
	static int init = 0;
	static int crash[circle_size] = { 0 };

	switch (imsg) {
	case WM_CREATE:
		srand(time(0));
		GetClientRect(hwnd, &win_size);

		for (i = 0; i < circle_size; i++) {
			rt[i].left = rand() % win_size.right;
			rt[i].top = rand() % win_size.bottom;
			temp = (rand() % 100) + size_var;
			rt[i].right = rt[i].left + temp;
			rt[i].bottom = rt[i].top + temp;
		}
		user_rt.left = win_size.right / 2 - 15;
		user_rt.top = win_size.bottom / 2 - 15;
		user_rt.right = user_rt.left + 50;
		user_rt.bottom = user_rt.top + 50;
		init = 1;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hbrush = CreateSolidBrush(RGB(0, 255, 0));
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		for (i = 0; i < circle_size; i++) {
			Ellipse(hdc, rt[i].left, rt[i].top, rt[i].right, rt[i].bottom);
		}
		SelectObject(hdc, oldbrush);
		CloseHandle(hbrush);
		if (flag == 1 || init == 1) {
			hbrush = CreateSolidBrush(RGB(255, 0, 0));
			oldbrush = SelectObject(hdc, hbrush);
			for (i = 0; i < circle_size; i++) {
				if(crash[i]==1)
					Ellipse(hdc, rt[i].left, rt[i].top, rt[i].right, rt[i].bottom);
			}
			SelectObject(hdc, oldbrush);
			CloseHandle(hbrush);
			Ellipse(hdc, user_rt.left, user_rt.top, user_rt.right, user_rt.bottom);
			init = 0;
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (LOWORD(lParam) >= user_rt.left && LOWORD(lParam) <= user_rt.right) {
			if (HIWORD(lParam) >= user_rt.top && HIWORD(lParam) <= user_rt.bottom) {
				flag = 1;
			}
		}
		break;
	case WM_LBUTTONUP:
		flag = 0;
		break;
	case WM_MOUSEMOVE:
		if (flag) {
			user_rt.left = LOWORD(lParam);
			user_rt.top = HIWORD(lParam);
			user_rt.right = user_rt.left + 50;
			user_rt.bottom = user_rt.top + 50;
			temp = CheckCrash(rt, &user_rt);
			if (temp < circle_size)
				crash[temp] = 1;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

int CheckCrash(RECT* ptr, RECT* user) {
	int index = 0;
	int i = 0;
	for (i = 0; i < circle_size; i++) {
		if (user->left >= ptr[i].left && user->left <= ptr[i].right) {
			if (user->top >= ptr[i].top && user->top <= ptr[i].bottom)
				break;
		}
	}
	return i;
}