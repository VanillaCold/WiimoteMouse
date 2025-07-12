#pragma once


#include <iostream>
#include <wiiuse.h>

#include <Windows.h>
#include "WiiCursorHandler.h"

class WiimoteMouse
{
public:
	void ToggleIR(wiimote_t* mote);
	int MainLoop(WiiCursorHandler* pCursorHandler);

	void HandleEvent(wiimote* remote);
	void MoveMouse(int x, int y);

	WiiCursorHandler* mpCursorHandle;
};