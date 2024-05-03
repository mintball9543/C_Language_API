#include<Windows.h>
#include<tchar.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	hinst = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "practice4-7";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice4-7", "practice4-7", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, memdc1, memdc2;
	PAINTSTRUCT ps;
	static HBITMAP hbit1, hbit2;
	static int y1, y2, y3, count;
	static char word1[] = "대한민국화이팅";
	static char word2[] = "프로그래밍";
	static char word3[] = "마이크로소프트";
	static RECT rectview;
	static char str[126] = { 0 };
	static RECT rt = { 500,680,650,705 };

	switch (imsg) {
	case WM_CREATE:
		hbit1 = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		y1 = -20;
		y2 = -100;
		y3 = -150;
		SetTimer(hwnd, 1, 500, NULL);
		GetClientRect(hwnd, &rectview);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);
		memdc2 = CreateCompatibleDC(memdc1);
		if (hbit2 == NULL) hbit2 = CreateCompatibleBitmap(hdc, 1024, 768);
		SelectObject(memdc2, hbit1);
		SelectObject(memdc1, hbit2);
		BitBlt(memdc1, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY); //배경
		//떨어지는 글씨
		SetBkMode(memdc1, TRANSPARENT);
		SetTextColor(memdc1, RGB(255, 255, 255));
		TextOut(memdc1, 200, y1, word1, _tcslen(word1));
		TextOut(memdc1, 600, y2, word2, _tcslen(word2));
		TextOut(memdc1, 400, y3, word3, _tcslen(word3));
		BitBlt(hdc, 0, 0, 1024, 768, memdc1, 0, 0, SRCCOPY);
		//글자
		Rectangle(hdc, 500, 675, 650, 700);
		DrawText(hdc, str, _tcslen(str), &rt, DT_TOP | DT_LEFT);
		DeleteDC(memdc1);
		DeleteDC(memdc2);
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			if (strcmp(str, word1) == 0) {
				y1 = -500;
				memset(str, 0, 126);
				count = 0;
				break;
			}
			else if (strcmp(str, word2) == 0) {
				y2 = -500;
				memset(str, 0, 126);
				count = 0;
				break;
			}
			else if (strcmp(str, word3) == 0) {
				y3 = -500;
				memset(str, 0, 126);
				count = 0;
				break;
			}
			else {
				memset(str, 0, 126);
				count = 0;
				break;
			}
		}

		if (wParam == VK_BACK) count--;
		else str[count++] = wParam;
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		y1 += 10;
		y2 += 10;
		y3 += 10;
		if (y1 > rectview.bottom) y1 = -20;
		if (y2 > rectview.bottom) y1 = -100;
		if (y3 > rectview.bottom) y1 = -150;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}