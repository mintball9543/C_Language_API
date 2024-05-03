#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
#include"resource.h"
//LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hinst;
HWND ChildHwnd[2];
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
	wc.lpfnWndProc = FrameWndProc;
	wc.lpszClassName = "Window Class Name";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	wc.lpfnWndProc = ChildWndProc;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Child Window Class Name";
	RegisterClass(&wc);
	hwnd = CreateWindow("Window Class Name", "Main Window Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	static int x[2] = { 20,20 }, y[2] = { 20,20 }, flag[2];
	int select;

	switch (imsg) {
	case WM_CREATE:
		x[wParam] = x[wParam] + 20;
		hdc = GetDC(hwnd);
		Ellipse(hdc, x[wParam] - 20, y[wParam] - 20, x[wParam] + 20, y[wParam] + 20);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONDOWN:
		if (hwnd == ChildHwnd[0])
			select = 0;
		else
			select = 1;
		flag[select] = 1 - flag[select];
		if (flag[select])
			SetTimer(hwnd, select, 100, NULL);
		else
			KillTimer(hwnd, select);
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, imsg, wParam, lParam);
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	RECT rectview;
	static BOOL split;
	static HCURSOR hcursor;
	static int boundary = -1;

	switch (imsg) {
	case WM_CREATE:
		split = FALSE;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SPLIT_2x1:
			if (split == TRUE) break;
			split = TRUE;
			GetClientRect(hwnd, &rectview);
			ChildHwnd[0] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window Class Name", NULL, WS_CHILD | WS_VISIBLE, 0, 0, rectview.right, rectview.bottom / 2 - 1, hwnd, NULL, hinst, NULL);
			ChildHwnd[1] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window Class Name", NULL, WS_CHILD | WS_VISIBLE, 0, rectview.bottom / 2 + 1, rectview.right, rectview.bottom / 2 - 1, hwnd, NULL, hinst, NULL);
			boundary = rectview.bottom / 2;
			hcursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
			return 0;
		}
		return 0;
	case WM_MOUSEMOVE:
		if (HIWORD(lParam) >= boundary - 2 && HIWORD(lParam) <= boundary + 2) SetCursor(hcursor);
		if (wParam == MK_LBUTTON && split == TRUE) {
			GetClientRect(hwnd, &rectview);
			if (rectview.top + 5 < HIWORD(lParam) && HIWORD(lParam) < rectview.bottom - 5) boundary = HIWORD(lParam);
			MoveWindow(ChildHwnd[0], 0, 0, rectview.right, boundary - 1, TRUE);
			MoveWindow(ChildHwnd[1], 0, boundary + 1, rectview.right, rectview.bottom - boundary + 1, TRUE);
		}
		return 0;
	case WM_LBUTTONDOWN:
		if (split == TRUE) {
			SetCursor(hcursor);
			SetCapture(hwnd);
		}
		return 0;
	case WM_LBUTTONUP:
		if (split == TRUE) ReleaseCapture();
		return 0;
	case WM_SIZE:
		if (split == TRUE) {
			GetClientRect(hwnd, &rectview);
			MoveWindow(ChildHwnd[0], 0, 0, rectview.right, rectview.bottom / 2 - 1, TRUE);
			MoveWindow(ChildHwnd[1], 0, rectview.bottom / 2 + 1, rectview.right, rectview.bottom / 2 - 1, TRUE);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}