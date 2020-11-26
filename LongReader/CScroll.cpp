#include "CScroll.h"

void CScroll::CreateScroll(int RangeMin, int RangeMax, unsigned int PageSize)
{
    si.cbSize   = sizeof(SCROLLINFO);
    si.fMask    = SIF_RANGE | SIF_PAGE;
    si.nMin     = RangeMin;
    si.nMax     = RangeMax;
    si.nPage    = PageSize;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

void CScroll::SetScrollPos(int PagePos)
{
    si.fMask    = SIF_POS;
    si.nPos     = PagePos;
    SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}

void CScroll::SetHwnd(HWND hwnd)
{
    hWnd = hwnd;
}

void CScroll::GetScrollInfo()
{
    si.cbSize   = sizeof(SCROLLINFO);
    si.fMask    = SIF_ALL;
    ::GetScrollInfo(hWnd, SB_VERT, &si);
    pos = si.nPos;
    min = si.nMin;
}
