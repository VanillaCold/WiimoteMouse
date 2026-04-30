#pragma once


#include <iostream>
#include <wiiuse.h>



#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_rect.h>


#include <inputtino/input.hpp>

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
	WiimoteMouse();

	void ToggleIR(wiimote_t* mote);
	int MainLoop(int* pCursorHandler);

	void HandleEvent(wiimote* remote);
	void MoveMouse(int x, int y, float angle = 0);

	//WiiCursorHandler* mpCursorHandle;
	int targetX;
	int targetY;
	float currentAngle;

	int screenW;
	int screenH;
	float screenDensity;
	SDL_Rect coordinates;

	bool allowUsage;

	float mCurrentCursorX;
	float mCurrentCursorY;

	static inputtino::Mouse mMouse;


};
