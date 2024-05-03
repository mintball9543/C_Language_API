#include<Windows.h>
#include<tchar.h>

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HGLOBAL hglobal;
	hinst = hInstance;
	hglobal=GlobalAlloc(GHND,1000);
	LPDLGTEMPLATE dlg;
	dlg= GlobalLock(hglobal);
	dlg->cdit = 0;
	dlg->dwExtendedStyle = 0;
	dlg->x = 100;
	dlg->y = 100;
	dlg->cx = 500;
	dlg->cy = 300;
	dlg->style = WS_SYSMENU | DS_CENTER;
	DialogBoxIndirect(hInstance, dlg, 0, dialog);
	return 0;
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
		hList=CreateWindow("listbox", "", LBS_STANDARD | WS_CHILD | WS_VISIBLE, 340, 100, 100, 200, hDlg, 3001, hinst, 0);
		//hList = GetDlgItem(hDlg, 3001);
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
			EndDialog(hDlg, 0);
			break;
		case 2002:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}

	return 0;
}