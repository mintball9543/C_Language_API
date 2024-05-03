#include <Windows.h>
#include<tchar.h>
#include<stdio.h>
#include"resource.h"

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
	wc.lpszClassName = _T("file");
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd=CreateWindow(_T("file"), _T("file"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void OutFromFile(TCHAR filename[], HWND hwnd) {
	FILE* fptr;
	HDC hdc;
	int line;
	TCHAR buffer[500];
	line = 0;
	hdc = GetDC(hwnd);
	_tfopen_s(&fptr, filename, _T("r"));
	while (_fgetts(buffer, 100, fptr) != NULL) {
		if (buffer[_tcslen(buffer) - 1]==_T('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
		line++;
	}
	fclose(fptr);
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME ofn;
	OPENFILENAME sfn;
	TCHAR lpstrFile[100] = _T("");
	TCHAR filter[] = "Every File(*.*) \0*.*\0"
		"Text File\0*.txt;*.doc\0";
	static TCHAR str[100];
	static TCHAR filename[100];
	static int count;
	HDC hdc;
	PAINTSTRUCT ps;
	FILE* fp;
	CHOOSEFONT FONT;
	static COLORREF fcolor;
	HFONT hFont, OldFont;
	static LOGFONT LogFont;
	
	switch (imsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hFont = CreateFontIndirect(&LogFont);
		OldFont = (HFONT)SelectObject(hdc, hFont);
		SetTextColor(hdc, fcolor);
		TextOut(hdc, 0, 0, str, _tcslen(str));
		SelectObject(hdc, OldFont);
		DeleteObject(hFont);
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK && count > 0) {
			count--;
			str[count] = NULL;
		}
		else if (wParam == VK_RETURN) {
			str[count++] = '\n';
			str[count] = NULL;
		}
		else {
			str[count++] = wParam;
			str[count] = NULL;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILEOPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = 256;
			ofn.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&ofn) != 0)
				OutFromFile(ofn.lpstrFile, hwnd);
			break;
		case ID_FILESAVE:
			memset(&sfn, 0, sizeof(OPENFILENAME));
			sfn.lStructSize = sizeof(OPENFILENAME);
			sfn.hwndOwner = hwnd;
			sfn.lpstrFilter = filter;
			sfn.lpstrFile = lpstrFile;
			sfn.nMaxFile = 256;
			sfn.lpstrInitialDir = ".";
			if (GetSaveFileName(&sfn) != 0) {
				sprintf(filename, "%s 파일로 저장하겠습니까?", sfn.lpstrFile);
				MessageBox(hwnd, filename, "저장하기 선택", MB_OK);
			}
			fp = fopen(sfn.lpstrFile, "w+");
			fprintf(fp, str);
			fclose(fp);
			break;
		case ID_FONTDLG:
			memset(&FONT, 0, sizeof(CHOOSEFONT));
			FONT.lStructSize = sizeof(CHOOSEFONT);
			FONT.hwndOwner = hwnd;
			FONT.lpLogFont = &LogFont;
			FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
			if (ChooseFont(&FONT) != 0) {
				fcolor = FONT.rgbColors;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}