#include<Windows.h>
#include<tchar.h>
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
	hwnd = CreateWindow("scroll", "scroll", WS_OVERLAPPEDWINDOW|WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND scroll;
	static int p = 0;
	static char buff[256] = { 0 };
	static int temp = 0;
	int i = 0;
	HDC hdc;
	PAINTSTRUCT ps;


	switch (imsg) {
	case WM_CREATE:
		SetScrollRange(hwnd, SB_VERT, 0, 255, TRUE);
		SetScrollPos(hwnd, SB_VERT, 0, TRUE);
	//	ShowScrollBar(hwnd, SB_VERT, TRUE);
		break;
	case WM_VSCROLL:

		//SendMessage(hwnd, SBM_SETPOS, 255, FALSE);
		
		//p = SendMessage(hwnd, SBM_GETPOS, 0, 0);
		p = GetScrollPos(hwnd, SB_VERT);
		if (LOWORD(wParam) == SB_LINEDOWN) {
			p++;
			if (p > 255)
				p = 255;
			//SendMessage(hwnd, SBM_SETPOS, p, TRUE);
			SetScrollPos(hwnd, SB_VERT, p, TRUE);
		}
		else if (LOWORD(wParam) == SB_LINEUP) {
			p--;
			if (p < 0)
				p = 0;
			//SendMessage(hwnd, SBM_SETPOS, p, TRUE);
			SetScrollPos(hwnd, SB_VERT, p, TRUE);
		}
		else if (LOWORD(wParam) == SB_PAGEDOWN) {
			p += 5;
			if (p > 255)
				p = 255;
			//SendMessage(hwnd, SBM_SETPOS, p, TRUE);
			SetScrollPos(hwnd, SB_VERT, p, TRUE);
		}
		else if (LOWORD(wParam) == SB_PAGEUP) {
			p -= 5;
			if (p < 0)
				p = 0;
			//SendMessage(hwnd, SBM_SETPOS, p, TRUE);
			SetScrollPos(hwnd, SB_VERT, p, TRUE);
		}
		else if (LOWORD(wParam) == SB_THUMBTRACK) {
			SetScrollPos(hwnd, SB_VERT, HIWORD(wParam), TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = p; i < 300+p; i++) {
			TextOut(hdc, 0, i*20, itoa(i,buff,_tcslen(i)), _tcslen(buff));
		}
		
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