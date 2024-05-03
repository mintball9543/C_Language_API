#include<windows.h>
#include<stdio.h>
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
	wc.lpszClassName = "file";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("file", "file", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	OPENFILENAME ofn;
	HMENU menu, file;
	static char filename[100], path[100];
	HANDLE hFile;
	DWORD size,charnum;
	int filesize;
	char *buffer;
	static HWND edit;
	RECT rect;

	switch (imsg) {
	case WM_CREATE:
		menu = CreateMenu();
		file = CreatePopupMenu();
		AppendMenu(menu,MF_POPUP | MF_STRING, file, "파일");
		AppendMenu(file, MF_STRING, 1000, "열기");
		AppendMenu(file, MF_STRING, 2000, "저장");
		SetMenu(hwnd, menu);
		GetClientRect(hwnd, &rect);
		edit = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE, 0, 0, rect.right, rect.bottom, hwnd, 1001, hinst, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1000:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = path;
			ofn.nMaxFileTitle = 100;
			ofn.nMaxFile=1000;
			ofn.lpstrFilter="*.txt\0*.txt*\0";
			ofn.Flags = OFN_EXPLORER;
			ofn.lpstrFileTitle = filename;
			ofn.hInstance = hinst;
			GetOpenFileName(&ofn);
			hFile = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			filesize = GetFileSize(hFile, &size);
			buffer = malloc((filesize+1) * sizeof(char));
			memset(buffer, 0, filesize);
			ReadFile(hFile, buffer, filesize, &charnum, NULL);
			buffer[filesize] = NULL;
			SetWindowText(edit, buffer);
			free(buffer);
			CloseHandle(hFile);
			break;
		case 2000:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = path;
			ofn.nMaxFileTitle = 100;
			ofn.nMaxFile = 1000;
			ofn.lpstrFilter = "*.txt\0*.txt*\0";
			ofn.Flags = OFN_EXPLORER;
			ofn.lpstrFileTitle = filename;
			ofn.hInstance = hinst;
			GetSaveFileName(&ofn);
			hFile = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			size=GetWindowTextLength(edit);
			buffer = malloc((size+1) * sizeof(char));
			memset(buffer, 0, size);
			GetWindowText(edit, (LPSTR)buffer, size + 1);
			buffer[size] = NULL;
			WriteFile(hFile, buffer, size, &charnum, 0);
			free(buffer);
			CloseHandle(hFile);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	DefWindowProc(hwnd, imsg, wParam, lParam);
}