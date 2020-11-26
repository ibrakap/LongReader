#pragma once
#include <Windows.h>

class CScroll
{
public:
	int pos;
	int min;
	
	void CreateScroll(int RangeMin, int RangeMax, unsigned int PageSize);
	void SetScrollPos(int PagePos);

	void SetHwnd(HWND hwnd);
	void GetScrollInfo();
private:
	SCROLLINFO si;
	HWND hWnd;
};
