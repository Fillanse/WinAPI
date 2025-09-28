#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<commctrl.h>
#include"resource.h"

#pragma comment(lib, "comctl32.lib")

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, NULL);

	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:		
	{
		InitCommonControls();
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
		//Edit_SetCueBannerText(hEditLogin, (LPARAM)"Login");
		//Edit_SetCueBannerText(hEditPassword, (LPARAM)"Password");
		SendMessage(hEditLogin, EM_SETCUEBANNER, 0, (LPARAM)"Login");
		
		//SetFocus(hEditLogin);
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
	break;
	case WM_COMMAND:		
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDOK:		MessageBox(hwnd, "OK button was pressed", "Info", MB_OK | MB_ICONINFORMATION);	break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:		
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}