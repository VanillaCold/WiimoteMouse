#include "WiimoteHomeUI.h"
#define MSG_UI 1
#define MSG_WM 0



WiimoteHomeUI::WiimoteHomeUI()
{

}




bool WiimoteHomeUI::CreateMenuInternal()
{

    mpHomeWindow = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_BORDERLESS |
        SDL_WINDOW_TRANSPARENT |
        SDL_WINDOW_ALWAYS_ON_TOP |
        SDL_WINDOW_MOUSE_CAPTURE           // flags
    );
    if (!mpHomeWindow)
    {
        return false;
    }

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

void WiimoteHomeUI::SendMessage(WiimoteUIMessage& msg)
{
    std::lock_guard<std::mutex> guard(mMessageMutex);
    if (msg.source == 1)
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

