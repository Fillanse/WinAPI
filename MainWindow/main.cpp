#include <Windows.h>
#include <winuser.h>
#include <iostream>
#include "resource.h"

CONST WCHAR szWNDCLASSNAME[] = L"My Window Class";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void UpdateWindowTitleWithPosSize(HWND hwnd)
{
	RECT window;
	WCHAR title[256];

	if (!GetWindowRect(hwnd, &window)) return;

	swprintf_s(
		title, L"MainWindow - Pos: %d,%d Size: %d x %d",
		window.left, window.top,
		window.right - window.left,
		window.bottom - window.top);

	SetWindowText(hwnd, title);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	//wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIcon = (HICON)LoadImage(NULL, L"astro.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(NULL, L"atom.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CURSOR1));
	wClass.hCursor = (HCURSOR)LoadImage(NULL, L"Protoss.cur", IMAGE_CURSOR, 24, 24, LR_LOADFROMFILE);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = (LPCWSTR)szWNDCLASSNAME;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wClass)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);
	int winW = int(screenW * 0.75);
	int winH = int(screenH * 0.75);
	int winX = (screenW - winW) / 2;
	int winY = (screenH - winH) / 2;

	HWND hwnd = CreateWindowEx(
		NULL,
		(LPCWSTR)szWNDCLASSNAME,
		(LPCWSTR)szWNDCLASSNAME,
		WS_OVERLAPPEDWINDOW,
		winX, winY, winW, winH,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	UpdateWindowTitleWithPosSize(hwnd);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStatic = CreateWindowEx(
			NULL,
			L"STATIC",
			L"This static-text as created via CreateWindowEx function",
			WS_CHILD | WS_VISIBLE,
			10, 10, 300, 20,
			hwnd,
			(HMENU)1,
			(HINSTANCE)GetModuleHandle(NULL),
			NULL);
		HWND hEdit = CreateWindowEx(
			NULL,
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			10, 40, 300, 20,
			hwnd,
			(HMENU)2,
			(HINSTANCE)GetModuleHandle(NULL),
			NULL);
		HWND hButton = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"Apply",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			320, 40, 100, 20,
			hwnd,
			(HMENU)3,
			(HINSTANCE)GetModuleHandle(NULL),
			NULL);
	}
	break;
	case WM_COMMAND: {
		HWND hEdit = GetDlgItem(hwnd, 2);
		CONST INT SIZE = 256;
		WCHAR buffer[SIZE];
		switch (LOWORD(wParam))
	case 3: {
			GetWindowText(hEdit, buffer, SIZE);
			MessageBox(hwnd, buffer, L"Information", MB_OK | MB_ICONINFORMATION);
		}
		break;

		break;
	}
	case WM_MOVE: UpdateWindowTitleWithPosSize(hwnd); break;
	case WM_SIZE: UpdateWindowTitleWithPosSize(hwnd); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Are you sure you want to exit?", L"Exit", MB_ICONQUESTION | MB_YESNO) == IDYES)
			DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
