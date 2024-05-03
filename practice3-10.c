#include<Windows.h>
#include<tchar.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;
	HACCEL hAcc;
	ACCEL accel[4];

	//hAcc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "practice3-10";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow("practice3-10", "practice3-10", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	accel[0].cmd = 3000;
	accel[0].fVirt = FCONTROL;
	accel[0].key = 'N';
	accel[1].cmd = 3001;
	accel[1].fVirt = FCONTROL;
	accel[1].key = 'O';
	accel[2].cmd = 3002;
	accel[2].fVirt = FCONTROL;
	accel[2].key = 'S';
	accel[3].cmd = 3003;
	accel[3].fVirt = FCONTROL;
	accel[3].key = 'E';
	hAcc=CreateAcceleratorTable(accel, 4);
	//LoadAccelerators(hInstance, hAcc);
	while (GetMessage(&msg, 0, 0, 0)) {
		if (!TranslateAccelerator(hwnd, hAcc, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
	struct Rect1 {
		int x1;
		int y1;
		int x2;
		int y2;
		LOGFONT *LogFont;
		COLORREF color;
	};
LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	HMENU hmenu, text, file;
	HDC hdc;
	PAINTSTRUCT ps;
	static int textck,i,idx,count,flag = 0;
	static char str[126][126] = { 0 };
	RECT rt;
	CHOOSEFONT FONT;
	HFONT hfont, oldfont;
	

	static struct Rect1 rect[126];

	switch (imsg) {
	case WM_CREATE:
		hmenu = CreateMenu();
		text = CreatePopupMenu();
		file = CreatePopupMenu();
		AppendMenu(hmenu, MF_STRING | MF_POPUP, text, _T("텍스트 출력"));
		AppendMenu(text, MF_STRING, 4000, _T("폰트 선택"));
		AppendMenu(text, MF_STRING, 4001, _T("글쓰기"));

		AppendMenu(hmenu, MF_STRING | MF_POPUP, file, _T("파일"));
		AppendMenu(file, MF_STRING, 3000, _T("새글\tCtrl+N"));
		AppendMenu(file, MF_STRING, 3001, _T("열기\tCtrl+O"));
		AppendMenu(file, MF_STRING, 3002, _T("저장\tCtrl+S"));
		AppendMenu(file, MF_STRING, 3003, _T("끝내기\tCtrl+E"));
		SetMenu(hwnd, hmenu);
		/*for (i = 0; i < 126; i++)
			str[i] = malloc(sizeof(char) * 126);*/
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (i = 0; rect[i].x1 != NULL; i++) {
			Rectangle(hdc, rect[i].x1, rect[i].y1, rect[i].x2, rect[i].y2);
			rt.left = rect[i].x1;
			rt.top = rect[i].y1;
			rt.right = rect[i].x2;
			rt.bottom = rect[i].y2;
			if ((rect[i].color != NULL) || (rect[i].LogFont != NULL)) {
				hfont = CreateFontIndirect(&(rect[i].LogFont));
				oldfont = (HFONT)SelectObject(hdc, hfont);
				SetTextColor(hdc, rect[i].color);
				DrawText(hdc, str[i], _tcslen(str[i]), &rt, DT_TOP | DT_LEFT);
				SelectObject(hdc, oldfont);
				DeleteObject(hfont);
			}
			else {
				SetTextColor(hdc, RGB(0, 0, 0));
				DrawText(hdc, str[i], _tcslen(str[i]), &rt, DT_TOP | DT_LEFT);
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK) {
			str[idx][count--] = NULL;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else {
			str[idx][count++] = wParam;
			str[idx][count] = NULL;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 4000: //글꼴
			memset(&FONT, 0, sizeof(CHOOSEFONT));
			FONT.lStructSize = sizeof(CHOOSEFONT);
			FONT.hwndOwner = hwnd;
			FONT.lpLogFont = &(rect[idx].LogFont);
			FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
			if (ChooseFont(&FONT) != 0) {
				rect[idx].color = FONT.rgbColors;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case 4001: //글쓰기상자
			textck = 1;
			if (flag) {
				idx++;
			}
			flag = 1;
			count = 0;
			break;
		case 3000:
			MessageBox(hwnd, "새글", "새글", MB_OK);
			break;
		case 3001:
			MessageBox(hwnd, "열기", "열기", MB_OK);
			break;
		case 3002:
			MessageBox(hwnd, "저장하기", "저장하기", MB_OK);
			break;
		case 3003:
			MessageBox(hwnd, "끝내기", "끝내기", MB_OK);
			break;
		}
	case WM_LBUTTONDOWN:
		if (textck) {
			rect[idx].x1 = LOWORD(lParam);
			rect[idx].y1 = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		if (textck) {
			rect[idx].x2 = LOWORD(lParam);
			rect[idx].y2 = HIWORD(lParam);
			textck = 0;
			//idx++;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hwnd, imsg, wParam, lParam);
}