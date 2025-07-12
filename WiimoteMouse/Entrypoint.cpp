
#include <Windows.h>;
#include "WiimoteMouse.h";
#include "WiiCursorHandler.h";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	auto mouse = new WiimoteMouse();
	mouse->MainLoop();
}