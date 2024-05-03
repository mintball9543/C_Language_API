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
	wc.lpszClassName = _T("practice7");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("practice7"), _T("practice7"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc,hdc2,hdc3;
	PAINTSTRUCT ps;
	HBRUSH hbrush, oldbrush;
	/*RECT rt1 = {};
	RECT rt2 = {};
	RECT rt3 = {};
	RECT rt4 = {};*/

	switch (imsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 300, 0, 400, 200);  //top
		TextOut(hdc, 335, 100, _T("위쪽"), _tcslen(_T("위쪽")));
		Rectangle(hdc, 200, 200, 300, 400); //left
		TextOut(hdc, 235, 300, _T("왼쪽"), _tcslen(_T("왼쪽")));
		Rectangle(hdc, 400, 200, 500, 400); //right
		TextOut(hdc, 425, 300, _T("오른쪽"), _tcslen(_T("오른쪽")));
		Rectangle(hdc, 300, 400, 400, 600); //bottom
		TextOut(hdc, 325, 500, _T("아래쪽"), _tcslen(_T("아래쪽")));
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		hdc2 = GetDC(hwnd);
		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		oldbrush = (HBRUSH)SelectObject(hdc2, hbrush);
		if (wParam == VK_UP) Rectangle(hdc2, 300, 0, 400, 200);  //top	
		if (wParam == VK_LEFT) Rectangle(hdc2, 200, 200, 300, 400); //left
		if(wParam==VK_RIGHT) Rectangle(hdc2, 400, 200, 500, 400); //right
		if(wParam==VK_DOWN) Rectangle(hdc2, 300, 400, 400, 600); //bottom
		SelectObject(hdc2, oldbrush);
		ReleaseDC(hwnd, hdc2);
		break;
	case WM_KEYUP:
		/*hdc3 = GetDC(hwnd);
		Rectangle(hdc3, 300, 0, 400, 200);  //top
		TextOut(hdc3, 335, 100, _T("위쪽"), _tcslen(_T("위쪽")));
		Rectangle(hdc3, 200, 200, 300, 400); //left
		TextOut(hdc3, 235, 300, _T("왼쪽"), _tcslen(_T("왼쪽")));
		Rectangle(hdc3, 400, 200, 500, 400); //right
		TextOut(hdc3, 425, 300, _T("오른쪽"), _tcslen(_T("오른쪽")));
		Rectangle(hdc3, 300, 400, 400, 600); //bottom
		TextOut(hdc3, 325, 500, _T("아래쪽"), _tcslen(_T("아래쪽")));
		ReleaseDC(hwnd, hdc3);*/
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}