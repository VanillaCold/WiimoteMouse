// WiimoteMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <wiiuse.h>

#include <Windows.h>
#include <thread>

#define _USE_MATH_DEFINES
#include <math.h>

#include "WiimoteMouse.h"


extern "C" {
	// C Function call
	wiimote_t** wiiuse_init(int);
	int wiiuse_find(wiimote_t**, int, int);
	int wiiuse_connect(wiimote_t**, int);
	void wiiuse_set_ir(wiimote_t*, int);
	void wiiuse_set_ir_sensitivity(wiimote_t*, int);
	int wiiuse_poll(wiimote_t**, int);
}

bool allowUsage = true;

void WiimoteMouse::ToggleIR(wiimote_t* mote)
{
	if (!WIIUSE_USING_IR(mote) && allowUsage == false)
	{
		wiiuse_set_ir(mote, 1);
	}
	allowUsage = !allowUsage;
}


void WiimoteMouse::MoveMouse(int x, int y, float angle)
{
	//printf("%i, %i, \n", x, y);

	// "normalise" the position
	// Through trial and error, I found that these numbers work best on my screen for finding the boundaries on where the wii remote can reach.
	// I don't think it's particularly normalised, but oh well.
	// Simultaneously, I do need to make a way to calibrate this with anyone's screen.

	// For instance,

	// Give a buffer of 40 on X and 20 on Y.
	// the max right value is about 550, and the max y value is about 420, so divide by this number minus twice the buffer size.

	// I do plan on making all this customisable. The base values shouldn't need to be changed, I think, but the buffer will need to be malleable.
	
	float normX = ((x-40) / 470.0f);
	float normY = (y-20) / 380.0f;


	//printf("%f, %f, \n", normX, normY);

	// get the screen height & width.
	auto width = GetSystemMetrics(SM_CXSCREEN);
	auto height = GetSystemMetrics(SM_CYSCREEN);

	// Multiply the position by these - ideally, this will map 1:1 with screen coordinates.
	normX *= width;
	normY *= height;

	// make sure the X and Y don't exceed the screen resolution, and that they aren't negative.
	//normX = max(min(width, normX), 0);
	//normY = max(min(height, normY), 0);


	
	POINT point;
	
	// Smooth out smaller movements
	if (GetCursorPos(&point))
	{
		// first get the differences in cursor position
		float diffX = point.x - normX;
		float diffY = point.y - normY;

		// Multiply the difference by its square root, and divide by the sqrt of the screen size.
		// this smooths out the position, at the cost of being less responsive.
		// might be possible to use the motion+ gyroscope to better smooth it out? but idk.
		diffX *= sqrt(abs(diffX)) / sqrt(width);
		diffY *= sqrt(abs(diffY)) / sqrt(height);
		
		// Subtract the new smoothed difference in coordinates.
		int newx = point.x - diffX;
		int newy = point.y - diffY;

		// lastly, set the cursor position using the Windows API.
		SetCursorPos(newx, newy);
		mpCursorHandle->UpdatePosition(newx, newy, angle);

		while (ShowCursor(false) >= 0);

	}
}

void WiimoteMouse::HandleEvent(wiimote* remote)
{


	if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_HOME) && IS_PRESSED(remote, WIIMOTE_BUTTON_TWO))
	{
		ToggleIR(remote);
	}

	// only do mouse stuff if the IR sensor is on!
	if (WIIUSE_USING_IR(remote) && allowUsage)
	{
		int meanX = 0;
		int meanY = 0;

		int validSources = 0;

		//printf("sources are: ");

		// In this case, we're only interested in the first two sensors,
		// as dealing with errors in the other two could result in some wonky shenanigans.
		for (int i = 0; i < 4; ++i) {
			if (remote->ir.dot[i].visible) {



				//HDC screenDC = ::GetDC(0);
				//::Rectangle(screenDC, remote->ir.dot[i].x-1, remote->ir.dot[i].y-1, remote->ir.dot[i].x+1, remote->ir.dot[i].y+1);
				//::ReleaseDC(0, screenDC);

				//if (validSources < 2)
				//{
				meanX += remote->ir.dot[i].x;
				meanY += remote->ir.dot[i].y;
				validSources++;

				//printf("%u, %u, ", remote->ir.dot[i].x, remote->ir.dot[i].y);
				//}
			}
		}
		//printf("\n");
		float angle = 0;

		if (remote->ir.dot[0].visible && remote->ir.dot[1].visible)
		{
			int dx = int(remote->ir.dot[1].x) - int(remote->ir.dot[0].x);
			int dy = int(remote->ir.dot[1].y) - int(remote->ir.dot[0].y);

			//printf("point diff is %u, %u, \n", dx, dy);
			
			angle = atan2f(dy, dx);

			//printf("angle is %f\n", angle);

			//string.sprintf("%f", angle);
		}
		//M_PI_4
		auto xPoint = remote->ir.x;
		auto yPoint = remote->ir.y;

		if (angle != 0)
		{
			auto point = WiiCursorHandler::RotateAboutPoint(remote->ir.x, remote->ir.y, remote->ir.vres[0] / 2, remote->ir.vres[1] / 2, angle);
			xPoint = point.first;
			yPoint = point.second;
		}


		//printf("IR cursor: (%u, %u)\n", remote->ir.x, remote->ir.y);
		//printf("IR z distance: %f\n", remote->ir.z);

		// If there's a non-even or zero number of IR dots detected, we don't want to use the info to move the mouse.
		if (validSources != 0)
		{
			//std::cout << "Mean cursor pos: " << meanX / validSources << " " << meanY / validSources << std::endl;

			// Move to the mean  cursor position - the average of the two sources.
			
			MoveMouse(xPoint, yPoint, angle);

			//MoveMouse(meanX / validSources, meanY / validSources);
		};


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_A))
		{
			// Use the Windows API to set up and send a left click
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;


			SendInput(1, &input, sizeof(INPUT));
		}


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_B))
		{
			// Use the Windows API to set up and send a right click
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;


			SendInput(1, &input, sizeof(INPUT));
		}


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_ONE))
		{
			// Use the Windows API to set up and send a right click
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;


			SendInput(1, &input, sizeof(INPUT));
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_B))
		{
			// Use the Windows API to set up and send a left release
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;


			SendInput(1, &input, sizeof(INPUT));
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_A))
		{
			// Use the Windows API to set up and send a right release
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

			SendInput(1, &input, sizeof(INPUT));
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_ONE))
		{
			// Use the Windows API to set up and send a right release
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

			SendInput(1, &input, sizeof(INPUT));
		}

		if (IS_PRESSED(remote, WIIMOTE_BUTTON_UP))
		{

			// Use the Windows API to set up and send a right release
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_WHEEL;
			input.mi.mouseData = 20;

			SendInput(1, &input, sizeof(INPUT));
		}

		if (IS_PRESSED(remote, WIIMOTE_BUTTON_DOWN))
		{

			// Use the Windows API to set up and send a right release
			INPUT input;
			input.type = INPUT_MOUSE;
			input.mi = MOUSEINPUT();
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_WHEEL;
			input.mi.mouseData = -20;

			SendInput(1, &input, sizeof(INPUT));
		}


	}

}
// Currently unused bc I couldn't figure out how to make it work-
/*void low_battery_alert(wiimote_t* remote)
{
	


	// Repeatedly enable and disable the rumble.
	// we want to alert the user that the battery is low!
	wiiuse_rumble(remote, 1);
	wiiuse_set_leds(remote, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
	Sleep(200);

	// this code is spaghetti, but like. i don't want to risk a crash if it suddenly disconnects.

	if (!WIIMOTE_IS_CONNECTED(remote))
	{
		return;
	}

	wiiuse_rumble(remote, 0);
	wiiuse_set_leds(remote, WIIMOTE_LED_NONE);
	Sleep(200);

	if (!WIIMOTE_IS_CONNECTED(remote))
	{
		return;
	}

	wiiuse_rumble(remote, 1);
	wiiuse_set_leds(remote, WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
	Sleep(200);

	if (!WIIMOTE_IS_CONNECTED(remote))
	{
		return;
	}

	wiiuse_rumble(remote, 0);
	wiiuse_set_leds(remote, WIIMOTE_LED_NONE);
	Sleep(200);

	if (!WIIMOTE_IS_CONNECTED(remote))
	{
		return;
	}

	// low battery has been alerted, so we don't alert again.
	wiiuse_set_leds(remote, WIIMOTE_LED_1);
}*/

int WiimoteMouse::MainLoop(WiiCursorHandler* pCursorHandler)
{
	mpCursorHandle = pCursorHandler;

	auto width = GetSystemMetrics(SM_CXSCREEN);
	auto height = GetSystemMetrics(SM_CYSCREEN);

	bool lowBatteryAlert = false;
	//main stuff!
	//std::cout << "Hello World!\n";

	// A lot of the connection code is, admittedly, based heavily on the example project
	// this starts out just initialising one single wii remote, as we only want one.
	wiimote_t** mote = wiiuse_init(1);

	while (true)
	{

		Sleep(150);
		// Find up to one wiimote with a 5 second timeout.
		int found = wiiuse_find(mote, 1, 5);
		if (!found) {
			// Restart the loop at this point
			// so that the program will just be looping for until a wii remote is found.
			continue;
		}
		else
		{
			// When it is found, say that it's found.
			std::cout << "wiimote found :D" << std::endl;
		}

		// Connect to the found wii remote.
		// not going to sugar-coat it, this small code section is literally just from the example project-
		int connected = wiiuse_connect(mote, 1);
		if (connected) {
			printf("Connected to %i wiimotes (of %i found).\n", connected, found);
		}
		else {
			std::cout << "Failed to connect" << std::endl;
			continue;
		}

		wiiuse_set_leds(mote[0], WIIMOTE_LED_1);
		wiiuse_rumble(mote[0], 1);

		Sleep(200);

		wiiuse_rumble(mote[0], 0);

		// give more time to wait so that the wii remote doesn't like
		// mess stuff up.
		Sleep(150);


		// enable IR, if it's not enabled.
		if (!WIIUSE_USING_IR(mote[0]))
		{
			wiiuse_set_ir(mote[0], 1);
			//wiiuse_set_ir_sensitivity(mote[0], 2);
			//wiiuse_set_ir_vres(mote[0], width+50, height+50);
		}

		mpCursorHandle->OnConnect();
		// The code was kinda ugly before
		using namespace std::chrono;


		auto start = steady_clock::now();
		auto timeoutStart = steady_clock::now();

		// Now, if the wii remote is connected, begin looping again.
		while (WIIMOTE_IS_CONNECTED(mote[0]))
		{
			
			// If the wii remote gives an event,
			if (wiiuse_poll(mote, 1))
			{
				// we know the wiimote is still active!
				timeoutStart = std::chrono::steady_clock::now();
				// then get the remote
				auto remote = mote[0];
				// and get its most recent event.
				switch (remote->event)
				{
					
					// We only care about regular events, so break for everything else - outside of a status event.
				case WIIUSE_EVENT:
					// forward to the handle_event script.
					HandleEvent(remote);
					break;
				default:
					break;
				}
			

			}

			std::chrono::seconds timeout = duration_cast<seconds>(steady_clock::now() - timeoutStart);
			if (timeout.count() >= 15)
			{
				// assume the wiimote is no longer connected after 15 seconds.


				// just in case it's still connected, clean some stuff up.
				wiiuse_set_leds(mote[0], WIIMOTE_LED_NONE);
				
				Sleep(50);

				// disconnect formally.
				wiiuse_disconnect(mote[0]);
				wiiuse_disconnected(mote[0]);

				Sleep(50);

				// and now clean up the entire thing,
				// this is so that we can reconnect later.
				wiiuse_cleanup(mote, 1);
				mote = wiiuse_init(1);
				break;
			}

			std::chrono::milliseconds timeSpent = duration_cast<milliseconds>(steady_clock::now() - start);
			if (timeSpent.count() >= 1)
			{
				start = std::chrono::steady_clock::now();
				mpCursorHandle->WindowUpdate();
			}
		}
		mpCursorHandle->OnDisconnect();

	}
	
    
  
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
