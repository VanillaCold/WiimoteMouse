#pragma once

#include <iostream>
#include <wiiuse.h>

#include <Windows.h>
#include <WinUser.h>

class WiiCursorHandler
{
public:

	void OnConnect();
	void OnDisconnect();

	void Update();
	HWND window;
	
};