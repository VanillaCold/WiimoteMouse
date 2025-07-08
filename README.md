# Wiimote Mouse

WiimoteMouse is a program, built on the [Wiiuse library](https://github.com/wiiuse/wiiuse), to use a Wii Remote's IR sensor as a means to control the cursor in Windows.
Although there isn't much practical use for this on regular desktop Windows, there is potential merit for it as a way to control a computer from afar - for home-theatre PCs, for instance.

Right now, only basic mouse usage is implemented - moving the cursor, left clicking with the A button, and right-clicking with B.

### Planned Features:
- Scrolling, likely using either the directional pad or, optionally, Wii Nunchucks.
- The ability to middle-click
- Some way to disconnect a controller, so that it can be used for other programs such as Dolphin.
- Screen calibration & saving/loading
- A Wii Remote cursor sprite - one which would rotate with the remote itself.

### Features that I want to add at some point but am unsure how:
- Improved cursor stabilization using Wii Motion Plus.
- Cross-platform support

### How to build:
I haven't fully tested all build configurations - right now I primarily just build via Release, as I know this works.

To begin, you will need to have Wiiuse downloaded and located at `../../wiiuse/`, relative to the repository.
CMake should then be used to create a Visual Studio 2019/2022 solution out of the library, and then it should be built as Release.

From there, open the solution for Wiimote Mouse and build it. It *should* work - key word is *should*, as I haven't tested this on any machine but my own.
