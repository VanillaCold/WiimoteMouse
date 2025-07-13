
#define OEMRESOURCE
#include <Windows.h>;
#include "WiimoteMouse.h";
#include "WiiCursorHandler.h";
#include <gdiplus.h>;
#include <wingdi.h>;
#include <windows.media.ocr.h>

//Gdiplus::Rect 

float angle = 0;

Gdiplus::PointF RotateAboutPoint(float x, float y, float cx, float cy, float angle)
{
    /// 2D Rotation Matrix:
    /// (cos T      sin T
    ///  -sin T     cos T)
    /// where T is the angle

    float posX = (x - cx) * cos(angle) + (y - cy) * sin(angle);
    float posY = -(x - cx) * sin(angle) + (y - cy) * cos(angle);

    return Gdiplus::PointF(posX+cx, posY+cy);
}


LRESULT CALLBACK WiiCursorWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch (uMsg)
    {

    case WM_DESTROY:
        SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        SetBkColor(hdc, RGB(0x5,0x1,0x1));
        SetBkMode(hdc, TRANSPARENT);
        FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0x5,0x01,0x01)));

        Gdiplus::PointF points[] =
        {
            Gdiplus::PointF(128.0f - 23.0f, 128.0f - 8.0f),
            Gdiplus::PointF(128.0f - 23.0f + 64.0f, 128.0f - 8.0f),
            Gdiplus::PointF(128.0f - 23.0f, 128.0f - 8.0f + 64.0f)
        };

        points[0] = RotateAboutPoint(points[0].X, points[0].Y, 128.0f, 128.0f, angle);
        points[1] = RotateAboutPoint(points[1].X, points[1].Y, 128.0f, 128.0f, angle);
        points[2] = RotateAboutPoint(points[2].X, points[2].Y, 128.0f, 128.0f, angle);
        

        Gdiplus::Graphics gf(hdc);

        gf.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);


        Gdiplus::Bitmap myImage(L"MainCursor.png");

        gf.DrawImage(&myImage, points, 3);//128-23, 128-8, 64, 64);

        angle += 0.01;

        EndPaint(hwnd, &ps);
    }

    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void dispose()
{
    SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
    std::atexit(dispose);
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



