#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<math.h>
#include <algorithm>
#include <stdio.h> 
#include"resource.h"

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CONST INT BSIZE = 256;
WCHAR BUFFER[BSIZE];
FLOAT LEFT;
FLOAT RIGHT;

FLOAT division(FLOAT left, FLOAT right) { return left / right; }
FLOAT multiplication(FLOAT left, FLOAT right) { return left * right; }
FLOAT subtraction(FLOAT left, FLOAT right) { return left - right; }
FLOAT addition(FLOAT left, FLOAT right) { return left + right; }
FLOAT percent(FLOAT right) { return right / 100; }
FLOAT fraction(FLOAT left) { return left != 0 ? 1 / left : 0; }
FLOAT power(FLOAT left) { return left * left; }
FLOAT root(FLOAT left) { return sqrt(left); }
FLOAT reverse(FLOAT left) { return -left; }

WCHAR GetOperation(HWND hEditOutput) {
	GetWindowText(hEditOutput, BUFFER, BSIZE);
	const WCHAR* del = L"/*-+";
	WCHAR* opPtr = wcspbrk(BUFFER, del);
	if (opPtr != NULL) {
		WCHAR op_char = *opPtr;
		*opPtr = L'\0';

		LEFT = _wtof(BUFFER);
		RIGHT = _wtof(opPtr + 1);
		*opPtr = op_char;
		return op_char;
	}
	return L'\0';
}

void UpdateResult(HWND hwnd, WCHAR symbol) {
	HWND hEditOutput = GetDlgItem(hwnd, IDC_RESULT);
	GetWindowText(hEditOutput, BUFFER, BSIZE);
	INT len = lstrlen(BUFFER);
	if (len < BSIZE - 1) {
		BUFFER[len] = symbol;
		BUFFER[len + 1] = L'\0';
		SetWindowText(hEditOutput, BUFFER);
	}
}

void UpdateHistory(HWND hwnd, LPCWSTR expression) {
	HWND hEditHistory = GetDlgItem(hwnd, IDC_HISTORY);
	SetWindowText(hEditHistory, expression);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_CALCULATOR), NULL, DlgProc, NULL);
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HWND hEditResult = GetDlgItem(hwnd, IDC_RESULT);
	HWND hEditHistory = GetDlgItem(hwnd, IDC_HISTORY);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hEditResult, L"");
		SetWindowText(hEditHistory, L"");
		BUFFER[0] = L'\0';
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ZERO: UpdateResult(hwnd, L'0'); break;
		case IDC_ONE: UpdateResult(hwnd, L'1'); break;
		case IDC_TWO: UpdateResult(hwnd, L'2'); break;
		case IDC_THREE: UpdateResult(hwnd, L'3'); break;
		case IDC_FOUR: UpdateResult(hwnd, L'4'); break;
		case IDC_FIVE: UpdateResult(hwnd, L'5'); break;
		case IDC_SIX: UpdateResult(hwnd, L'6'); break;
		case IDC_SEVEN: UpdateResult(hwnd, L'7'); break;
		case IDC_EIGHT: UpdateResult(hwnd, L'8'); break;
		case IDC_NINE: UpdateResult(hwnd, L'9'); break;
		case IDC_DIVISION: UpdateResult(hwnd, L'/'); break;
		case IDC_MULTIPLICATION: UpdateResult(hwnd, L'*'); break;
		case IDC_SUBSTRACTION: UpdateResult(hwnd, L'-'); break;
		case IDC_ADDITION: UpdateResult(hwnd, L'+'); break;
		case IDC_REVERSE: {
			WCHAR op = GetOperation(hEditResult);
			if (op == L'\0') {
				LEFT = _wtof(BUFFER);
				LEFT = reverse(LEFT);
				_snwprintf(BUFFER, BSIZE, L"%g", LEFT);
				SetWindowText(hEditResult, BUFFER);
			}
			SetWindowText(hEditResult, BUFFER);
			break;
		}

		case IDC_DOT: if (wcschr(BUFFER, L'.') == NULL) { UpdateResult(hwnd, L'.'); } break;

		case IDC_CE:
			SetWindowText(hEditResult, L"");
			break;

		case IDC_C:
			SetWindowText(hEditResult, L"");
			SetWindowText(hEditHistory, L"");
			LEFT = 0;
			RIGHT = 0;
			BUFFER[0] = L'\0';
			break;

		case IDC_PERCENT: {
			WCHAR op = GetOperation(hEditResult);
			if (op != L'\0') {
				FLOAT result = 0;
				switch (op)
				{
				case L'/':
					if (percent(RIGHT) == 0) {
						MessageBox(hwnd, L"Division by zero", L"Error", MB_ICONERROR);
						result = 0;
					}
					else {
						result = division(LEFT, percent(RIGHT));
					}
					break;
				case L'*': result = multiplication(LEFT, percent(RIGHT)); break;
				case L'-': result = subtraction(LEFT, LEFT * percent(RIGHT)); break;
				case L'+': result = addition(LEFT, LEFT * percent(RIGHT)); break;
				default:
					break;
				}
				UpdateHistory(hwnd, BUFFER);
				_snwprintf(BUFFER, BSIZE, L"%g", result);
				SetWindowText(hEditResult, BUFFER);
			}
			break;
		}

		case IDC_EQUALS: {
			WCHAR op = GetOperation(hEditResult);
			if (op != L'\0') {
				FLOAT result = 0;
				switch (op) {
				case L'/':
					if (RIGHT == 0) {
						MessageBox(hwnd, L"Division by zero", L"Error", MB_ICONERROR);
						result = 0;
					}
					else {
						result = division(LEFT, RIGHT);
					}
					break;
				case L'*':
					result = multiplication(LEFT, RIGHT);
					break;
				case L'-':
					result = subtraction(LEFT, RIGHT);
					break;
				case L'+':
					result = addition(LEFT, RIGHT);
					break;
				default:
					break;
				}

				UpdateHistory(hwnd, BUFFER);
				_snwprintf(BUFFER, BSIZE, L"%g", result);
				SetWindowText(hEditResult, BUFFER);
			}
			break;
		}
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
