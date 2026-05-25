#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_rect.h>
#include <mutex>
#include <unordered_map>
#include <queue>

enum class WiimoteUIMessageType
{
    NoMessage,
    SetMode,
    IsClosed,
    CloseMenu,
};

struct WiimoteUIMessage
{
    WiimoteUIMessageType type;
    bool source; // 0 is "wiimote sent", 1 is "ui sent"
    std::unordered_map<std::string, int> data; //simple as pie. Integer is the class used here since it's pretty normal and I can't think of anything that'd need more bits than that.

    WiimoteUIMessage()
    {
        type = WiimoteUIMessageType::NoMessage;
        source = 0;
    }
};

class WiimoteHomeUI
{
    // Internal function to create the menu thread, and the window.
    bool CreateMenuInternal();
    // Internal function to tell the menu thread to close.
    void CloseMenuInternal();
    // Internal function that deletes the window.
    void DeleteMenuInternal();
    void HomeMenuLoop();

    bool mbIsWindowOpen;

    std::thread mpWindowThread;
    SDL_Window* mpHomeWindow;
    SDL_Renderer* mpHomeRenderer;
    std::mutex mMessageMutex;
    std::queue<WiimoteUIMessage*> mSentMessages;
    std::queue<WiimoteUIMessage*> mRecievedMessages;
    static WiimoteHomeUI* sInstance;

public:
    WiimoteHomeUI();
    ~WiimoteHomeUI();
    static WiimoteHomeUI* Get();
    // Simple user-facing "menu open" function
    bool OpenMenu();
    // Simple user-facing "menu close" function.
    void CloseMenu();
    void SendMessage(WiimoteUIMessage* msg);
    WiimoteUIMessage* ReceiveMessage(bool receiver);
};
