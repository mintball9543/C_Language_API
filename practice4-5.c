#include<Windows.h>
#include<tchar.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCMdLine, int nCmdShow) {
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
	wc.lpszClassName = _T("practice4-5");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice4-5"), _T("practice4-5"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP bk,hbit1,hbit2;
	int i, j = 0;
	static int mx, my;

	switch (imsg) {
	case WM_CREATE:
		bk = (HBITMAP)LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP4));
		hbit1 = (HBITMAP)LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbit2 = (HBITMAP)LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, bk);
		BitBlt(hdc, 0, 0, 1024, 768, memdc, 0, 0, SRCCOPY);
		
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		hdc = GetDC(hwnd);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hbit1);
		BitBlt(hdc, mx, my, mx + 55, my + 55, memdc, 0, 0, SRCAND);
		SelectObject(memdc, hbit2);
		BitBlt(hdc, mx, my, mx + 55, my + 55, memdc, 0, 0, SRCPAINT);		
		DeleteDC(memdc);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}