#include<Windows.h>
#include<CommCtrl.h>
#include<stdio.h>

#pragma comment(lib,"comctl32.lib")
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static INITCOMMONCONTROLSEX ic;
	static HWND hlist;
	static HINSTANCE hinst;
	LVCOLUMN lvc;
	static LVITEM lv = { 0 };
	static HWND hedit, hedit2;
	static char name_buff[128] = { 128, };
	static char phone_buff[64] = { 64, };
	static int row = 0;
	static int col = 0;
	NMHDR nh;
	NMLISTVIEW nlv;
	static char temp_buff[256] = { 0 };

	switch (imsg) {
	case WM_CREATE:
		ic.dwICC = ICC_LISTVIEW_CLASSES;
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&ic);
		hinst = ((CREATESTRUCT*)lParam)->hInstance;
		hlist = CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_BORDER, 10,10,400,300, hwnd, 1000, hinst, 0);
		CreateWindow("button", "가입", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 450,150,150,30, hwnd, 2000, hinst, 0);
		//CreateWindow("static", "회원이름", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 100, 90, 30, hwnd, 3000, hinst, 0);
		hedit=CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 450,50,200,30, hwnd, 3001, hinst, 0);
		//CreateWindow("static", "전화번호", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 200, 90, 30, hwnd, 3002, hinst, 0);
		hedit2=CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 450,100,200,30, hwnd, 3003, hinst, 0);
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.cx = 200;
		lvc.cchTextMax = 32;
		lvc.pszText = "이름";
		lvc.iSubItem = 0;
		SendMessage(hlist, LVM_INSERTCOLUMN, 0, &lvc);
		lvc.pszText = "전화번호";
		lvc.iSubItem = 1;
		SendMessage(hlist, LVM_INSERTCOLUMN, 1, &lvc);
		SendMessage(hlist, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				SendMessage(hedit, EM_GETLINE, 0, name_buff);
				lv.mask = LVIF_TEXT;
				lv.cchTextMax = 128;
				lv.iItem = row++;
				lv.iSubItem = col++;
				lv.pszText = name_buff;
				SendMessage(hlist, LVM_INSERTITEM, 0, &lv);
				SendMessage(hedit2, EM_GETLINE, 0, phone_buff);
				lv.iSubItem = col--;
				lv.pszText = phone_buff;
				lv.cchTextMax = 64;
				SendMessage(hlist, LVM_SETITEM, 0, &lv);
				SendMessage(hlist, LVM_UPDATE, row, 0);
				SendMessage(hedit, WM_SETTEXT, 0, "");
				SendMessage(hedit2, WM_SETTEXT, 0, "");
				break;
			}
			break;
		}
		break;
	case WM_NOTIFY:
		nh = *(NMHDR*)lParam;
		switch (nh.idFrom) {
		case 1000:
			switch (nh.code) {
			case LVN_ITEMCHANGED:
				nlv = *(NMLISTVIEW*)lParam;
				lv.iSubItem = 0;
				SendMessage(hlist, LVM_GETITEMTEXT, nlv.iItem, &lv);
				memset(temp_buff, 0, 256);
				strcpy(temp_buff, "선택한 이름: ");
				sprintf(temp_buff + strlen(temp_buff), "%s", lv.pszText);
				SendMessage(hedit, WM_SETTEXT, 0, lv.pszText);
				lv.iSubItem = 1;
				SendMessage(hlist, LVM_GETITEMTEXT, nlv.iItem, &lv);
				sprintf(temp_buff + strlen(temp_buff), " / 선택한 전화번호 : %s", lv.pszText);
				
				SendMessage(hedit2, WM_SETTEXT, 0, lv.pszText);
				InvalidateRgn(hwnd, 0, TRUE);
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 450, 220, temp_buff, strlen(temp_buff));
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}