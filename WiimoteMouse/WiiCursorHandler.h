#pragma once

//#include <iostream>
#include <wiiuse.h>

#include <Windows.h>
#include <WinUser.h>
#include <utility>



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

	float angle;



	static std::pair<float, float> RotateAboutPoint(float x, float y, float cx, float cy, float angle);

	
};