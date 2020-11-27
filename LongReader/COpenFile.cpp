#include "COpenFile.h"
#include "WinHelper.h"

#include <iostream>

void COpenFile::SetHwnd(HWND hwnd, HINSTANCE hinstance)
{
	hWnd = hwnd;
	hInstance = hinstance;
}

const char* COpenFile::GetFileName()
{
	char Buffer[(260*2)];
	std::wstring filename(MAX_PATH, L'\0');
	ofw.lStructSize = sizeof(OPENFILENAMEW);
	ofw.hwndOwner = hWnd;
	ofw.hInstance = hInstance;
	ofw.lpstrFile = &filename[0];
	ofw.nMaxFile  = 500;
	GetOpenFileNameW(&ofw);

	wcstombs(Buffer, &filename[0], (260 * 2));
	FileString = Buffer;
	return FileString.c_str();
}
