#include<Windows.h>
#include<tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hpreInstance, LPSTR lpszCmdLine, int nCmdShow) {
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
	wc.lpszClassName = _T("color");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("color"), _T("color"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
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
	CHOOSECOLOR COLOR;
	static COLORREF tmp[16], color;
	HBRUSH hbrush, oldbrush;
	int i;
	HMENU menu,edit;

	switch (imsg) {
	case WM_CREATE:
		menu=CreateMenu();
		edit=CreatePopupMenu();
		AppendMenu(menu, MF_POPUP | MF_STRING, edit,_T("편집"));
		AppendMenu(edit, MF_STRING, 4000, _T("색상 변경"));
		SetMenu(hwnd, menu);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hbrush = CreateSolidBrush(color);
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		Ellipse(hdc, 10, 10, 200, 200);
		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 4000:
			for (i = 0; i < 16; i++)
				tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&color, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = tmp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0) {
				color = COLOR.rgbResult;
				InvalidateRgn(hwnd, NULL, TRUE);
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