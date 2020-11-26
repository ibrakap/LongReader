#include <iostream>
#include "WinFile.h"
#include <Windows.h>
#include "WinHelper.h"
#include "resource.h"

#include "CSurface.h"

LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
#define ID_EDITCHILD 99888

SCROLLINFO si;
HINSTANCE gInstance;
File f;
char* Buffer = nullptr;
DWORD FP;
OPENFILENAMEW opf;
wchar_t FileNameBuf[500];

CSurface Surface;
int REQ;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    gInstance = hInstance;
    WNDCLASSEXW wcex;
    MSG sMsg;
    RegisterMyWindowClass(wcex, WindowProc, hInstance);

    DWORD ExWindowStyles    = (WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TOPMOST);
    DWORD WindowStyles      = (WS_OVERLAPPEDWINDOW | WS_VSCROLL);
    HWND  hWnd              = CreateMyWindow(WindowStyles, ExWindowStyles, hInstance, 500, 500);
    return LoopMyMessageLoop(sMsg, hWnd);
    
}


LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    short wheel;
    int nPos;
    int nNewPos;
    
    switch (uMsg)
    {
    case WM_CREATE:
    {
        Surface.SetHwnd(hwnd);
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 80;
        si.nPage = 10;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case ID_OPEN_OPENFILE:
            {
                opf.lStructSize = sizeof(OPENFILENAMEW);
                opf.hwndOwner = hwnd;
                opf.hInstance = gInstance;
                opf.lpstrFile = FileNameBuf;
                opf.nMaxFile = 500;
                GetOpenFileNameW(&opf);

                REQ = REQUEST_CLEAR;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case ID_OPEN_CLOSEFILE:
            {
                REQ = 12;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case ID_HELP_ABOUT:
            {
                MessageBoxW(hwnd, L"Written by ibrakap", L"About", MB_OK | MB_ICONINFORMATION);
                break;
            }
            case ID_OPEN_EXIT:
            {
                SendMessageW(hwnd, WM_CLOSE, NULL, NULL);
                break;
            }
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_CLOSE:
    {
        free(Buffer);
        DestroyWindow(hwnd);
        break;
    }
    case WM_SIZE:
    {
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_VSCROLL:
    {
        nPos = (short int)HIWORD(wParam);

        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        nNewPos = si.nPos;
        switch (LOWORD(wParam))
        {
        case SB_LINEDOWN:
        {
            nNewPos += 10;
            break;
        }
        case SB_LINEUP:
        {
            nNewPos -= 10;
            break;
        }
        case SB_THUMBPOSITION:
        {
            nNewPos = nPos + si.nMin;
            break;
        }
        }
        si.fMask = SIF_POS;
        si.nPos = nNewPos;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        wheel = GET_WHEEL_DELTA_WPARAM(wParam);

        switch (wheel)
        {
            case WHEEL_UP:
            {
             
                si.nPos -= 10;
                if (f.getpos() < 8192) break;
                f.seek(-8192, SeekMode::current);
                Buffer = f.read(4096);
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case WHEEL_DOWN:
            {
                si.nPos += 10;
                DWORD FPOS = f.getpos();
                DWORD FSIZE = f.getsize();
                if (FPOS == FSIZE || FPOS > FSIZE) break;
                Buffer = f.read(4096);
                InvalidateRect(hwnd, NULL, TRUE);
                break; 
            }
        }

        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        break;
    }
    case WM_PAINT:
    {
        Surface.BeginDraw();

        if (REQ == REQUEST_CLEAR)
        {
            Surface.ClearSurface();
            REQ = 0;
        }
        else
        {
            Surface.DrawTextOnSurface(Buffer);
        }
        Surface.EndDraw();
        break;
    }
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}