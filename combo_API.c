#include<Windows.h>
#include<tchar.h>
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	LPDLGTEMPLATE dlg;
	HGLOBAL hglobal;
	hglobal = GlobalAlloc(GHND, 500);
	dlg = GlobalLock(hglobal);
	dlg->cdit = 0;
	dlg->x = 100;
	dlg->y = 100;
	dlg->cx = 300;
	dlg->cy = 300;
	dlg->dwExtendedStyle = 0;
	dlg->style = WS_SYSMENU | DS_CENTER;
	DialogBoxIndirect(hInstance, dlg, 0, dialog);

	return 0;
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static char name[20];
	static int selection;
	static HWND hCombo;

	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("static", "È¸¿øÀÌ¸§", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 100, 100, 50, hDlg, 1000, hinst, 0);
		CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE, 95, 120, 100, 20, hDlg, 1001, hinst, 0);
		CreateWindow("static", "È¸¿ø¸í´Ü", SS_LEFT | WS_CHILD | WS_VISIBLE, 300, 100, 100, 50, hDlg, 2000, hinst, 0);
		CreateWindow("combobox", "", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE, 290, 120, 100, 100, hDlg, 2001, hinst, 0);
		CreateWindow("button", "°¡ÀÔ", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 95, 250, 100, 20, hDlg, 3000, hinst, 0);
		CreateWindow("button", "Á¾·á", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 95, 300, 100, 20, hDlg, 3001, hinst, 0);
		CreateWindow("button", "Å»Åð", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 300, 250, 100, 20, hDlg, 3002, hinst, 0);
		hCombo = GetDlgItem(hDlg, 2001);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 3000://°¡ÀÔ
			GetDlgItemText(hDlg, 1001, name, 20);
			SendMessage(hCombo, CB_ADDSTRING, 0, name);
			break;
		case 3002://Å»Åð
			SendMessage(hCombo, CB_DELETESTRING, selection, 0);
			break;
		case 2001://ÄÞº¸
			if(HIWORD(wParam)==CBN_SELCHANGE)
				selection = SendMessage(hCombo, CB_SETCURSEL, 0, 0);
			break;
		case 3001:
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