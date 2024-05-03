#include<stdio.h>
#include<Windows.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK dialog(HWND hDlg, UINT Imsg, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hinst;
HWND hwnd;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	
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
	wc.lpszClassName = "practice6-10";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice6-10", "practice6-10", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

char people[126][126] = { 0 };
int idx = 0;
//WNDPROC oldproc;

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND combo, edit1, edit2, radio1, radio2;
	int i;
	char temp[20];
	static int male = 0, female = 0;
	static int selection = 0;
	static char name[20] = { 20, }, phone[20] = { 20, }, year[20] = { 0 }, result[126] = { 0 };

	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("static", "회원이름", SS_CENTER | WS_CHILD | WS_VISIBLE, 100, 150, 100, 20, hDlg, 2000, hinst, 0);
		edit1 = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 150, 400, 20, hDlg, 3000, hinst, 0);
		CreateWindow("static", "전화번호", SS_CENTER | WS_CHILD | WS_VISIBLE, 100, 200, 100, 20, hDlg, 2001, hinst, 0);
		edit2 = CreateWindow("edit", "", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 200, 400, 20, hDlg, 3001, hinst, 0);
		CreateWindow("static", "성         별", SS_CENTER | WS_CHILD | WS_VISIBLE, 100, 250, 100, 20, hDlg, 2002, hinst, 0);
		radio1 = CreateWindow("button", "여자", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 230, 250, 100, 20, hDlg, 3002, hinst, 0);
		radio2 = CreateWindow("button", "남자", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 400, 250, 100, 20, hDlg, 3003, hinst, 0);
		CreateWindow("static", "출생연도", SS_CENTER | WS_CHILD | WS_VISIBLE, 100, 300, 100, 20, hDlg, 2003, hinst, 0);
		combo = CreateWindow("combobox", "", CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | WS_VSCROLL, 200, 300, 400, 200, hDlg, 3004, hinst, 0);
		CreateWindow("button", "확인", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 650, 200, 100, 30, hDlg, 4000, hinst, 0);
		CreateWindow("button", "취소", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 650, 300, 100, 30, hDlg, 4001, hinst, 0);
		//oldproc = SetWindowLongPtr(hDlg, GWL_WNDPROC, (LONG)EditProc);

		for (i = 1930; i <= 2020; i++) {
			itoa(i, temp, 10);
			SendMessage(combo, CB_ADDSTRING, 0, temp);
		}

		memset(name, 0, 20);
		memset(phone, 0, 20);
		memset(year, 0, 20);
		name[0] = 20;
		phone[0] = 20;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 3004: //콤보박스
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				selection = SendMessage(combo, CB_GETCURSEL, 0, 0);
				selection += 1930;
			}
			break;
		case 3002: //여자
			if (SendMessage(radio1, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(radio1, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(radio2, BM_SETCHECK, BST_UNCHECKED, 0);
				female = 1;
				male = 0;
			}
			break;
		case 3003: //남자
			if (SendMessage(radio2, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(radio2, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(radio1, BM_SETCHECK, BST_UNCHECKED, 0);
				male = 1;
				female = 0;
				break;
		case 4000: //확인
			SendMessage(edit1, EM_GETLINE, 0, name);
			SendMessage(edit2, EM_GETLINE, 0, phone);
			itoa(selection, year, 10);
			if (male)
				sprintf(people[idx], "이름: %s, 전화번호: %s, 성별: 남자, 출생연도: %s", name, phone, year);
			else
				sprintf(people[idx], "이름: %s, 전화번호: %s, 성별: 여자, 출생연도: %s", name, phone, year);
			idx++;

			InvalidateRgn(hwnd, NULL, TRUE);
		case 4001:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
			}
		}
		return 0;
	}
	return 0;
}

/*LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case 4000:
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	}
	return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
}*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static LPDLGTEMPLATE dlg = 0;
	HGLOBAL hglobal;
	HDC hdc;
	PAINTSTRUCT ps;
	int i,j;

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

		CreateWindow("button", "회원 입력", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 0, 0, 100, 30, hwnd, 1000, hinst, 0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0, j = 1; i<idx; i++, j++)
			TextOut(hdc, 0, 30 * j, people[i], strlen(people[i]));
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1000:
			DialogBoxIndirect(hinst, dlg, hwnd, dialog);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}