#define OEMRESOURCE
#include "WiiCursorHandler.h";


#include <Windows.h>
#include <math.h>


WiiCursorHandler::WiiCursorHandler(HINSTANCE pInstance)
{
    angle = 0;
    mpHInstance = pInstance;
    customCursor = LoadCursorFromFile(L"cursor1.cur");
}

void WiiCursorHandler::OnConnect()
{
    
    mpWindow = CreateWindowEx(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT | WS_EX_TOPMOST,                              // Optional window styles.
        L"WiiCursorWinProc",                     // Window class
        L"",    // Window text
        WS_POPUP,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 256, 256,

        NULL,       // Parent window    
        NULL,       // Menu
        GetModuleHandle(NULL),//mpHInstance,  // Instance handle
        NULL        // Additional application data
    );



    auto dc = GetDC(mpWindow);

    SetBkMode(dc, TRANSPARENT);
    SetLayeredWindowAttributes(mpWindow, RGB(0x5,0x1,0x1), 255, LWA_ALPHA | LWA_COLORKEY);
    SetBkColor(dc, 0x1);

    ReleaseDC(mpWindow, dc);

    UpdateWindow(mpWindow);

    

    ShowWindow(mpWindow, 1);
    SetWindowPos(mpWindow, nullptr, 0, 0, 256, 256, 0);


    //CreateCursor(NULL, 0, 0, 1, 1, new BYTE (0), new BYTE (1));
    

    //Need to copy the cursor each time as SetSystemCursor consumes the cursor
    HCURSOR cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_APPSTARTING);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_NORMAL);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_CROSS);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_HAND);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_IBEAM);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_NO);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_SIZEALL);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_SIZENESW);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_SIZENS);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_SIZENWSE);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_SIZEWE);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_UP);

    cursorCopy = CopyCursor(customCursor);
    SetSystemCursor(cursorCopy, OCR_WAIT);


}

void WiiCursorHandler::OnDisconnect()
{
    DestroyWindow(mpWindow);
    SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
}

void WiiCursorHandler::WindowUpdate()
{
    RedrawWindow(mpWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    
    // we really, really want to encounter a lot of messages-
    MSG msg = { };
    for (int i = 0; i < 5; i++)
    {
        if (PeekMessage(&msg, NULL, 0, 0, 6) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

}

void WiiCursorHandler::UpdatePosition(int x, int y, float angle)
{
    SetCapture(mpWindow);
    ShowCursor(false);
    SetWindowPos(mpWindow, nullptr, x-128, y-128, 256, 256, 0);
    this->angle = angle;
}

std::pair<float, float> WiiCursorHandler::RotateAboutPoint(float x, float y, float cx, float cy, float angle)
{
    /// 2D Rotation Matrix:
    /// (cos T      sin T
    ///  -sin T     cos T)
    /// where T is the angle

    float posX = (x - cx) * cos(angle) + (y - cy) * sin(angle);
    float posY = -(x - cx) * sin(angle) + (y - cy) * cos(angle);

    return std::pair<float, float>(posX + cx, posY + cy);
}