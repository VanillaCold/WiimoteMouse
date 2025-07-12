
#include <Windows.h>;
#include "WiimoteMouse.h";
#include "WiiCursorHandler.h";


LRESULT CALLBACK WiiCursorWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, HBRUSH(GetStockObject(BLACK_BRUSH)));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"WiiCursorWinProc";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WiiCursorWinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

	auto mouse = new WiimoteMouse();
	auto cursor = new WiiCursorHandler(hInstance);
	mouse->MainLoop(cursor);
}