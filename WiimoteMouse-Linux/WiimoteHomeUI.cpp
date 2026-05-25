#include "WiimoteHomeUI.h"
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
    mLastTime = std::chrono::steady_clock::now();

    //mpWindowThread = std::thread(&WiimoteHomeUI::HomeMenuLoop, this);
    mbIsWindowOpen = true;

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
    if (mbIsWindowOpen)
    {
        SDL_Event event;

        for (int i = 0; i < 5; i++){
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    //done = true;
                }
            }
        }

        auto msg = ReceiveMessage(MSG_UI);
        if (msg != nullptr)
        {
            if (msg->type == WiimoteUIMessageType::CloseMenu)
            {
                // stuff
                //done = true;
            }
            delete msg;
        }
        // Do game logic, present a frame, etc.

        SDL_Surface* bmp = SDL_LoadPNG("MainCursor.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(mpHomeRenderer, bmp);
        SDL_DestroySurface(bmp);

        //SDL_Event e;

        SDL_RenderClear(mpHomeRenderer);
        SDL_RenderTexture(mpHomeRenderer, tex, NULL, NULL);
        SDL_RenderPresent(mpHomeRenderer);

        SDL_DestroyTexture(tex);

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

void WiimoteHomeUI::DoLoop()
{
    using namespace std::chrono;
    milliseconds timeout = duration_cast<milliseconds>(steady_clock::now() - mLastTime);
    if (mbIsWindowOpen == true && timeout >= milliseconds(17))
    {
        HomeMenuLoop();
        mLastTime = steady_clock::now();
    }
}




WiimoteHomeUI * WiimoteHomeUI::Get()
{
    return sInstance;
}

WiimoteHomeUI* WiimoteHomeUI::sInstance;

