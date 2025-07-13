# Wiimote Mouse

<img width="1754" height="881" alt="wiimote mouse logo" src="https://github.com/user-attachments/assets/5b8e3466-a1f2-41cb-9630-8702cc1e253b" />
(logo art by VanillaCold)


-----

WiimoteMouse is a program built on the [Wiiuse library](https://github.com/wiiuse/wiiuse) to use a Wii Remote's IR sensor as a means to control the cursor in Windows.
Although there isn't much practical use for this on regular desktop Windows, there is potential merit for it as a way to control a computer from afar - for home-theatre PCs, for instance.

### Implemented Features
- Left, right and middle mouse buttons, mapped to A, B and 1 respectively.
- Scrolling, using up and down on the directional pad.
- IR cursor pointing, with rudimentary stabilization
- A custom Wii Remote cursor that rotates with the remote, as in the Wii Menu

### Planned Features
- A settings menu, styled after the Wii's Home Menu and accessed by pressing the Home button
- Screen calibration settings
- An option to use the regular Windows cursor instead of the Wii Remote cursor

### Features that I want to add at some point but am unsure how:
- Improved cursor stabilization using Wii Motion Plus.
- Cross-platform support

### How to build:
I haven't fully tested all build configurations - right now I primarily just build via Release, as I know this works.

To begin, you will need to have Wiiuse downloaded and located at `../../wiiuse/`, relative to the repository.
CMake should then be used to create a Visual Studio 2019/2022 solution out of the library, and then it should be built as Release.

From there, open the solution for Wiimote Mouse and build it. It *should* work - key word is *should*, as I haven't tested this on any machine but my own.
