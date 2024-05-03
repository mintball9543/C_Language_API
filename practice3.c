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
	wc.lpszClassName = _T("practice3");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice3"), _T("pratice3"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR* str[10];
	static int count,ypos;
	//static int srelength[10];
	RECT rt = { 0,0,1000,1000 };
	int i = 0;

	switch (imsg) {
	case WM_CREATE:
		for (i = 0; i < 10; i++) {
			str[i] = (char)malloc(sizeof(char) * 100);
			memset(str[i], 0, 100);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; i <= ypos; i++) {

		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (ypos == 10&&wParam==VK_RETURN) break;
		if (count == 100 && ypos < 10) {
			sprintf((str + count - 1), "\n");
			ypos++;
			count = 0;
		}
		else if (count == 100 && ypos == 10) if(wParam!=VK_BACK) break;
		if (wParam == VK_RETURN) {
			ypos++;
			count = 0;
		}
		str[ypos][count++] = wParam;
		str[ypos][count] = 0;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}