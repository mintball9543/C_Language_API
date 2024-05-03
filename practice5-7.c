#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT Imsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	wc.lpszClassName = "practice5-7";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice5-7", "practice5-7", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

WNDPROC oldproc;
static HWND edit;
static HWND list;
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static char temp[126] = { 0, };

	switch (imsg) {
	case WM_INITDIALOG:
		list = CreateWindow("listbox", "", WS_VSCROLL | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 50, 800, 300, hDlg, 1000, hinst, 0);
		edit = CreateWindow("edit", "", ES_MULTILINE | ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 370, 700, 100, hDlg, 1001, hinst, 0);
		CreateWindow("button", "종료하기", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 760, 380, 80, 70, hDlg, 2000, hinst, 0);
		oldproc = SetWindowLongPtr(edit, GWL_WNDPROC, (LONG)EditProc);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char temp[126] = { 126, };
	
	switch (msg) {
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			SendMessage(edit, EM_GETLINE, 0, temp);
			SendMessage(list, LB_INSERTSTRING, 0, temp);
			SendMessage(edit, WM_SETTEXT, 0, "");
		}
		return 0;
	}

	return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static LPDLGTEMPLATE dlg = 0;
	HGLOBAL hglobal;

	switch (imsg) {
	case WM_CREATE:
		hglobal = GlobalAlloc(GHND, 2048);
		dlg = (LPDLGTEMPLATE)GlobalLock(hglobal);
		dlg->cdit = 0;
		dlg->x = 100;
		dlg->y = 100;
		dlg->cx = 500;
		dlg->cy = 300;
		dlg->dwExtendedStyle = 0;
		dlg->style = WS_SYSMENU | DS_CENTER;


		break;
	case WM_LBUTTONDOWN:
		DialogBoxIndirect(hinst, dlg, hwnd, dialog);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}