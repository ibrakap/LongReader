#include <Windows.h>

class CSurface
{
public:
	void BeginDraw();
	void EndDraw();

	void ClearSurface();
	void DrawTextOnSurface(const char* buffer);

	void SetHwnd(HWND hwnd);
private:
	PAINTSTRUCT ps;
	HDC hDc;
	HWND hWnd;
	RECT  rect;
};
