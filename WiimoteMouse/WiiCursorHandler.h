#pragma once

//#include <iostream>
#include <wiiuse.h>

#include <Windows.h>
#include <WinUser.h>

class WiiCursorHandler
{
public:
	WiiCursorHandler(HINSTANCE pInstance);

	void OnConnect();
	void OnDisconnect();

	void WindowUpdate();

	void UpdatePosition(int x, int y, float angle);
	HWND mpWindow;
	HINSTANCE mpHInstance;
	
};