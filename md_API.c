#include<Windows.h>
#include<tchar.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
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
	wc.lpszClassName = "md";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("md", "md", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND hList;
	static int selection;
	static char name[20];

	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("static", "회원이름", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 50, 100, 20, hDlg, 1000, hinst, 0);
		CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE, 90, 100, 100, 20, hDlg, 1001, hinst, 0);
		CreateWindow("button", "가입", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 90, 150, 100, 20, hDlg, 2000, hinst, 0);
		CreateWindow("button", "탈퇴", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 90, 180, 100, 20, hDlg, 2001, hinst, 0);
		CreateWindow("button", "종료", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 90, 210, 100, 20, hDlg, 2002, hinst, 0);
		CreateWindow("static", "회원명단", SS_LEFT | WS_CHILD | WS_VISIBLE, 350, 50, 100, 20, hDlg, 3000, hinst, 0);
		hList = CreateWindow("listbox", "", LBS_STANDARD | WS_CHILD | WS_VISIBLE, 340, 100, 100, 200, hDlg, 3001, hinst, 0);
		//hList = GetDlgItem(hDlg, IDC_LIST_NAME);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			GetDlgItemText(hDlg, 1001, name, 20);
			SendMessage(hList, LB_ADDSTRING, 0, name);
			break;
		case 2001:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			break;
		case 3001:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		case IDCANCEL:
			//DestroyWindow(hDlg);
			//hDlg = NULL;
			EndDialog(hDlg, 0);
			break;
		case 2002:
			//DestroyWindow(hDlg);
			//hDlg = NULL;
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HWND hDlg = NULL;
	HMENU hmenu, dlg;
	static LPDLGTEMPLATE md;
	HGLOBAL hglobal;
	hglobal = GlobalAlloc(GHND, 1000);
	md = GlobalLock(hglobal);
	md->cdit = 0;
	md->x = 100;
	md->y = 100;
	md->cx = 500;
	md->cy = 300;
	md->dwExtendedStyle = 0;
	switch (imsg) {
	case WM_CREATE:
		hmenu = CreateMenu();
		dlg = CreatePopupMenu();
		AppendMenu(hmenu, MF_POPUP | MF_STRING, dlg, "다이얼로그");
		AppendMenu(dlg, MF_STRING, 100, "모덜");
		AppendMenu(dlg, MF_STRING, 101, "모덜리스");
		SetMenu(hwnd, hmenu);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 100:
			md->style = WS_SYSMENU | DS_CENTER;
			//hDlg = CreateDialogIndirect(hinst, md, hwnd, dialog);
			//ShowWindow(hDlg, SW_SHOW);
			DialogBoxIndirect(hinst, md, hwnd, dialog);
			return 0;
		case 101:
			md->style = WS_SYSMENU | DS_CENTER;
			hDlg = CreateDialogIndirect(hinst, md, hwnd, dialog);
			ShowWindow(hDlg, SW_SHOW);
			return 0;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}