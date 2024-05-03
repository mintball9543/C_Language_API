#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
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
	wc.lpszClassName = _T("menu");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	hwnd = CreateWindow(_T("menu"), _T("menu"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT imsg, WPARAM wParam, LPARAM lParam) {
	static HMENU hdle;
	static HMENU file,edit,inform,new;
	static char str1[126];
	int temp;
	int answer;
	static FILE* fp;
	HDC hdc;
	PAINTSTRUCT ps;
	int i = 0;
	static HINSTANCE hinst;
	static OPENFILENAME OFN;
	static TCHAR str[512], lpstrFile[512] = ("");
	static TCHAR filter[]=("Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0");

	switch (imsg) {
	case WM_CREATE:
		hinst = ((CREATESTRUCT*)lParam)->hInstance;
		hdle = CreateMenu();
		file = CreatePopupMenu();
		edit = CreatePopupMenu();
		inform = CreatePopupMenu();
		new = CreatePopupMenu();
		AppendMenu(hdle, MF_POPUP | MF_STRING, file,"����");
		AppendMenu(file, MF_STRING|MF_POPUP, new, "����"); //�� ������Ʈ, �� �ؽ�Ʈ ����, ���� ����
		AppendMenu(new, MF_STRING, 1010, "�� ������Ʈ");
		AppendMenu(new, MF_STRING, 1011, "�� �ؽ�Ʈ ����");
		AppendMenu(new, MF_STRING, 1012, "���� ����");

		AppendMenu(file, MF_STRING, 1001, "����");
		AppendMenu(file, MF_STRING|MF_GRAYED, 1002, "����");
		AppendMenu(file, MF_SEPARATOR, 0000, NULL);
		AppendMenu(file, MF_STRING, 1003, "������");

		AppendMenu(hdle, MF_POPUP | MF_STRING, edit, "����");
		AppendMenu(edit, MF_STRING, 2000, "����ϱ�");
		AppendMenu(edit, MF_STRING, 2001, "�����ϱ�");
		AppendMenu(edit, MF_STRING, 2002, "�ٿ��ֱ�");

		AppendMenu(hdle, MF_POPUP | MF_STRING, inform, "����");
		AppendMenu(inform, MF_STRING, 3000, "���α׷� ����");
		SetMenu(hwnd, hdle);
	   
		memset(str1, 0, 126);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 100, 100, str1, _tcslen(str1));
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1000:
			MessageBox(hwnd, ("�� ������ ���ڽ��ϱ�?"), ("�� ���� ����"), MB_OKCANCEL);
			break;
		case 1001:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 512;
			OFN.lpstrInitialDir = (".");
			if (GetOpenFileName(&OFN) != 0) {
				sprintf(str, ("%s ������ ���ڽ��ϱ�?"), OFN.lpstrFile);
				MessageBox(hwnd, str, ("���� ����"), MB_OK);
			}
			fp = fopen(OFN.lpstrFile, "r+");
			//temp = fgetc(fp);
			/*for (i = 0; feof(fp) == 0; i++) {
				str[i] = (char)temp;
				temp = fgetc(fp);
			}*/
			fscanf(fp, "%s", str1);
			fclose(fp);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 1003:
			answer=MessageBox(hwnd, _T("���α׷��� �����ðڽ��ϱ�?"), _T("������"), MB_YESNO);
			if (answer == IDYES) {
				PostQuitMessage(0);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, imsg, wParam, lParam);
}