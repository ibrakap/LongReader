#include <Windows.h>
#include <iostream>

class COpenFile
{
public:
	void SetHwnd(HWND hwnd, HINSTANCE hInstance);
	const char* GetFileName();
private:
	OPENFILENAMEW ofw;
	HWND hWnd;
	HINSTANCE hInstance;
	std::string FileString;
	wchar_t FileNameBuffer[500];
};