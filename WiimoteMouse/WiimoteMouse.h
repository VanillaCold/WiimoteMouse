#pragma once


#include <iostream>
#include <wiiuse.h>

#include <Windows.h>

class WiimoteMouse
{
public:
	void ToggleIR(wiimote_t* mote);
	int MainLoop();

	void HandleEvent(wiimote* remote);
	void MoveMouse(int x, int y);
};