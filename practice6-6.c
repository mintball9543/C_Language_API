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
	wc.lpszClassName = "practice6-6";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice6-6", "practice6-6", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND edit1, edit2, edit3;
	static char num1[20] = { "20" }, num2[20] = { "20" }, result[20];
	switch (imsg) {
	case WM_CREATE:
		edit1=CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 200, 100, 30, hwnd, 1000, hinst, 0);
		edit2=CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 200, 100, 30, hwnd, 1001, hinst, 0);
		CreateWindow("button", "+", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 230, 250, 50, 30, hwnd, 2000, hinst, 0);
		CreateWindow("button", "-", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 350, 250, 50, 30, hwnd, 2001, hinst, 0);
		CreateWindow("button", "X", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 230, 300, 50, 30, hwnd, 2002, hinst, 0);
		CreateWindow("button", "/", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 350, 300, 50, 30, hwnd, 2003, hinst, 0);
		edit3=CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 230, 350, 170, 30, hwnd, 1002, hinst, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000: // +
			SendMessage(edit1, EM_GETLINE, 0, num1);
			SendMessage(edit2, EM_GETLINE, 0, num2);
			sprintf(result, "%d", atoi(num1) + atoi(num2));
			SendMessage(edit3, WM_SETTEXT, 0, result);
			break;
		case 2001: // -
			SendMessage(edit1, EM_GETLINE, 0, num1);
			SendMessage(edit2, EM_GETLINE, 0, num2);
			sprintf(result, "%d", atoi(num1) - atoi(num2));
			SendMessage(edit3, WM_SETTEXT, 0, result);
			break;
		case 2002: // *
			SendMessage(edit1, EM_GETLINE, 0, num1);
			SendMessage(edit2, EM_GETLINE, 0, num2);
			sprintf(result, "%d", atoi(num1) * atoi(num2));
			SendMessage(edit3, WM_SETTEXT, 0, result);
			break;
		case 2003: // /
			SendMessage(edit1, EM_GETLINE, 0, num1);
			SendMessage(edit2, EM_GETLINE, 0, num2);
			sprintf(result, "%d", atoi(num1) / atoi(num2));
			SendMessage(edit3, WM_SETTEXT, 0, result);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}