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
	int i = 0;
	static HWND combo1, combo2, combo3, front;
	char temp[20] = { 0 };
	int selection;
	static char year[20] = { 0 }, result[20] = { 0 };
	static int month = NULL, day = NULL;
	switch (imsg) {
	case WM_CREATE:
		CreateWindow("static", "연도선택", ES_LEFT | WS_CHILD | WS_VISIBLE, 100, 100, 70, 20, hwnd, 1000, hinst, 0);
		CreateWindow("static", "월 선택", ES_CENTER | WS_CHILD | WS_VISIBLE, 200, 100, 70, 20, hwnd, 1001, hinst, 0);
		CreateWindow("static", "일 선택", ES_CENTER | WS_CHILD | WS_VISIBLE, 300, 100, 70, 20, hwnd, 1002, hinst, 0);
		combo1=CreateWindow("combobox", "", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 90, 130, 90, 200, hwnd, 2000, hinst, 0);
		combo2=CreateWindow("combobox", "", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 190, 130, 90, 200, hwnd, 2001, hinst, 0);
		combo3=CreateWindow("combobox", "", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 290, 130, 90, 200, hwnd, 2002, hinst, 0);
		CreateWindow("static", "주민번호 앞자리-주민번호 뒷자리", SS_CENTER | WS_CHILD | WS_VISIBLE, 80, 170, 300, 20, hwnd, 1003, hinst, 0);
		front=CreateWindow("edit", "", ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 150, 200, 70, 20, hwnd, 3000, hinst, 0);
		CreateWindow("edit", "2031456", ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 235, 200, 70, 20, hwnd, 3001, hinst, 0);
		for (i = 1900; i < 2021; i++) {
			itoa(i, temp, 10);
			SendMessage(combo1, CB_ADDSTRING, 0, temp);
		}
		for (i = 1; i < 13; i++) {
			itoa(i, temp, 10);
			SendMessage(combo2, CB_ADDSTRING, 0, temp);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				selection = SendMessage(combo1, CB_GETCURSEL, 0, 0);
				itoa(selection + 1900, year, 10);
				sprintf(result, "%c%c%02d%02d", year[2], year[3], month, day);
				SendMessage(front, WM_SETTEXT, 0, result);
			}
			break;
		case 2001:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				while (1)
					if (SendMessage(combo3, CB_DELETESTRING, 0, 0) == CB_ERR)
						break;

				selection = SendMessage(combo2, CB_GETCURSEL, 0, 0);

				if (selection + 1 == 2) {
					for (i = 1; i <= 28; i++) {
						itoa(i, temp, 10);
						SendMessage(combo3, CB_ADDSTRING, 0, temp);
					}
				}
				else if (selection + 1 == 1 || selection + 1 == 3 || selection + 1 == 5 || selection + 1 == 7 || selection + 1 == 8 || selection + 1 == 10 || selection + 1 == 12) {
					for (i = 1; i <= 31; i++) {
						itoa(i, temp, 10);
						SendMessage(combo3, CB_ADDSTRING, 0, temp);
					}
				}
				else {
					for (i = 1; i <= 30; i++) {
						itoa(i, temp, 10);
						SendMessage(combo3, CB_ADDSTRING, 0, temp);
					}
				}
				month = selection + 1;
				sprintf(result, "%c%c%02d%02d", year[2], year[3], month, day);
				SendMessage(front, WM_SETTEXT, 0, result);
			}
			break;
		case 2002:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				selection = SendMessage(combo3, CB_GETCURSEL, 0, 0);
				day = selection + 1;
				sprintf(result, "%c%c%02d%02d", year[2], year[3], month, day);
				SendMessage(front, WM_SETTEXT, 0, result);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}