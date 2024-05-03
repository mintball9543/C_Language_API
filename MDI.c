#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
#include"resource.h"
//LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

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
	switch (imsg) {
	case WM_CREATE:
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, imsg, wParam, lParam);
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	static HWND hwndChild;

	switch (imsg) {
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, hinst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW);
		
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILENEW:
			mdicreate.szClass = "Child Window Class Name";
			mdicreate.szTitle = "Child Window Title Name";
			mdicreate.hOwner = hinst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			break;
		case ID_WINDOW_CLOSE:
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
			if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
				SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0);
			return 0;
		case ID_TILE:
			SendMessage(hwndClient, WM_MDITILE, 0, 0);
			return 0;
		case ID_CASCADE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			return 0;
		case ID_ARRANGE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			return 0;
		/*case ID_EXIT:
			PostQuitMessage(0);
			return 0;*/
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hwnd, hwndClient, imsg, wParam, lParam);
}


/*LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {


	switch (imsg) {
	case WM_CREATE:
		
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}*/