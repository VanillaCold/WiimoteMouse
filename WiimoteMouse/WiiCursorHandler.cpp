#include "WiiCursorHandler.h";

#include <thread>


WiiCursorHandler::WiiCursorHandler(HINSTANCE pInstance)
{
    mpHInstance = pInstance;
}

void WiiCursorHandler::OnConnect()
{
    
    mpWindow = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,                              // Optional window styles.
        L"WiiCursorWinProc",                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_BORDER,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 128, 128,

        NULL,       // Parent window    
        NULL,       // Menu
        GetModuleHandle(NULL),//mpHInstance,  // Instance handle
        NULL        // Additional application data
    );


    auto dc = GetDC(mpWindow);

    SetBkMode(dc, TRANSPARENT);
    SetLayeredWindowAttributes(mpWindow, 0x00000000, 200, LWA_ALPHA | LWA_COLORKEY);
    SetBkColor(dc, 0x00000000);

    ReleaseDC(mpWindow, dc);

    UpdateWindow(mpWindow);

    

    ShowWindow(mpWindow, 1);
    SetWindowPos(mpWindow, nullptr, 0, 0, 128, 128, 0);


}

void WiiCursorHandler::OnDisconnect()
{
}

void WiiCursorHandler::WindowUpdate()
{
    UpdateWindow(mpWindow);
    MSG msg = { };
    if (PeekMessage(&msg, NULL, 0, 0, 3) > 0)
    {
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }
}

void WiiCursorHandler::UpdatePosition(int x, int y, float angle)
{
    SetWindowPos(mpWindow, nullptr, x-64, y-64, 128, 128, 0);
}
