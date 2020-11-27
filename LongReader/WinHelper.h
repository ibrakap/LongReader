#include <Windows.h>
#include "resource.h"

#define WHEEL_UP     120
#define WHEEL_DOWN  -120

#define REQUEST_CLEAR 12
#define REQUEST_DRAW  13
#define REQUEST_CLEAR_AND_DRAW 14
#define REQUEST_SCROLL_AND_DRAW 15


ATOM RegisterMyWindowClass(WNDCLASSEXW& WinClass, const WNDPROC& WndProc, HINSTANCE hInstance);
HWND CreateMyWindow(DWORD WinStyles, DWORD ExWinStyles, HINSTANCE hInstance, int Width, int Height);
WPARAM LoopMyMessageLoop(MSG& Message, HWND hWnd);