#include<windows.h>
#include<stdio.h>
#include"resource.h"
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
	wc.lpszClassName = "notepad";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("notepad", "notepad", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void FileRead(HWND hwnd, LPSTR filename) {
	HANDLE hFile;
	char *buffer;
	DWORD size, CharNum;
	int fileSize;
	hFile = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	fileSize = GetFileSize(hFile, &size);
	buffer = malloc(fileSize+1);
	memset(buffer, 0, fileSize+1);
	ReadFile(hFile, buffer, fileSize+1, &CharNum, NULL);
	buffer[fileSize] = NULL;
	SetWindowText(hwnd, buffer);
	free(buffer);
	CloseHandle(hFile);
}

void FileSave(HWND hwnd, LPTSTR filename) {
	HANDLE hFile;
	LPTSTR buffer;
	int size;
	hFile = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	size = GetWindowTextLength(hwnd);
	buffer = malloc((size + 1) * sizeof(char));
	memset(buffer, 0, (size + 1) * sizeof(char));
	size = GetWindowText(hwnd, (LPTSTR)buffer, size + 1);
	buffer[size] = NULL;
	WriteFile(hFile, buffer, size * sizeof(char), (LPDWORD)&size, NULL);
	free(buffer);
	CloseHandle(hFile);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {

	static HWND hedit;
	RECT rect;

	switch (imsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hedit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 100, rect.right, rect.bottom, hwnd, 1000, hinst, 0);
		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hedit, 0, 0, rect.right, rect.bottom, TRUE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILEOPEN:
			FileRead(hedit, "test2.txt");
			break;
		case ID_FILESAVE:
			FileSave(hedit, "test2.txt");
			break;
		}
		break;
	case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}