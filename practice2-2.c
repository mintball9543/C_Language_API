#include<Windows.h>
#include<tchar.h>
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
	wc.lpszClassName = _T("practice2-1");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice2-1"), _T("practice2-1"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
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
	static int x, y, x2, y2;
	static RECT rectview;
	static int flag;
	HPEN hpen, oldpen;
	static int left, right, up, down;

	switch (imsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rectview);
		x = y = 20;
		x2 = 60;
		y2 = 20;
		flag = 0;
		right = 1;
		left = up = down = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, rectview.right - 20, rectview.bottom - 40);
		hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); //블루
		oldpen = (HPEN)SelectObject(hdc, hpen);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //레드
		SelectObject(hdc, hpen);
		Ellipse(hdc, x2 - 20, y2 - 20, x2 + 20, y2 + 20);
		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			if (x + 20 < rectview.right) {
				x2 = x + 40;
				y2 = y;
				right = 1;
				left = up = down = 0;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT) {
			if (x - 20 > rectview.left) {
				x2 = x - 40;
				y2 = y;
				left = 1;
				right = up = down = 0;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_UP) {
			if (y - 20 > rectview.top) {
				y2 = y - 40;
				x2 = x;
				up = 1;
				left = right = down = 0;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN) {
			if (y + 20 < rectview.bottom) {
				y2 = y + 40;
				x2 = x;
				down = 1;
				left = right = up = 0;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		/*if (x + 20 >= rectview.right) x -= 40;
		else if (x - 20 < rectview.left) x += 40;
		else if (y - 20 < rectview.top) y += 40;
		else if (y + 20 >= rectview.bottom) y -= 40;*/

		if (wParam == VK_HOME) x = y = 20;
		InvalidateRgn(hwnd, NULL, TRUE);

		if (wParam == VK_RETURN) {
			if (flag == 0) { //on
				SetTimer(hwnd, 1, 50, NULL);
				flag = 1;
			}
			else if (flag == 1) { //off
				KillTimer(hwnd, 1);
				flag = 0;
			}
		}
		break;
	case WM_TIMER:
		if (right == 1) {
			if (x2 + 80 < rectview.right) {
				x += 40;
				x2 += 40;
			}
		}
		else if (left == 1) {
			if (x2 - 20 > rectview.left) {
				x2 -= 40;
				x -= 40;
			}
		}
		else if (up == 1) {
			if (y2 - 20 > rectview.top) {
				y2 -= 40;
				y -= 40;
			}
		}
		else if (down == 1) {
			if (y2 + 80 < rectview.bottom) {
				y2 += 40;
				y += 40;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}