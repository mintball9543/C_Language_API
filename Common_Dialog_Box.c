#include<windows.h>
#include<stdio.h>
#include"resource.h"
HINSTANCE hinst;
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);

void ReadFromFile(int WndIndex, char filename[]);
HWND hwndChild[100];
static char WinBuff[100][1000];
int WndCount;

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
	wc.lpfnWndProc = FrameWndProc;
	wc.lpszClassName = "common dialog";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	wc.lpfnWndProc = ChildWndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Child Window Class Name";
	RegisterClass(&wc);
	hwnd = CreateWindow("common dialog", "common dialog", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	static char filepath[1000], folder[100], filename[100];
	LPSTR pStr;
	static char filter[] = "텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0";
	OPENFILENAME ofn;

	switch (imsg) {
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, hinst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILEOPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = filepath;
			ofn.nMaxFileTitle = 100;
			ofn.lpstrFileTitle = filename;
			ofn.nMaxFile = 1000;
			ofn.lpstrFilter = filter;
			ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
			GetOpenFileName(&ofn);
			pStr = filepath;
			if (!*pStr) break;
			strcpy(folder, pStr);
			pStr = pStr + strlen(pStr) + 1;
			if (!*pStr) {
				WndCount++;
				ReadFromFile(WndCount, filepath);
				mdicreate.szClass = "Child Window Class Name";
				mdicreate.szTitle = folder;
				mdicreate.hOwner = hinst;
				mdicreate.x = CW_USEDEFAULT;
				mdicreate.y = CW_USEDEFAULT;
				mdicreate.cx = CW_USEDEFAULT;
				mdicreate.cy = CW_USEDEFAULT;
				mdicreate.style = 0;
				mdicreate.lParam = 0;
				hwndChild[WndCount] = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
				return 0;
			}
			while (*pStr) {
				strcpy(filename, pStr);
				pStr = pStr + strlen(pStr) + 1;
				WndCount++;
				ReadFromFile(WndCount, filename);
				mdicreate.szClass = "Child Window Class Name";
				mdicreate.szTitle = folder;
				mdicreate.hOwner = hinst;
				mdicreate.x = CW_USEDEFAULT;
				mdicreate.y = CW_USEDEFAULT;
				mdicreate.cx = CW_USEDEFAULT;
				mdicreate.cy = CW_USEDEFAULT;
				mdicreate.style = 0;
				mdicreate.lParam = 0;
				hwndChild[WndCount] = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);

			}
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefFrameProc(hwnd, hwndClient, imsg, wParam, lParam);
}

void ReadFromFile(int WndIndex, char filename[]) {
	HANDLE hFile;
	DWORD size = 1000;
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	ReadFile(hFile, WinBuff[WndIndex], size * sizeof(char), &size, NULL);
	WinBuff[WndIndex][size] = NULL;
	CloseHandle(hFile);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	int i, SelectWnd;
	HDC hdc;
	RECT rt;
	PAINTSTRUCT ps;

	for (i = 1; i <= WndCount; i++) {
		if (hwnd == hwndChild[i]) {
			SelectWnd = i;
			break;
		}
	}

	switch (imsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rt);
		DrawText(hdc, WinBuff[SelectWnd], (int)strlen(WinBuff[SelectWnd]), &rt, DT_TOP | DT_LEFT);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, imsg, wParam, lParam);
}