#pragma once
#include <Windows.h>
#include "resource.h"

#define WHEEL_UP     120
#define WHEEL_DOWN  -120



ATOM RegisterMyWindowClass(WNDCLASSEXW& WinClass, const WNDPROC& WndProc, HINSTANCE hInstance)
{
    WinClass.cbSize         = sizeof(WNDCLASSEXW);
    WinClass.style          = (CS_HREDRAW | CS_VREDRAW);
    WinClass.lpfnWndProc    = WndProc;
    WinClass.cbClsExtra     = NULL;
    WinClass.cbWndExtra     = NULL;
    WinClass.hInstance      = hInstance;
    WinClass.hIcon          = NULL;
    WinClass.hCursor        = NULL;
    WinClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WinClass.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    WinClass.lpszClassName  = L"MAINWINDOW";
    WinClass.hIconSm        = NULL;

    return RegisterClassExW(&WinClass);
}
HWND CreateMyWindow(DWORD WinStyles, DWORD ExWinStyles, HINSTANCE hInstance, int Width, int Height)
{
    HWND WinHandle = CreateWindowExW(ExWinStyles, L"MAINWINDOW", L"LongReader", WinStyles, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);
    ShowWindow(WinHandle, SW_SHOW);
    UpdateWindow(WinHandle);
    return WinHandle;
}
WPARAM LoopMyMessageLoop(MSG& Message, HWND hWnd)
{
    while (GetMessageW(&Message, hWnd, NULL, NULL) > 0)
    {
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
    return Message.wParam;
}

#define REQUEST_CLEAR 12
#define REQUEST_DRAW  13
