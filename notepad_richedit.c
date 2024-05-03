#include<windows.h>
#include<stdio.h>
#include<Richedit.h>
#include"resource.h"
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
	wc.lpszClassName = "notepad";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hRichedit = LoadLibrary("RICHED20.DLL");
	hwnd = CreateWindow("notepad", "notepad", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(hRichedit);
	return msg.wParam;
}

DWORD CALLBACK StreamInCallback(DWORD dwCookie, LPBYTE buffer, LONG size, LONG* CharNum) {
	HANDLE hFile = (HANDLE)dwCookie;
	ReadFile(hFile, buffer, size, (LPDWORD)CharNum, NULL);
	return(0);
}

DWORD CALLBACK StreamOutCallback(DWORD dwCookie, LPBYTE buffer, LONG size, LONG* CharNum) {
	HANDLE hFile = (HANDLE)dwCookie;
	WriteFile(hFile, buffer, size, (LPDWORD)CharNum, NULL);
	return(0);
}

void FileRead(HWND hwnd, LPTSTR filename) {
	HANDLE hFile;
	EDITSTREAM es;
	hFile = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) return;
	es.dwCookie = (DWORD)hFile;
	es.pfnCallback = StreamInCallback;
	SendMessage(hwnd, EM_STREAMIN, (WPARAM)SF_RTF, (LPARAM)&es);
	CloseHandle(hFile);
}

void FileSave(HWND hwnd, LPTSTR filename) {
	HANDLE hFile;
	EDITSTREAM es;
	hFile = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	es.dwCookie = (DWORD)hFile;
	es.pfnCallback = StreamOutCallback;
	SendMessage(hwnd, EM_STREAMOUT, (WPARAM)SF_RTF, (LPARAM)&es);
	CloseHandle(hFile);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND hRichedit;
	CHOOSEFONT font;
	LOGFONT LogFont;
	RECT rect;
	CHARFORMAT cf;

	switch (imsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hRichedit = CreateWindow("RichEdit20W", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 100, rect.right, rect.bottom, hwnd, 1000, hinst, 0);
		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hRichedit, 0, 0, rect.right, rect.bottom, TRUE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILEOPEN:
			FileRead(hRichedit, "test3.txt");
			break;
		case ID_FILESAVE:
			FileSave(hRichedit, "test3.txt");
			break;
		case ID_FONTDLG:
			memset(&font, 0, sizeof(CHOOSEFONT));
			font.lStructSize = sizeof(CHOOSEFONT);
			font.hwndOwner = hwnd;
			font.lpLogFont = &LogFont;
			font.Flags = CF_EFFECTS | CF_SCREENFONTS;
			if (ChooseFont(&font) != 0) {
				cf.cbSize = sizeof(CHARFORMAT);
				cf.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR;
				cf.yHeight = font.iPointSize * 3;
				cf.dwEffects = NULL;
				strcpy(cf.szFaceName, font.lpLogFont->lfFaceName);
				cf.crTextColor = font.rgbColors;
				SendMessage(hRichedit, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
			}
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}