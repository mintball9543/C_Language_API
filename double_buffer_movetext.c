#include<Windows.h>
#include<tchar.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hist;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	hist = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "movetext";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("movetext", "mvoetext", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void TextPrint(HDC hdc, int x, int y, TCHAR text[]) {
	int i, j;
	SetTextColor(hdc, RGB(255, 255, 255));
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++)
			TextOut(hdc, x + i, y + j, text, _tcslen(text));
	SetTextColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, x, y, text, _tcslen(text));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, mem1dc,mem2dc;
	PAINTSTRUCT ps;
	static HBITMAP hBit1, oldBit1, hBit2, oldBit2;
	static RECT rectview;
	static int ypos;
	TCHAR word[] = _T("대한민국 화이팅");

	switch (imsg) {
	case WM_CREATE:
		ypos = -30;
		GetClientRect(hwnd, &rectview);
		SetTimer(hwnd, 1, 70, NULL);
		hBit2 = LoadBitmap(hist, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_TIMER:
		ypos += 5;
		if (ypos > rectview.bottom) ypos = -30;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &rectview);
		hdc = BeginPaint(hwnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);
		if (hBit1 == NULL)
			hBit1 = CreateCompatibleBitmap(hdc, 530, 530);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
		BitBlt(mem1dc, 0, 0, 530, 530, mem2dc, 0, 0, SRCCOPY);
		SetBkMode(mem1dc, TRANSPARENT);
		TextPrint(mem1dc, 200, ypos, word);
		BitBlt(hdc, 0, 0, 530, 530, mem1dc, 0, 0, SRCCOPY);
		SelectObject(mem1dc, oldBit1);
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		if (hBit1) DeleteObject(hBit1);
		DeleteObject(hBit2);
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}