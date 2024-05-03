#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
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
	wc.lpszClassName = "scroll";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("scroll", "scroll", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
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
	static HWND scroll;
	static char num[10];
	static int p = 0;
	static int val = 0;
	static char buff[1024] = { 0 };
	HBRUSH hbrush;
	switch (imsg) {
	case WM_CREATE:
		scroll=CreateWindow("scrollbar", "", SBS_VERT |WS_CHILD, 100, 100, 20, 100, hwnd, 0, hinst, 0);
		SetScrollRange(scroll, SB_CTL, 0, 255, TRUE);
		SetScrollPos(scroll, SB_CTL, 0, TRUE);
		ShowScrollBar(scroll, SB_CTL, TRUE);
		break;
	case WM_VSCROLL:
		memset(buff, 0, 1024);
		if (lParam == scroll) {
			p = SendMessage(scroll, SBM_GETPOS, 0, 0);
			if (LOWORD(wParam) == SB_LINEDOWN) {
				p++;
				if (p >= 256)
					p = 255;
				SendMessageA(scroll, SBM_SETPOS, p, TRUE);
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == SB_LINEUP) {
				p--;
				if (p < 0)
					p = 0;
				SendMessage(scroll, SBM_SETPOS, p, TRUE);
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == SB_THUMBTRACK) {
				p = HIWORD(wParam);
				//sprintf(buff, "%d", HIWORD(wParam));
				//p = SendMessage(scroll, SBM_GETPOS, 0, 0);
				SendMessage(scroll, SBM_SETPOS, HIWORD(wParam), TRUE);
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == SB_PAGEDOWN) {
				p+=5;
				if (p > 255)
					p = 255;
				SendMessage(scroll, SBM_SETPOS, p, TRUE);
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			else if (LOWORD(wParam) == SB_PAGEUP) {
				p-=5;
				if (p < 0)
					p = 0;
				SendMessage(scroll, SBM_SETPOS, p, TRUE);
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			itoa(p, num, 10);
			hdc = GetDC(hwnd);
			TextOut(hdc, 150, 130, num, _tcslen(num));
			//TextOut(hdc, 150, 100, buff, strlen(buff));
			ReleaseDC(hwnd, hdc);
			
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hbrush = CreateSolidBrush(RGB(0, 0, p));
		SelectObject(hdc, hbrush);
		Rectangle(hdc, 0, 100, 100, 200);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	switch (imsg) {
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		}
		break;
	}
	return 0;
}