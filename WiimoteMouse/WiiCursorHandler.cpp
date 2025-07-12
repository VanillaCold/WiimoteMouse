#include "WiiCursorHandler.h";
#include <Windows.h>



WiiCursorHandler::WiiCursorHandler(HINSTANCE pInstance)
{
    mpHInstance = pInstance;
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
    SetLayeredWindowAttributes(mpWindow, RGB(0x1,0x1,0x1), 255, LWA_ALPHA | LWA_COLORKEY);
    SetBkColor(dc, 0x1);

    ReleaseDC(mpWindow, dc);

    UpdateWindow(mpWindow);

    

    ShowWindow(mpWindow, 1);
    SetWindowPos(mpWindow, nullptr, 0, 0, 256, 256, 0);


}

void WiiCursorHandler::OnDisconnect()
{
}

void WiiCursorHandler::WindowUpdate()
{
    UpdateWindow(mpWindow);
    //RedrawWindow(mpWindow, NULL, NULL, RDW_INVALIDATE);
    MSG msg = { };
    if (PeekMessage(&msg, NULL, 0, 0, 3) > 0)
    {
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

}

void WiiCursorHandler::UpdatePosition(int x, int y, float angle)
{
    SetCapture(mpWindow);
    ShowCursor(false);
    SetWindowPos(mpWindow, nullptr, x-128, y-128, 256, 256, 0);
}
