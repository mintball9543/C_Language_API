#include<Windows.h>
#include<tchar.h>
#include"resource.h"
#include<math.h>
#define BSIZE 40

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hpreInstance, LPSTR lpszCmdLine, int nCmdShow) {
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
	wc.lpszClassName = _T("color");
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("color"), _T("color"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

double LengthPts(int x1, int y1, int x2, int y2) {
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my) {
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HMENU hmenu, hsubmenu;
	HDC hdc;
	PAINTSTRUCT ps;
	int mx, my;
	static BOOL Select;
	static BOOL Copy;
	static int x, y;

	switch (imsg) {
	case WM_CREATE:
		hmenu = GetMenu(hwnd);
		hsubmenu = GetSubMenu(hmenu, 0);
		EnableMenuItem(hsubmenu, ID_EDITCOPY, MF_GRAYED|MF_DISABLED);
		EnableMenuItem(hsubmenu, ID_EDITPASTE, MF_GRAYED|MF_DISABLED);
		Select = FALSE;
		Copy = FALSE;
		x = 50;
		y = 50;
		break;
	case WM_PAINT:
		EnableMenuItem(hsubmenu, ID_EDITCOPY, Select ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hsubmenu, ID_EDITPASTE, Copy ? MF_ENABLED : MF_GRAYED);
		hdc = BeginPaint(hwnd, &ps);
		if (Select)
			Rectangle(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
		Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_EDITCOPY) {
			Copy = TRUE;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) Select = TRUE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}