#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include "resource.h"

INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK InputDlgProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE HINST;
CONST INT BSIZE = 256;
TCHAR BUFFER[BSIZE] = {};

bool DialogInput(HWND hwnd, LPCWSTR initialText = L"")
{
	lstrcpyn(BUFFER, initialText, BSIZE);
	return DialogBoxParam(HINST, MAKEINTRESOURCE(IDD_INPUT), hwnd, InputDlgProc, 0) == IDOK;
}
int CheckSelected(HWND hList)
{
	int selected = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
	if (selected == LB_ERR) return 0;
	return selected;
}
void EditSelected(HWND hList, HWND hwnd)
{
	int selected = CheckSelected(hList);

	SendMessage(hList, LB_GETTEXT, selected, (LPARAM)BUFFER);
	if (DialogInput(hwnd, BUFFER))
	{
		SendMessage(hList, LB_DELETESTRING, selected, 0);
		SendMessage(hList, LB_INSERTSTRING, selected, (LPARAM)BUFFER);
	}
}


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	HINST = hInst;
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc, 0);
	return 0;
}


INT_PTR CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: SetDlgItemText(hwnd, IDC_EDIT_INPUT, BUFFER); return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK: GetDlgItemText(hwnd, IDC_EDIT_INPUT, BUFFER, BSIZE); EndDialog(hwnd, IDOK); return TRUE;

		case IDC_CANCEL: EndDialog(hwnd, IDCANCEL); return TRUE;
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK MainDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM)
{
	HWND hList = GetDlgItem(hwnd, IDC_LISTBOX);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LISTBOX: if (HIWORD(wParam) == LBN_DBLCLK) EditSelected(hList, hwnd); /*goto EDIT;*/ break;

		case IDC_ADD: if (DialogInput(hwnd)) SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)BUFFER); return TRUE;

		case IDC_EDIT: EditSelected(hList, hwnd); return TRUE;
			//EDIT:
				//int selected = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
				//if (selected != LB_ERR)
				//{
				//	SendMessage(hList, LB_GETTEXT, selected, (LPARAM)BUFFER);
				//	if (DialogBoxParam(HINST, MAKEINTRESOURCE(IDD_INPUT), hwnd, InputDlgProc, 0) == IDOK)
				//	{
				//		SendMessage(hList, LB_DELETESTRING, selected, 0);
				//		SendMessage(hList, LB_INSERTSTRING, selected, (LPARAM)BUFFER);
				//	}
				//}
		case IDC_DELETE: SendMessage(hList, LB_DELETESTRING, CheckSelected(hList), 0); return TRUE;
		

		case IDOK:
		
			SendMessage(hList, LB_GETTEXT, CheckSelected(hList), (LPARAM)BUFFER);

			TCHAR msg[BSIZE];
			wsprintf(msg, L"You chose: %s", BUFFER);
			MessageBox(hwnd, msg, L"Info", MB_OK | MB_ICONINFORMATION);

			return TRUE;
		

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;

	}
	return FALSE;
}
