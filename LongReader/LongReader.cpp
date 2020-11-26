#include "WinHelper.h"
#include "WinFile.h"
#include "COpenFile.h"
#include "CSurface.h"
#include "CScroll.h"

LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

SCROLLINFO si;
HINSTANCE gInstance;
File f;
char* Buffer = nullptr;
wchar_t FileNameBuf[500];

CSurface Surface;
COpenFile oDlg;
CScroll Scroll;
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
        oDlg.SetHwnd(hwnd, gInstance);
        Surface.SetHwnd(hwnd);
        Scroll.SetHwnd(hwnd);
        Scroll.CreateScroll(0, 80, 10);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case ID_OPEN_OPENFILE:
            {
                const char* outp = oDlg.GetFileName();
                REQ = REQUEST_CLEAR_AND_DRAW;
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
        Scroll.GetScrollInfo();

        switch (LOWORD(wParam))
        {
        case SB_LINEDOWN:
        {
            Scroll.pos += 10;
            break;
        }
        case SB_LINEUP:
        {
            Scroll.pos -= 10;
            break;
        }
        case SB_THUMBPOSITION:
        {
            Scroll.pos = nPos + Scroll.min;
            break;
        }
        }
        Scroll.SetScrollPos(Scroll.pos);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        Scroll.GetScrollInfo();
        wheel = GET_WHEEL_DELTA_WPARAM(wParam);

        switch (wheel)
        {
            case WHEEL_UP:
            {
                Scroll.pos -= 10;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case WHEEL_DOWN:
            {
                Scroll.pos += 10;
                InvalidateRect(hwnd, NULL, TRUE);
                break; 
            }
        }
        Scroll.SetScrollPos(Scroll.pos);
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
        else if (REQ == REQUEST_CLEAR_AND_DRAW)
        {
            Surface.ClearSurface();
            Surface.DrawTextOnSurface(Buffer);
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