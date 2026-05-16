#define OEMRESOURCE
#include "WiiCursorHandler.h"


//#include <Windows.h>
#include <math.h>


WiiCursorHandler::WiiCursorHandler()//HINSTANCE pInstance)
{
    angle = 0;
    //mpWindow = SDL_CreateWindow("SDL3 Image",512, 512, SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE | SDL_WINDOW_ALWAYS_ON_TOP );
    //mpRenderer = SDL_CreateRenderer(mpWindow, NULL);
    //mpHInstance = pInstance;
    //customCursor = LoadCursorFromFile(L"cursor1.cur");
    //SDL_Surface* bmp = SDL_LoadPNG("MainCursor.png");
    //SDL_Cursor* cursor = SDL_CreateColorCursor(bmp, 10, 10);
    //SDL_Texture* tex = SDL_CreateTextureFromSurface(mpRenderer, bmp);


    //SDL_SetWindowShape(mpWindow, bmp);
    //SDL_RenderClear(mpRenderer);
    //SDL_RenderTexture(mpRenderer, tex, NULL, NULL);
    //SDL_RenderPresent(mpRenderer);
    //SDL_DestroySurface(bmp)
    //SDL_SetCursor(cursor);
    ;

    //SDL_Event e;

}

void WiiCursorHandler::OnConnect()
{
    
    /*mpWindow = CreateWindowEx(
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
    SetWindowPos(mpWindow, nullptr, 0, 0, 256, 256, 0);*/


}

void WiiCursorHandler::OnDisconnect()
{
    //DestroyWindow(mpWindow);
    //SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
}

void WiiCursorHandler::WindowUpdate()
{
    /*RedrawWindow(mpWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    
    // we really, really want to encounter a lot of messages-
    MSG msg = { };
    for (int i = 0; i < 5; i++)
    {
        if (PeekMessage(&msg, NULL, 0, 0, 6) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

}

void WiiCursorHandler::UpdatePosition(int x, int y, float angle)
{
    //SetCapture(mpWindow);
    //ShowCursor(false);
    //SetWindowPos(mpWindow, nullptr, x-128, y-128, 256, 256, 0);
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
