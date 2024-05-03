#include<windows.h>
#include<Richedit.h>
#include"resource.h"
#include<stdio.h>
HINSTANCE hinst;
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	HINSTANCE hRichedit;
	hinst = hInstance;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "richedit";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hRichedit = LoadLibrary("RICHED20.DLL");
	hwnd = CreateWindow("richedit", "richedit", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(hRichedit);
	return msg.wParam;
}

#define IDC_RICHEDIT 100
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND hRichedit;
	RECT rect;
	CHARFORMAT cf;

	switch (imsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hRichedit = CreateWindow("richedit20w", "¿ì¸®´Â ¹ÎÁ· ÁßÈïÀÇ ¿ª»çÀû »ç¸íÀ» ¶ì°í ÀÌ ¶¥¿¡ ÅÂ¾î³µ´Ù", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_BORDER, 
			100, 100, rect.right, rect.bottom, hwnd, IDC_RICHEDIT, hinst, NULL);
		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hRichedit, 0, 0, rect.right, rect.bottom, TRUE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FONT:
			cf.cbSize = sizeof(CHARFORMAT);
			cf.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR;
			cf.yHeight = 300;
			cf.dwEffects = NULL;
			strcpy(cf.szFaceName, "HY³ª¹«B");
			cf.crTextColor = RGB(100, 0, 0);
			SendMessage(hRichedit, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
			return 0;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}