#include "WinHelper.h"
#include "WinFile.h"
#include "COpenFile.h"
#include "CSurface.h"
#include "CScroll.h"
#include "Parser.h"

#define ExWindowStyles (WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES | WS_EX_TOPMOST)
#define WindowStyles   (WS_OVERLAPPEDWINDOW | WS_VSCROLL)

LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

HINSTANCE gInstance;
int REQ;
bool isFileOpened;

File f;
CSurface Surface;
COpenFile oDlg;
CScroll Scroll;
std::string Buffer;



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    File Fuck(R"(C:\Users\ibrakap\Desktop\data_dump.sql)", FileMode::FILE_READ);
    GetAllLineAndPositions(Fuck);
    gInstance = hInstance;
    WNDCLASSEXW wcex;
    MSG sMsg;
    RegisterMyWindowClass(wcex, WindowProc, hInstance);
    HWND  hWnd = CreateMyWindow(WindowStyles, ExWindowStyles, hInstance, 500, 500);
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

        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case ID_OPEN_OPENFILE:
            {
                const char* outp = oDlg.GetFileName();
                f.open(outp, FileMode::FILE_READ);
                isFileOpened = true;

                Scroll.CreateScroll(0, f.getsize(), 10);
                REQ = REQUEST_CLEAR_AND_DRAW;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case ID_OPEN_CLOSEFILE:
            {
                f.close();
                isFileOpened = false;
                Scroll.CreateScroll(0, 100, 100);
                REQ = REQUEST_CLEAR;
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
        DestroyWindow(hwnd);
        break;
    }
    case WM_SIZE:
    {
        REQ = REQUEST_CLEAR_AND_DRAW;
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_VSCROLL:
    {
        if (isFileOpened == true)
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
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        Scroll.GetScrollInfo();
        wheel = GET_WHEEL_DELTA_WPARAM(wParam);
        if (isFileOpened == true)
        {
            switch (wheel)
            {
            case WHEEL_UP:
            {
                f.seek(200);
                f.read(Buffer, -8192);
                REQ = REQUEST_CLEAR_AND_DRAW;
                Scroll.pos -= 10;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            case WHEEL_DOWN:
            {
                Scroll.pos += 10;
                f.read(Buffer, 4096);
                REQ = REQUEST_CLEAR_AND_DRAW;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            }
            }
            Scroll.SetScrollPos(Scroll.pos);
        }
        break;
    }
    case WM_PAINT:
    {
        Surface.BeginDraw();
        if (isFileOpened == true)
        {
            switch (REQ)
            {
            case REQUEST_CLEAR:
                Surface.ClearSurface();
                REQ = 0;
            case REQUEST_CLEAR_AND_DRAW:
                Surface.ClearSurface();
                Surface.DrawTextOnSurface(Buffer.c_str());
                REQ = 0;
            case REQUEST_SCROLL_AND_DRAW:
                Surface.ClearSurface();
                Surface.DrawTextOnSurface(Buffer.c_str());
                REQ = 0;
            }
        }
        Surface.EndDraw();
        break;
    }
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
