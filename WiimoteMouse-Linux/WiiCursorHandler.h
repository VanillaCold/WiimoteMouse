#pragma once

//#include <iostream>
#include <wiiuse.h>

//#include <Windows.h>
//#include <WinUser.h>
#include <utility>


#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_rect.h>



class WiiCursorHandler
{
public:
	WiiCursorHandler();//HINSTANCE pInstance);

	void OnConnect();
	void OnDisconnect();

	void WindowUpdate();

	void UpdatePosition(int x, int y, float angle);
	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;
	//HINSTANCE mpHInstance;

	//HCURSOR customCursor;

	float angle;



	static std::pair<float, float> RotateAboutPoint(float x, float y, float cx, float cy, float angle);

	
};
