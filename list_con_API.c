#include<Windows.h>
#include<tchar.h>
#include <CommCtrl.h>

#pragma comment(lib,"Comctl32.lib")
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

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
	wc.lpszClassName = "listcon";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("listcon", "listcon", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

/*int selectitem(HWND hwnd, HWND list, LPARAM lParam) {
	LPNMLISTVIEW nlv;
	LVITEM item;
	static char name[20], phone[20];
	nlv = (LPNMLISTVIEW)lParam;
	item.
	item.iSubItem = 0;
	SendMessage(list, LVM_GETITEMTEXT, nlv->iItem, &item);
	item.iSubItem = 1;
	SendMessage(list, LVM_GETITEMTEXT, nlv->iItem, &item);
	//ListView_GetItemText(list, nlv->iItem, 0, name, 20);
	//ListView_GetItemText(list, nlv->iItem, 1, phone, 20);
	SetDlgItemText(hwnd, 3001, name);
	SetDlgItemText(hwnd, 3003, phone);
	return(nlv->iItem);
}*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static INITCOMMONCONTROLSEX ctrl;
	static HWND list,edit,edit2;
	int i = 0;
	ctrl.dwICC = ICC_LISTVIEW_CLASSES;
	ctrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
	LPSTR name[2] = { "이름", "전화번호" };
	static char name1[20], phone[20];
	int count;
	LVCOLUMN lvcol = { 0 };
	static LVITEM lvitem = { 0 };
	NMHDR hdr;
	NMLISTVIEW nlv;


	switch (imsg) {
	case WM_CREATE:
		InitCommonControlsEx(&ctrl);
		list = CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_BORDER, 100, 100, 500, 300, hwnd, 1000, hinst, 0);
		CreateWindow("button", "가입", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 620, 150, 100, 30, hwnd, 2000, hinst, 0);
		CreateWindow("static", "회원이름", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 100, 90, 30, hwnd, 3000, hinst, 0);
		edit = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 10, 150, 90, 30, hwnd, 3001, hinst, 0);
		CreateWindow("static", "전화번호", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 200, 90, 30, hwnd, 3002, hinst, 0);
		edit2 = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 10, 250, 90, 30, hwnd, 3003, hinst, 0);
		lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_LEFT;
		for (i = 0; i < 2; i++) {
			lvcol.cx = 90;
			lvcol.iSubItem = i;
			lvcol.pszText = name[i];
			SendMessage(list, LVM_INSERTCOLUMN, i, &lvcol);
		}
		break;
	case WM_NOTIFY:
		hdr = *(NMHDR*)lParam;

		switch (hdr.idFrom) {
		case 1000:
			switch (hdr.code) {
			case LVN_ITEMCHANGED:
				nlv = *(NMLISTVIEW*)lParam;
				lvitem.iSubItem = 0;
				SendMessage(list, LVM_GETITEMTEXT, nlv.iItem, &lvitem);
				SendMessage(edit, WM_SETTEXT, 0, lvitem.pszText);
				lvitem.iSubItem = 1;
				SendMessage(list, LVM_GETITEMTEXT, nlv.iItem, &lvitem);
				SendMessage(edit2, WM_SETTEXT, 0, lvitem.pszText);
				break;
			}
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			GetDlgItemText(hwnd, 3001, name1, 20);
			GetDlgItemText(hwnd, 3003, phone, 20);
			SetDlgItemText(hwnd, 3001, "");
			SetDlgItemText(hwnd, 3003, "");
			count = ListView_GetItemCount(list);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = count;
			lvitem.cchTextMax = 128;
			lvitem.iSubItem = 0;
			lvitem.pszText = name1;
			SendMessage(list, LVM_INSERTITEM, 0, &lvitem);
			lvitem.iSubItem = 1;
			lvitem.cchTextMax = 64;
			lvitem.pszText = phone;
			SendMessage(list, LVM_SETITEM, 0, &lvitem);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}