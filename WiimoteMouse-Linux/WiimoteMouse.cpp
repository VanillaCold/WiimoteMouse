// WiimoteMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <wiiuse.h>

//#include <Windows.h>
#include <thread>
#include <unistd.h>

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

inputtino::Mouse WiimoteMouse::mMouse = std::move(*inputtino::Mouse::create());


WiimoteMouse::WiimoteMouse()
{
	SDL_DisplayID display = SDL_GetPrimaryDisplay();
	SDL_PropertiesID displayProperties = SDL_GetDisplayProperties(display);
	const SDL_DisplayMode *DM = SDL_GetDesktopDisplayMode(display);

	SDL_Rect coordinates;
	SDL_GetDisplayBounds(display, &coordinates);

	std::cout << coordinates.x << " " << coordinates.y << std::endl;
	std::cout << coordinates.w << " " << coordinates.h << std::endl;


	int w = DM->w * DM->pixel_density;
	int h = DM->h * DM->pixel_density;
	std::cout << w << " " << h << "\n";

	screenW = w;
	screenH = h;
	screenDensity = DM->pixel_density;
	this->coordinates = coordinates;

	targetX = 0;
	targetY = 0;
	currentAngle = 0;
	allowUsage = true;

}

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
	// the max right value is about 560, and the max y value is about 420, so divide by this number minus twice the buffer size.

	// I do plan on making all this customisable. The base values shouldn't need to be changed, I think, but the buffer will need to be malleable.
	
	if (x < 0 || y < 0 || x > 560 || y > 420)
	{
		return;
	}

	targetX = x;
	targetY = y;
	currentAngle = angle;

	while (angle < 0)
	{
		angle += M_PI * 2;
	}
	while (angle > M_PI * 2)
	{
		angle -= M_PI * 2;
	}


	angle = (M_PI * 2) - angle;

	float tX = (x - 280)*cos(angle) - (y - 210)*sin(angle);
	float tY = (x - 280)*sin(angle) + (y - 210)*cos(angle);

	tX += (560.0f/2.0f);
	tY += (420.0f/2.0f);

	std::cout << x << " " << y << " " << angle << " " << tX << " " << tY << "\n";

	//560 420 is the normal res.
	float normX = ((tX-40) / 480.0f);
	float normY = (tY-20) / 380.0f;

	//printf("%f, %f, \n", normX, normY);

	// get the screen height & width.

	// Multiply the position by these - ideally, this will map 1:1 with screen coordinates.
	normX *= screenW;
	normY *= screenH;

	// make sure the X and Y don't exceed the screen resolution, and that they aren't negative.
	//normX = max(min(width, normX), 0);
	//normY = max(min(height, normY), 0);

	//std::cout << normX << " " << normY << " nyaaaa \n";

	// first get the differences in cursor position
	float diffX = mCurrentCursorX - normX;
	float diffY = mCurrentCursorY - normY;

	// Multiply the difference by its square root, and divide by the sqrt of the screen size.
	// this smooths out the position, at the cost of being less responsive.
	// might be possible to use the motion+ gyroscope to better smooth it out? but idk.
	diffX *= (abs(diffX)) / (screenW);
	diffY *= (abs(diffY)) / (screenH);
		
	// Subtract the new smoothed difference in coordinates.
	float newx = mCurrentCursorX - diffX;
	float newy = mCurrentCursorY - diffY;

	// lastly, set the cursor position using the Windows API.

	//SetCursorPos(newx, newy);
	mpCursorHandle->UpdatePosition(newx, newy, angle);

	mCurrentCursorX = newx;
	mCurrentCursorY = newy;

	int newWidth = (coordinates.w + coordinates.x) * screenDensity;
	int newHeight = (coordinates.h + coordinates.y) * screenDensity;
	newx = newx + (coordinates.x * screenDensity);
	newy = newy + (coordinates.y * screenDensity);

	mMouse.move_abs(newx, newy, newWidth, newHeight);
	//std::cout << newx << " " << newy << " nyaaaa \n";

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
		for (int i = 0; i < 4; ++i)
		{
			if (remote->ir.dot[i].visible)
			{
				meanX += remote->ir.dot[i].x;
				meanY += remote->ir.dot[i].y;
				validSources++;
			}
		}
		//printf("\n");
		float angle = 0;

		if (remote->ir.dot[0].visible && remote->ir.dot[1].visible)
		{
			int dx = int(remote->ir.dot[1].x) - int(remote->ir.dot[0].x);
			int dy = int(remote->ir.dot[1].y) - int(remote->ir.dot[0].y);
			
			angle = atan2f(dy, dx);
		}
		//M_PI_4
		auto xPoint = remote->ir.x;
		auto yPoint = remote->ir.y;

		// If there's a non-even or zero number of IR dots detected, we don't want to use the info to move the mouse.
		if (validSources > 1)
		{
			if (validSources % 2 != 0)
			{
				uint minX = 9999;//remote->ir.dot[0].x;
				uint minY = 9999;//remote->ir.dot[0].y;
				uint maxX = -1;//remote->ir.dot[0].x;
				uint maxY = -1;//remote->ir.dot[0].y;

				for (int i = 0; i < validSources; i++)
				{
					if (remote->ir.dot[i].visible)
					{
						minX = std::min(minX, remote->ir.dot[i].x);
						maxX = std::max(maxX, remote->ir.dot[i].x);
						minY = std::min(minY, remote->ir.dot[i].y);
						maxY = std::max(maxY, remote->ir.dot[i].y);
						std::cout << remote->ir.dot[i].x << " " << remote->ir.dot[i].y << " " << i << " ";
					}
					std::cout << "\n";
				}

				xPoint = minX + maxX / 2;
				yPoint = minY + maxY / 2;
			}
			//std::cout << "Mean cursor pos: " << meanX / validSources << " " << meanY / validSources << std::endl;

			// Move to the mean  cursor position - the average of the two sources.
			
			MoveMouse(xPoint, yPoint, angle);

			//MoveMouse(meanX / validSources, meanY / validSources);
		}
		else
		{
			std::cout << "no valid sources. \n";

		}


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_A))
		{
			mMouse.press(inputtino::Mouse::MOUSE_BUTTON::LEFT);
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			std::cout << "A button pressed\n";
		}


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_B))
		{
			mMouse.press(inputtino::Mouse::MOUSE_BUTTON::RIGHT);
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			std::cout << "B button pressed\n";
		}


		if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_ONE))
		{
			// TODO: pleaes clean this code up. like a lot.
			mMouse.press(inputtino::Mouse::MOUSE_BUTTON::MIDDLE);
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			std::cout << "1 button pressed\n";
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_B))
		{
			// TODO: stuff
			mMouse.release(inputtino::Mouse::MOUSE_BUTTON::RIGHT);

			std::cout << "B button rel\n";
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_A))
		{
			mMouse.release(inputtino::Mouse::MOUSE_BUTTON::LEFT);

			std::cout << "A button rel\n";
		}

		if (IS_RELEASED(remote, WIIMOTE_BUTTON_ONE))
		{
			mMouse.release(inputtino::Mouse::MOUSE_BUTTON::MIDDLE);

			std::cout << "1 button rel\n";
		}

	}

}

int WiimoteMouse::MainLoop(WiiCursorHandler* pCursorHandler)
{
	mpCursorHandle = pCursorHandler;

	// TODO get width and height
	auto width = 2560;//GetSystemMetrics(SM_CXSCREEN);
	auto height = 1440;//GetSystemMetrics(SM_CYSCREEN);

	bool lowBatteryAlert = false;
	//main stuff!
	//std::cout << "Hello World!\n";

	// A lot of the connection code is, admittedly, based heavily on the example project
	// this starts out just initialising one single wii remote, as we only want one.
	wiimote_t** mote = wiiuse_init(1);
	std::cout << "init'd a mote\n\n\n";

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		// Find up to one wiimote with a 5 second timeout.
		int found = wiiuse_find(mote, 1, 5);
		if (!found) {
			// Restart the loop at this point
			// so that the program will just be looping for until a wii remote is found.
			std::cout << "nope";
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

		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		wiiuse_rumble(mote[0], 0);

		// give more time to wait so that the wii remote doesn't like
		// mess stuff up.
		std::this_thread::sleep_for(std::chrono::milliseconds(150));


		// enable IR, if it's not enabled.
		if (!WIIUSE_USING_IR(mote[0]))
		{
			wiiuse_set_ir(mote[0], 1);
			//wiiuse_set_ir_sensitivity(mote[0], 2);
			//wiiuse_set_ir_vres(mote[0], width+50, height+50);
		}

		//mpCursorHandle->OnConnect();
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
				timeoutStart = steady_clock::now();
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
			//TODO: scrolling
			if (IS_PRESSED(mote[0], WIIMOTE_BUTTON_UP))
			{
				mMouse.vertical_scroll(1);
			}

			if (IS_PRESSED(mote[0], WIIMOTE_BUTTON_DOWN))
			{
				mMouse.vertical_scroll(-1);
			}

			seconds timeout = duration_cast<seconds>(steady_clock::now() - timeoutStart);
			if (timeout.count() >= 15)
			{
				// assume the wiimote is no longer connected after 15 seconds.


				// just in case it's still connected, clean some stuff up.
				wiiuse_set_leds(mote[0], WIIMOTE_LED_NONE);
				
				std::this_thread::sleep_for(milliseconds(50));

				// disconnect formally.
				wiiuse_disconnect(mote[0]);
				wiiuse_disconnected(mote[0]);

				std::this_thread::sleep_for(milliseconds(50));

				// and now clean up the entire thing,
				// this is so that we can reconnect later.
				wiiuse_cleanup(mote, 1);
				mote = wiiuse_init(1);
				break;
			}

			milliseconds timeSpent = duration_cast<milliseconds>(steady_clock::now() - start);
			if (timeSpent.count() >= 33)
			{
				//printf("%i ms", timeSpent.count());
				start = steady_clock::now();
				//mpCursorHandle->WindowUpdate();
			}
			MoveMouse(targetX, targetY, currentAngle);
		}
		mpCursorHandle->OnDisconnect();
		sleep(1);
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
