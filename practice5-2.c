#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
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
	wc.lpszClassName = "practice5-2";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice5-2", "practice5-2", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK dialog(HWND hDlg, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HWND book, music, internet, female, male, edit;
	static int bookch=0;
	static int musicch=0;
	static int internetch=0;
	static int femalech=0;
	static int malech=0;
	static char hobby[126] = { 0, };
	static char sex[126] = { 0, };

	switch (imsg) {
	case WM_INITDIALOG:
		CreateWindow("static", "취미를 선택하세요", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 100, 150, 30, hDlg, 1000, hinst, 0);
		CreateWindow("static", "성별을 선택하세요", SS_LEFT | WS_CHILD | WS_VISIBLE, 100, 250, 150, 30, hDlg, 1001, hinst, 0);
		book=CreateWindow("button", "독서", BS_CHECKBOX | WS_CHILD | WS_VISIBLE, 110, 130, 50, 30, hDlg, 2000, hinst, 0);
		music=CreateWindow("button", "음악감상", BS_CHECKBOX | WS_CHILD | WS_VISIBLE, 200, 130, 100, 30, hDlg, 2001, hinst, 0);
		internet=CreateWindow("button", "인터넷", BS_CHECKBOX | WS_CHILD | WS_VISIBLE, 300, 130, 100, 30, hDlg, 2002, hinst, 0);
		female=CreateWindow("button", "여성", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 110, 300, 50, 30, hDlg, 2003, hinst, 0);
		male=CreateWindow("button", "남성", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 250, 300, 50, 30, hDlg, 2004, hinst, 0);
		CreateWindow("button", "출력하기", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 400, 250, 100, 30, hDlg, 3000, hinst, 0);
		CreateWindow("button", "종료하기", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 400, 300, 100, 30, hDlg, 3001, hinst, 0);
		edit=CreateWindow("edit", "", ES_LEFT | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 330, 400, 200, hDlg, 4000, hinst, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2000:
			if (SendMessage(book, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(book, BM_SETCHECK, BST_CHECKED, 0);
				bookch = 1;
			}
			else if (SendMessage(book, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendMessage(book, BM_SETCHECK, BST_UNCHECKED, 0);
				bookch = 0;
			}
			break;
		case 2001:
			if (SendMessage(music, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(music, BM_SETCHECK, BST_CHECKED, 0);
				musicch = 1;
			}
			else if (SendMessage(music, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendMessage(music, BM_SETCHECK, BST_UNCHECKED, 0);
				musicch = 0;
			}
			break;
		case 2002:
			if (SendMessage(internet, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(internet, BM_SETCHECK, BST_CHECKED, 0);
				internetch = 1;
			}
			else if (SendMessage(internet, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendMessage(internet, BM_SETCHECK, BST_UNCHECKED, 0);
				internetch = 0;
			}
			break;

		case 2003:
			if (SendMessage(female, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(female, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(male, BM_SETCHECK, BST_UNCHECKED, 0);
				femalech = 1;
				malech = 0;
			}
			break;

		case 2004:
			if (SendMessage(male, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
				SendMessage(male, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(female, BM_SETCHECK, BST_UNCHECKED, 0);
				malech = 1;
				femalech = 0;
			}
			break;

		case 3000:
			sprintf(hobby, "선택한 취미는 ");
			if (bookch == 1)
				sprintf(hobby+strlen(hobby), "독서");
			
			if (bookch == 1 && musicch == 1)
				sprintf(hobby + strlen(hobby), ", 음악감상");
			else if (musicch == 1)
				sprintf(hobby + strlen(hobby), "음악감상");

			if (musicch == 1 && internetch == 1)
				sprintf(hobby + strlen(hobby), ", 인터넷");
			else if (bookch == 1 && internetch == 1)
				sprintf(hobby + strlen(hobby), ", 인터넷");
			else if (internetch == 1)
				sprintf(hobby + strlen(hobby), "인터넷");

			sprintf(hobby + strlen(hobby), " 입니다");


			sprintf(sex, "\n선택한 성별은 ");
			if (malech == 1)
				sprintf(sex + strlen(sex), "남자 입니다.");
			else 
				sprintf(sex + strlen(sex), "여자 입니다.");

			sprintf(hobby + strlen(hobby), "%s", sex);

			SendMessage(edit, WM_SETTEXT, 0, hobby);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static LPDLGTEMPLATE dlg=0;
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