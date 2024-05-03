#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
#include<CommCtrl.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT Imsg, WPARAM wParam, LPARAM lParam);

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
	wc.lpszClassName = "practice5-8";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice5-8", "practice5-8", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND cb, name, phone, female, male, list;
	int i = 0;
	char temp[10] = { 0 };
	static char name1[64] = { 64, };
	static char phone1[128] = { 128, };
	static char year[4];
	static int sex = 1;
	static int selection = 20;
	static int selection2 = 0;
	char addlist[126] = { 0, };

	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("static", "회원이름", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 100, 100, 30, hDlg, 1000, hinst, 0);
		name = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 100, 150, 20, hDlg, 2000, hinst, 0);
		CreateWindow("static", "전화번호", SS_LEFT | WS_CHILD | WS_VISIBLE, 340, 100, 100, 30, hDlg, 1001, hinst, 0);
		phone = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 410, 100, 200, 20, hDlg, 2001, hinst, 0);
		CreateWindow("static", "성       별", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 135, 100, 30, hDlg, 1002, hinst, 0);
		female = CreateWindow("button", "여자", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 170, 130, 100, 30, hDlg, 2002, hinst, 0);
		male = CreateWindow("button", "남자", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 260, 130, 100, 30, hDlg, 2003, hinst, 0);
		CreateWindow("static", "출생년도", SS_LEFT | WS_CHILD | WS_VISIBLE, 340, 135, 100, 30, hDlg, 1003, hinst, 0);
		cb = CreateWindow("combobox", "", CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE, 410, 130, 200, 200, hDlg, 2004, hinst, 0);
		CreateWindow("button", "새회원", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 130, 170, 90, 30, hDlg, 3000, hinst, 0);
		CreateWindow("button", "가입", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 250, 170, 90, 30, hDlg, 3001, hinst, 0);
		CreateWindow("button", "탈퇴", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 370, 170, 90, 30, hDlg, 3002, hinst, 0);
		CreateWindow("button", "종료하기", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 490, 170, 90, 30, hDlg, 3003, hinst, 0);
		CreateWindow("static", "회원명단", SS_LEFT | WS_CHILD | WS_VISIBLE, 120, 230, 100, 30, hDlg, 1004, hinst, 0);
		list = CreateWindow("listbox", "", LBS_STANDARD | WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 250, 510, 300, hDlg, 4000, hinst, 0);
		//CreateWindow("scrollbar", "", SBS_VERT | WS_CHILD | WS_VISIBLE, 100, 100, 10, 100, cb, 5000, hinst, 0);

		for (i = 2020; i >= 1900; i--)
			SendMessage(cb, CB_ADDSTRING, 0, itoa(i,temp,10));
		SendMessage(cb, CB_SELECTSTRING, -1, "2000");
		SendMessage(female, BM_SETCHECK, BST_CHECKED, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 4000:
			selection2 = SendMessage(list, LB_GETCURSEL, 0, 0);
			break;
		case 2002:
			if (SendMessage(female, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(female, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(male, BM_SETCHECK, BST_UNCHECKED, 0);
				sex = 1;
			}
			break;
		case 2003:
			if (SendMessage(male, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(male, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(female, BM_SETCHECK, BST_UNCHECKED, 0);
				sex = 0;
			}
			break;
		case 2004:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				selection = SendMessage(cb, CB_GETCURSEL, 0, 0);
			break;
		case 3001:
			SendMessage(name, EM_GETLINE, 0, name1);
			SendMessage(phone, EM_GETLINE, 0, phone1);
			SendMessage(cb, CB_GETLBTEXT, selection, year);

			sprintf(addlist, "이름: %s,전화번호: %s, 성별: %s, 출생연도: %s", name1, phone1, sex ? "여자" : "남자", year);
			SendMessage(list, LB_ADDSTRING, 0, addlist);
			memset(name1, 0, 64);
			memset(phone1, 0, 128);
			name1[0] = 64;
			phone1[0] = 128;
			break;
		case 3002:
			SendMessage(list, LB_DELETESTRING, selection2, 0);
			break;
		case 3000:
			SendMessage(name, WM_SETTEXT, 0, "");
			SendMessage(phone, WM_SETTEXT, 0, "");
			SendMessage(cb, CB_SELECTSTRING, -1, "2000");
			SendMessage(female, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(male, BM_SETCHECK, BST_UNCHECKED, 0);
			sex = 1;
			break;
		case 3003:
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