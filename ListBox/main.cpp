#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* g_ITEMS[] = { "This", "is", "my", "first", "List", "Box" };

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{		
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc, 0);
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hListBox = GetDlgItem(hwnd, IDC_LISTBOX);
		for (int i = 0; i < sizeof(g_ITEMS) / sizeof(g_ITEMS[0]); i++)
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_ITEMS[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LISTBOX:
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT), hwnd, DlgProcEdit, 0);
			}
			break;
		case IDC_ADD:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT), hwnd, DlgProcAdd, 0);
			break;
		case IDC_OK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			CHAR sz_message[SIZE] = {};
			HWND hListBox = GetDlgItem(hwnd, IDC_LISTBOX);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, (LPARAM)hListBox);
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "You selected item #%i with value \"%s\"", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDC_CANCEL:
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_INPUT));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
		{
			HWND hEditItem = GetDlgItem(hwnd, IDC_EDIT_INPUT);
			CHAR sz_buffer[FILENAME_MAX] = {};
			SendMessage(hEditItem, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			HWND hListBox = GetDlgItem(GetParent(hwnd), IDC_LISTBOX);
			if (SendMessage(hListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			else
			{
				MessageBox(hwnd, "This item already exists in the list, please enter another value", "Warning", MB_OK | MB_ICONINFORMATION);
				break;
			}
		}
		case IDC_CANCEL:
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
INT_PTR CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Edit item");
		CHAR sz_buffer[FILENAME_MAX] = {};
		HWND hListBox = GetDlgItem(GetParent(hwnd), IDC_LISTBOX);
		HWND hEditItem = GetDlgItem(hwnd, IDC_EDIT_INPUT);
		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEditItem, WM_SETTEXT, 0, (LPARAM)sz_buffer);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CANCEL:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}