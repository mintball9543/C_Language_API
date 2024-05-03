#include<windows.h>
#include<stdio.h>
HINSTANCE hinst;
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
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
	wc.lpszClassName = "practice6-7";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice6-7", "practice6-7", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt[5];
	static RECT test;
	int i = 0;
	static HWND combo[5];
	char temp[20];

	switch (imsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 100, 100, 130);
		MoveToEx(hdc, 100, 115, NULL);
		LineTo(hdc, 200, 115);
		rt[0].left = 0;	rt[0].top = 100;	rt[0].right = 100;	rt[0].bottom = 130;
		Rectangle(hdc, 200, 100, 300, 130);
		MoveToEx(hdc, 300, 115, NULL);
		LineTo(hdc, 400, 115);
		rt[1].left = 200;	rt[1].top = 100;	rt[1].right = 300;	rt[1].bottom = 130;
		Rectangle(hdc, 400, 100, 500, 130);
		MoveToEx(hdc, 500, 115, NULL);
		LineTo(hdc, 600, 115);
		rt[2].left = 400;	rt[2].top = 100;	rt[2].right = 500;	rt[2].bottom = 130;
		Rectangle(hdc, 600, 100, 700, 130);
		MoveToEx(hdc, 700, 115, NULL);
		LineTo(hdc, 800, 115);
		rt[3].left = 600;	rt[3].top = 100;	rt[3].right = 700;	rt[3].bottom = 130;
		Rectangle(hdc, 800, 100, 900, 130);
		rt[4].left = 800;	rt[4].top = 100;	rt[4].right = 900;	rt[4].bottom = 130;
		EndPaint(hwnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		for (i = 0; i < 5; i++) {
			if (LOWORD(lParam) > rt[i].left && LOWORD(lParam) < rt[i].right && HIWORD(lParam) > rt[i].top && HIWORD(lParam) < rt[i].bottom) {
				combo[i]=CreateWindow("combobox", 0, CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, rt[i].left, rt[i].top, 100, 100, hwnd, i, hinst, 0);
				SendMessage(combo[i], CB_ADDSTRING, 0, "역전");
				SendMessage(combo[i], CB_ADDSTRING, 0, "시장");
				SendMessage(combo[i], CB_ADDSTRING, 0, "초등학교");
				SendMessage(combo[i], CB_ADDSTRING, 0, "중학교");
				SendMessage(combo[i], CB_ADDSTRING, 0, "고등학교");
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	DefWindowProc(hwnd, imsg, wParam, lParam);
}