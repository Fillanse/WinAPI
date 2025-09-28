#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include<string>
#include"resource.h"

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE HINST;
CONST INT BSIZE = 256;
TCHAR BUFFER[BSIZE] = {};

void AppendQuality(HWND hwnd, int checkboxId, std::wstring& result, const wchar_t* text) {
    if (IsDlgButtonChecked(hwnd, checkboxId)) {
        result += text;
        result += L"\r\n";
    }
}

void UpdateOutput(HWND hwnd)
{
    HWND hEditStudent = GetDlgItem(hwnd, IDC_STUDENT);
    HWND hOutput = GetDlgItem(hwnd, IDC_EDIT2);

    TCHAR student[BSIZE];
    GetWindowText(hEditStudent, student, BSIZE);

    std::wstring result = L"Student: ";
    result += student;
    result += L"\r\n";

    AppendQuality(hwnd, IDC_ADVANTAGE1, result, L" Hardworking");
    AppendQuality(hwnd, IDC_ADVANTAGE2, result, L" Responsible");
    AppendQuality(hwnd, IDC_ADVANTAGE3, result, L" Clever");

    AppendQuality(hwnd, IDC_DISADVANTAGE1, result, L" Late");
    AppendQuality(hwnd, IDC_DISADVANTAGE2, result, L" Inattentive");
    AppendQuality(hwnd, IDC_DISADVANTAGE3, result, L" Skips classes");

    SetWindowText(hOutput, result.c_str());

}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	HINST = hInst;
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_STUDENT:
			if (HIWORD(wParam) == EN_CHANGE)
				UpdateOutput(hwnd);
			return TRUE;

		case IDC_ADVANTAGE1: case IDC_ADVANTAGE2: case IDC_ADVANTAGE3:
		case IDC_DISADVANTAGE1: case IDC_DISADVANTAGE2:	case IDC_DISADVANTAGE3:
			UpdateOutput(hwnd);
			return TRUE;

		case IDOK:
		case IDCANCEL: EndDialog(hwnd, 0); return TRUE;
		}
		break;

	case WM_CLOSE: EndDialog(hwnd, 0); return TRUE;
	}
	return FALSE;
}