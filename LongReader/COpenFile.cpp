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
	char Buffer[500];
	ofw.lStructSize = sizeof(OPENFILENAMEW);
	ofw.hwndOwner = hWnd;
	ofw.hInstance = hInstance;
	ofw.lpstrFile = FileNameBuffer;
	ofw.nMaxFile  = 500;
	GetOpenFileNameW(&ofw);

	wcstombs(Buffer, FileNameBuffer, 500);
	FileString = Buffer;
	return FileString.c_str();
}
