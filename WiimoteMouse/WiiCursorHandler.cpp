#include "WiiCursorHandler.h";



WiiCursorHandler::WiiCursorHandler(HINSTANCE pInstance)
{
    mpHInstance = pInstance;
}

void WiiCursorHandler::OnConnect()
{
    
    mpWindow = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,                              // Optional window styles.
        L"STATIC",                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        GetModuleHandle(NULL),//mpHInstance,  // Instance handle
        NULL        // Additional application data
    );

    SetLayeredWindowAttributes(mpWindow, 0xFFFFFFFF, 25, LWA_ALPHA);
    ShowWindow(mpWindow, 1);
}

void WiiCursorHandler::OnDisconnect()
{
}

void WiiCursorHandler::UpdatePosition(int x, int y, float angle)
{
    SetWindowPos(mpWindow, nullptr, x-64, y-64, 128, 128, 0);
}
