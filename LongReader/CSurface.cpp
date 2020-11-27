#include "CSurface.h"

void CSurface::BeginDraw()
{
	hDc = BeginPaint(hWnd, &ps);
}

void CSurface::EndDraw()
{
	EndPaint(hWnd, &ps);
}

void CSurface::ClearSurface()
{
	FillRect(hDc, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
}

void CSurface::DrawTextOnSurface(const char* buffer)
{
	rect.left = 0;
	rect.top = 0;
	// TextOutA(hDc, 0, 0, buffer, strlen(buffer));
	DrawTextA(hDc, buffer, -1, &rect, DT_NOCLIP);
}

void CSurface::SetHwnd(HWND hwnd)
{
	hWnd = hwnd;
}
