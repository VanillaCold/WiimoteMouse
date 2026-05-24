#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_rect.h>
#include <mutex>
#include <unordered_map>

enum class WiimoteUIMessageType
{
    NoMessage,
    CloseMenu,
    SetMode
};

struct WiimoteUIMessage
{
    WiimoteUIMessageType type;
    bool source; // 0 is "wiimote sent", 1 is "ui sent"
    std::unordered_map<std::string, int> data; //simple as pie. Integer is the class used here since it's pretty normal and I can't think of anything that'd need more bits than that.
};

class WiimoteHomeUI
{
    // Internal function to create the menu thread, and the window.
    bool CreateMenuInternal();
    // Internal;
    void CloseMenuInternal();

    bool mbIsWindowOpen;

    std::thread mWindowThread;

    std::mutex mMessageMutex;
    WiimoteUIMessage mMessage;

public:
    WiimoteHomeUI();
    // Simple user-facing "menu open" function
    bool OpenMenu();
    // Simple user-facing "menu close" function.
    void CloseMenu();
    void SendMessage();
    void RecieveMessage();
};
