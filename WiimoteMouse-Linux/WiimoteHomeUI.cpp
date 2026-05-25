#include "WiimoteHomeUI.h"
#include <iostream>
#define MSG_UI 1
#define MSG_WM 0




WiimoteHomeUI::WiimoteHomeUI()
{
    sInstance = this;
}

WiimoteHomeUI::~WiimoteHomeUI()
{
    sInstance = nullptr;
}



bool WiimoteHomeUI::CreateMenuInternal()
{

    mpHomeWindow = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        640,                               // width, in pixels
        480,                               // height, in pixels
        //SDL_WINDOW_OPENGL |
        SDL_WINDOW_BORDERLESS |
        SDL_WINDOW_TRANSPARENT |
        SDL_WINDOW_ALWAYS_ON_TOP |
        SDL_WINDOW_MOUSE_CAPTURE           // flags
    );
    if (!mpHomeWindow)
    {
        return false;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(mpHomeWindow, NULL);
    if (!ren)
    {
        SDL_DestroyWindow(mpHomeWindow);
        mpHomeWindow = nullptr;
        return false;
    }
    mpHomeRenderer = ren;
    SDL_Surface* bmp = SDL_LoadPNG("MainCursor.png");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_DestroySurface(bmp);

    //SDL_Event e;

    SDL_RenderClear(ren);
    //SDL_RenderTexture(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);

    SDL_DestroyTexture(tex);

    mpWindowThread = std::thread(&WiimoteHomeUI::HomeMenuLoop, this);

    return true;
}


void WiimoteHomeUI::CloseMenuInternal()
{
    //TODO: close menu
}

void WiimoteHomeUI::DeleteMenuInternal()
{
    //TODO: delete the menu window!
}

void WiimoteHomeUI::HomeMenuLoop()
{
    bool done = false;
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {

                //done = true;
            }
        }

        auto msg = ReceiveMessage(MSG_UI);
        if (msg != nullptr)
        {
            if (msg->type == WiimoteUIMessageType::CloseMenu)
            {
                done = true;
            }
            delete msg;
        }
        // Do game logic, present a frame, etc.
        SDL_FRect rect(500, 500);

        SDL_RenderClear(mpHomeRenderer);
        SDL_RenderFillRect(mpHomeRenderer, &rect);
        SDL_RenderPresent(mpHomeRenderer);
        std::cout << "nya" << "\n";
    }

}


bool WiimoteHomeUI::OpenMenu()
{
    return CreateMenuInternal();
}

void WiimoteHomeUI::CloseMenu()
{
    CloseMenuInternal();
}

void WiimoteHomeUI::SendMessage(WiimoteUIMessage* msg)
{
    std::lock_guard<std::mutex> guard(mMessageMutex);
    if (msg->source == 1)
    {
        mSentMessages.emplace(msg);
        return;
    }
    mRecievedMessages.emplace(msg);
}


// 0 is that the wiimote is the receiver, 1 is that the UI is the receiver.
WiimoteUIMessage* WiimoteHomeUI::ReceiveMessage(bool receiver)
{
    std::lock_guard<std::mutex> guard(mMessageMutex);
    if (receiver == MSG_UI)
    {
        // recieved from wiimote, go for mRecievedMessages
        if (mRecievedMessages.empty())
        {
            return nullptr;
        }
        auto value = mRecievedMessages.front();
        mRecievedMessages.pop();
        return value;
    }
    else
    {
        // recieved from UI, go for mSentMessages
        if (mSentMessages.empty())
        {
            return nullptr;
        }
        auto value = mSentMessages.front();
        mSentMessages.pop();
        return value;
    }
}



WiimoteHomeUI * WiimoteHomeUI::Get()
{
    return sInstance;
}

WiimoteHomeUI* WiimoteHomeUI::sInstance;

