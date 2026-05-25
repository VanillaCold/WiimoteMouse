#include "WiimoteHomeUI.h"



WiimoteHomeUI::WiimoteHomeUI()
{
}




bool WiimoteHomeUI::CreateMenuInternal()
{
    //TODO: create window.
    return false;
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
    if (receiver == 1)
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

