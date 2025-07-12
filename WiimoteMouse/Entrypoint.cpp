
#include <Windows.h>;
#include "WiimoteMouse.h";
#include "WiiCursorHandler.h";
#include <gdiplus.h>;
#include <wingdi.h>;


LRESULT CALLBACK WiiCursorWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ShowCursor(false);

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

        SetBkColor(hdc, RGB(0x1,0x1,0x1));
        SetBkMode(hdc, TRANSPARENT);
        FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0x01,0x01,0x01)));

        Gdiplus::Graphics gf(hdc);

        Gdiplus::Bitmap myImage(L"MainCursor.png");

        gf.DrawImage(&myImage, 128-23, 128-8, 64, 64);


        EndPaint(hwnd, &ps);
    }

    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    ULONG_PTR m_token = 0;
    Gdiplus::GdiplusStartupInput startupInput;
    Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);
    
    
    const wchar_t CLASS_NAME[] = L"WiiCursorWinProc";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WiiCursorWinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

	auto mouse = new WiimoteMouse();
	auto cursor = new WiiCursorHandler(hInstance);
	mouse->MainLoop(cursor);


    if (m_token)
        Gdiplus::GdiplusShutdown(m_token);
}