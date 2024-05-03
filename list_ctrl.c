#include<windows.h>
#include<tchar.h>
#include<CommCtrl.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam);
void makecolumn(HWND hDlg);
void insertdata(HWND hDlg);
int selectitem(HWND hDlg, LPARAM lParam);
void modifyitem(HWND hDlg, int selection);
void deleteitem(HWND hDlg, int selection);

HINSTANCE hinst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCMdLine, int nCmdShow) {
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
	wc.lpszClassName = _T("list");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("list"), _T("list"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void modifyitem(HWND hDlg, int selection) {
	static HWND hList;
	static char name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
	GetDlgItemText(hDlg, IDC_EDIT_PHONE, phone, 20);
	ListView_SetItemText(hList, selection, 0, name);
	ListView_SetItemText(hList, selection, 1, phone);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, "");
	return;
}

void deleteitem(HWND hDlg, int selection) {
	static HWND hList;
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	ListView_DeleteItem(hList, selection);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, "");
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, "");
	return;
}

#define UNSELECTED -1

int selectitem(HWND hDlg, LPARAM lParam) {
	LPNMLISTVIEW nlv;
	HWND hList;
	static char name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	nlv = (LPNMLISTVIEW)lParam;
	ListView_GetItemText(hList, nlv->iItem, 0, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, name);
	ListView_GetItemText(hList, nlv->iItem, 1, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, phone);
	return(nlv->iItem);
}

void insertdata(HWND hDlg) {
	LVITEM item;
	HWND hList;
	int count;
	static char name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);

	GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, "");

	GetDlgItemText(hDlg, IDC_EDIT_PHONE, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, "");

	count = ListView_GetItemCount(hList);
	item.mask = LVIF_TEXT;
	item.iItem = count;
	item.iSubItem = 0;
	item.pszText = name;
	ListView_InsertItem(hList, &item);
	ListView_SetItemText(hList, count, 1, phone);
}

void makecolumn(HWND hDlg) {
	LPSTR name[2] = { "이름","전화번호" };
	LVCOLUMN lvcol = { 0, };
	HWND hList;
	int i;
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for (i = 0; i < 2; i++) {
		lvcol.cx = 90;
		lvcol.iSubItem = i;
		lvcol.pszText = name[i];
		SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)& lvcol);
	}
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	LPNMHDR hdr;
	HWND hList;
	static int selection;

	switch (imsg) {
	case WM_INITDIALOG:
		makecolumn(hDlg);
		selection = UNSELECTED;
		return 1;
	case WM_NOTIFY:
		hdr = (LPNMHDR)lParam;
		hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
		if (hdr->hwndFrom == hList && hdr->code == LVN_ITEMCHANGING) selection = selectitem(hDlg, lParam);

		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_INSERT:
			insertdata(hDlg);
			break;
		case IDC_BUTTON_MODIFY:
			if (selection == UNSELECTED) break;
			modifyitem(hDlg, selection);
			selection = UNSELECTED;
			return 0;
		case IDC_BUTTON_DELETE1:
			if (selection == UNSELECTED) break;
			deleteitem(hDlg, selection);
			selection = UNSELECTED;;
			return 0;
		case IDCLOSE:
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
	switch (imsg) {
	case WM_CREATE:
		
		break;
	case WM_RBUTTONDOWN:
		DialogBox(hinst, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, dialog);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}