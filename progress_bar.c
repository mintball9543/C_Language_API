#include<Windows.h>
#include<tchar.h>
#include <CommCtrl.h>

#pragma comment(lib,"Comctl32.lib")
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
	wc.lpszClassName = "progress";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("progress", "progress", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	INITCOMMONCONTROLSEX con;
	con.dwICC = ICC_PROGRESS_CLASS;
	con.dwSize = sizeof(INITCOMMONCONTROLSEX);
	static HWND pg;
	static int temp = 0;
	static char pos[10] = { 0 };
	HDC hdc;
	PAINTSTRUCT ps;

	switch (imsg) {
	case WM_CREATE:
		pg = CreateWindow(PROGRESS_CLASS, 0, WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 100, 100, 500, 20, hwnd, 0, hinst, 0);
		SendMessage(pg, PBM_SETPOS, temp++, 0);
		SetTimer(hwnd, 1, 100, 0);
		break;
	case WM_TIMER:
		SendMessage(pg, PBM_SETPOS, temp++, 0);
		itoa(SendMessage(pg, PBM_GETPOS, 0, 0),pos,10);
		hdc = GetDC(hwnd);
		TextOut(hdc, 50, 50, pos, _tcslen(pos));
		ReleaseDC(hwnd, hdc);
		break;
	case WM_NOTIFY:
		return 0;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}