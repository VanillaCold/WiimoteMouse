
//#define OEMRESOURCE
//#include <Windows.h>;
#include <iostream>
#include "WiimoteMouse.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_rect.h>
#include "WiiCursorHandler.h"
#include "WiimoteHomeUI.h"
//#include <gdiplus.h>;
//#include <wingdi.h>;
//#include <windows.media.ocr.h>

//Gdiplus::Rect 

WiiCursorHandler* cursorHandler;
WiimoteHomeUI* WiimoteHome;
WiimoteMouse* Mouse;
//Gdiplus::Bitmap* cursorImage;
//HBRUSH colBrush;

void dispose()
{
    delete cursorHandler;
    delete WiimoteHome;
    delete Mouse;
}



int main(int argc, char **argv) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 0;
    }
    //SDL_Window* win = SDL_CreateWindow("SDL3 Image",512, 512, SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE | SDL_WINDOW_ALWAYS_ON_TOP );
    //SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
    //SDL_Surface* bmp = SDL_LoadPNG("MainCursor.png");
    //SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);


    //SDL_SetWindowShape(win, bmp);
    //SDL_DestroySurface(bmp);

    //SDL_Event e;

    //SDL_RenderClear(ren);
    //SDL_RenderTexture(ren, tex, NULL, NULL);
    //SDL_RenderPresent(ren);



    WiimoteHomeUI* home = new WiimoteHomeUI();
    WiimoteHome = home;
    auto cursor = new WiiCursorHandler();
    home->OpenMenu();

    cursorHandler = cursor;
    auto mouse = new WiimoteMouse();
    Mouse = mouse;
    mouse->MainLoop(cursorHandler);

    dispose();
}

