#include<Windows.h>
#include<tchar.h>
#include"resource.h"
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
	wc.lpszClassName = _T("practice1,2");
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice1,2"), _T("practice1,2"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static OPENFILENAME OFN;
	static TCHAR lpstrFile[100] = "";
	static TCHAR filter[] = "C++ 소스와 헤더 파일\0*.c;*.h\0Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	HDC hdc;
	PAINTSTRUCT ps;
	static int size = 0;
	static int x, y, mx, my;
	static HMENU hmenu, hsub, hsub2;
	struct rect {
		int x1;
		int y1;
		int x2;
		int y2;
		COLORREF color;
	};
	static struct rect rect[10] = { 0 };
	int i = 0;
	static int idx = 0;
	static int paint = 0;
	static int temp = 0;
	static int select = 0;
	CHOOSECOLOR COLOR;
	static COLORREF tmp[16];
	HBRUSH hbrush, oldbrush;

	switch (imsg) {
	case WM_CREATE:
		hmenu = GetMenu(hwnd);
		hsub = GetSubMenu(hmenu, 1);
		hsub2 = GetSubMenu(hmenu, 3);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, 0, 0, 40, 40); 
		for (i = 0; rect[i].x1 != 0; i++) {
			if (rect[i].color != NULL) {
				hbrush = CreateSolidBrush(rect[i].color);
				oldbrush = SelectObject(hdc, hbrush);
				Rectangle(hdc, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2);
				SelectObject(hdc, oldbrush);
				DeleteObject(hbrush);
			}
			else
				Rectangle(hdc, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2);
		}
		if (select) {
			Rectangle(hdc, rect[temp].x1 - 7, rect[temp].y1 - 7, rect[temp].x1, rect[temp].y1);
			Rectangle(hdc, rect[temp].x1 - 7, rect[temp].y2 + 7, rect[temp].x1, rect[temp].y2);
			Rectangle(hdc, rect[temp].x2, rect[temp].y1, rect[temp].x2 + 7, rect[temp].y1 - 7);
			Rectangle(hdc, rect[temp].x2, rect[temp].y2, rect[temp].x2 + 7, rect[temp].y2 + 7);
			EnableMenuItem(hsub2, ID_BRUSH, MF_ENABLED);
		}
		else
			EnableMenuItem(hsub2, ID_BRUSH, MF_GRAYED);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_EDITUNDO:
			MessageBox(hwnd, "편집을 취소 하겠습니까?", "편집 취소", MB_OKCANCEL);
			break;
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFile = lpstrFile;
			OFN.lpstrFilter = filter;
			OFN.nMaxFile = 100;
			OFN.lpstrInitialDir = ".";
			if (GetOpenFileName(&OFN) != 0) {
				MessageBox(hwnd, "test", "test", MB_OK);
			}
			break;
		case ID_EDITCOPY:
			x = y = 20;
			size = 20;
			EnableMenuItem(hsub, ID_EDITPASTE, MF_ENABLED);
			break;
		case ID_EDITPASTE:
			hdc = GetDC(hwnd);
			x += 60; y += 60;
			Ellipse(hdc, x - size, y - size, x + size, y + size);
			ReleaseDC(hwnd, hdc);
			break;
		case ID_RECT:
			paint = 1;
			break;
		case ID_BRUSH:
			for (i = 0; i < 16; i++)
				tmp[i] = RGB(0, 0, 0);
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.lpCustColors = tmp;
			COLOR.hwndOwner = hwnd;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0) {
				rect[temp].color = COLOR.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (i = 0;rect[i].x1!=0; i++) {
			if (mx >= rect[i].x1 && my >= rect[i].y1 && mx <= rect[i].x2 && my <= rect[i].y2) {
				temp = i;
				select = 1;
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			}
			else {
				select = 0;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
		}
		if (paint) {
			rect[idx].x1 = LOWORD(lParam);
			rect[idx].y1 = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		if (paint) {
			rect[idx].x2 = LOWORD(lParam);
			rect[idx].y2 = HIWORD(lParam);
			idx++;
			paint = 0;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}