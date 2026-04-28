#pragma once


#include <iostream>
#include <wiiuse.h>

//#include <Windows.h>
//#include "WiiCursorHandler.h"

struct Point
{
	int x = 0;
	int y = 0;
};

class WiimoteMouse
{
public:
	void ToggleIR(wiimote_t* mote);
	int MainLoop(int* pCursorHandler);

	void HandleEvent(wiimote* remote);
	void MoveMouse(int x, int y, float angle = 0);

	//WiiCursorHandler* mpCursorHandle;
	int targetX;
	int targetY;
	float currentAngle;
};
