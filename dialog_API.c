#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	LPDLGTEMPLATE dlg=0;
	HGLOBAL hglobal;
	hinst = hInstance;
	hglobal=GlobalAlloc(GMEM_ZEROINIT, 2048);
	dlg = (LPDLGTEMPLATE)GlobalLock(hglobal);
	dlg->style = WS_POPUP | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
	dlg->cx = 600;
	dlg->cy = 300;
	dlg->x = 100;
	dlg->y = 100;
	dlg->dwExtendedStyle = 0;
	dlg->cdit = 0;
	DialogBoxIndirect(hInstance, dlg, NULL, dialog);


	return 0;
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static char temp[126];
	static HWND hedit, check, male, female;
	const char buff[256] = { "256" };
	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("button", "ok", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 1000, 540, 100, 50, hDlg, 1000, hinst, 0);
		CreateWindow("static", "ȸ���̸�", SS_LEFT | WS_CHILD | WS_VISIBLE, 250, 100, 70, 50, hDlg, 2000, hinst, 0);
		hedit = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE, 230, 130, 100, 30, hDlg, 3000, hinst, 0);
		CreateWindow("button", "����", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 230, 230, 100, 30, hDlg, 4000, hinst, 0);
		CreateWindow("button", "Ż��", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 400, 230, 100, 30, hDlg, 4001, hinst, 0);
		CreateWindow("button", "����", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 230, 300, 100, 30, hDlg, 4002, hinst, 0);
		check = CreateWindow("button", "test", BS_CHECKBOX | WS_CHILD | WS_VISIBLE, 500, 100, 100, 30, hDlg, 5000, hinst, 0);
		male=CreateWindow("button", "��", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 600, 100, 50, 50, hDlg, 6000, hinst, 0);
		female=CreateWindow("button", "��", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 650, 100, 50, 50, hDlg, 6001, hinst, 0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case 1000:
				//MessageBox(hDlg, "cheking ok button", "ok", MB_OK);
				//SetWindowText(hedit, "test");
				SendMessage(hedit, EM_GETLINE, 0, buff);
				MessageBox(0, buff, "test", MB_OK);
				break;
			case 4000:
				//GetWindowText(hedit,temp, 126);
				//memset(buff, 0, 256);
				SendMessage(hedit, EM_GETLINE, 0, buff);
				sprintf(buff, "%s���� ���ԵǾ����ϴ�.", buff);
				MessageBox(0, buff, "����", MB_OK);
				break;
			case 4001:
				//GetWindowText(hedit, temp, 126);
				//memset(buff, 0, 256);
				SendMessage(hedit, EM_GETLINE, 0, buff);
				sprintf(buff, "%s���� Ż��Ǿ����ϴ�.", buff);
				MessageBox(0, buff, "Ż��", MB_OK);
				break;
			case 4002:
				EndDialog(hDlg, 0);
				break;
			case 5000:
				if (SendMessage(check, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
					SendMessage(check, BM_SETCHECK, BST_CHECKED, 0);
				else if (SendMessage(check, BM_GETCHECK, 0, 0) == BST_CHECKED)
					SendMessage(check, BM_SETCHECK, BST_UNCHECKED, 0);
				break;
			case 6000:
				if (SendMessage(male, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
					SendMessage(male, BM_SETCHECK, BST_CHECKED, 0);
					SendMessage(female, BM_SETCHECK, BST_UNCHECKED, 0);
					MessageBox(hDlg, "���ڸ� �����߽��ϴ�.", "����", MB_OK);
				}
				break;
			case 6001:
				if (SendMessage(female, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
					SendMessage(female, BM_SETCHECK, BST_CHECKED, 0);
					SendMessage(male, BM_SETCHECK, BST_UNCHECKED, 0);
					MessageBox(hDlg, "���ڸ� �����߽��ϴ�.", "����", MB_OK);
				}
				break;
			case IDCANCEL:
				EndDialog(hDlg, 0);
				break;
			}
			break;
		case WM_PAINT:
			hdc=BeginPaint(hDlg, &ps);
			EndPaint(hDlg, &ps);
	}
	return 0;
}