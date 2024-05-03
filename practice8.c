#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

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
	wc.lpszClassName = _T("practice8");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice8"), _T("practice8"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	SIZE size;
	static TCHAR* str[10];
	static int count,y;
	static char temp[256] = { 0 };
	int i = 0;
	switch (imsg) {
	case WM_CREATE:
		for (i = 0; i < 10; i++) {
			str[i] = (TCHAR*)malloc(sizeof(TCHAR*) * 100);
			memset(str[i], 0, 100);
		}
		CreateCaret(hwnd, NULL, 2, 12);
		ShowCaret(hwnd);
		count = 0;
		y = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 100, 100, 400, 200);
		for (i = 0; i <= y; i++) {
			TextOut(hdc, 100, (i*20 + 100), str[i], _tcslen(str[i]));
			GetTextExtentPoint(hdc, str[i], _tcslen(str[i]), &size);
		}
		SetCaretPos(size.cx+100, (y*20+100));
		if (size.cx + 100 >= 400) {
			y++;
			count = 0;
		}
		
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		GetTextExtentPoint(hdc, str[y], _tcslen(str[y]), &size);
		if (size.cx + 100 >= 390 && y == 4) {
			break;
		}
		if (wParam == VK_BACK && count > 0) count--;
		else if (wParam == VK_RETURN) {
			y++;
			count = 0;
		}
		else str[y][count++] = wParam;
		str[y][count] = NULL;

		ReleaseDC(hwnd, hdc);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DestroyCaret();
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}
