
#define OEMRESOURCE
#include <Windows.h>;
#include "WiimoteMouse.h";
#include "WiiCursorHandler.h";
#include <gdiplus.h>;
#include <wingdi.h>;
#include <windows.media.ocr.h>

//Gdiplus::Rect 

WiiCursorHandler* cursorHandler;
Gdiplus::Bitmap* cursorImage;
HBRUSH colBrush;

LRESULT CALLBACK WiiCursorWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch (uMsg)
    {

    case WM_DESTROY:
        SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
        PostQuitMessage(0);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

    case WM_PAINT:
    {
        float angle = cursorHandler->angle;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        //SetBkColor(hdc, RGB(0x5,0x1,0x1));
        //SetBkMode(hdc, TRANSPARENT);
        FillRect(hdc, &ps.rcPaint, colBrush);

        Gdiplus::PointF points[] =
        {
            Gdiplus::PointF(128.0f - 23.0f, 128.0f - 8.0f),
            Gdiplus::PointF(128.0f - 23.0f + 64.0f, 128.0f - 8.0f),
            Gdiplus::PointF(128.0f - 23.0f, 128.0f - 8.0f + 64.0f)
        };


        auto p1 = WiiCursorHandler::RotateAboutPoint(points[0].X, points[0].Y, 128.0f, 128.0f, angle);
        points[0].X = p1.first;
        points[0].Y = p1.second;

        auto p2 = WiiCursorHandler::RotateAboutPoint(points[1].X, points[1].Y, 128.0f, 128.0f, angle);
        points[1].X = p2.first;
        points[1].Y = p2.second;

        auto p3 = WiiCursorHandler::RotateAboutPoint(points[2].X, points[2].Y, 128.0f, 128.0f, angle);
        points[2].X = p3.first;
        points[2].Y = p3.second;
        

        Gdiplus::Graphics gf(hdc);

        gf.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);



        gf.DrawImage(cursorImage, points, 3);//128-23, 128-8, 64, 64);


        EndPaint(hwnd, &ps);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void dispose()
{
    SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    FILE* fp;

    AllocConsole();
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    

    SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
    std::atexit(dispose);
    ULONG_PTR m_token = 0;
    Gdiplus::GdiplusStartupInput startupInput;
    Gdiplus::GdiplusStartup(&m_token, &startupInput, NULL);


    Gdiplus::Bitmap img(L"MainCursor.png");
    cursorImage = &img;

    colBrush = CreateSolidBrush(RGB(0x5, 0x01, 0x01));
    
    
    const wchar_t CLASS_NAME[] = L"WiiCursorWinProc";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WiiCursorWinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

	auto mouse = new WiimoteMouse();
	auto cursor = new WiiCursorHandler(hInstance);

    cursorHandler = cursor;

	mouse->MainLoop(cursor);


    if (m_token)
        Gdiplus::GdiplusShutdown(m_token);
}



