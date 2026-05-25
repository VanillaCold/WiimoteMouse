cmake_minimum_required(VERSION 3.11)
include(FetchContent)

project(wiimotemouse-linux)


add_executable(wiimotemouse-linux Entrypoint.cpp)
add_compile_options(-fPIC)


add_library(wmm
  SHARED
    WiiCursorHandler.cpp
    WiiCursorHandler.h
    WiimoteMouse.cpp
    WiimoteMouse.h
    WiimoteHomeUI.cpp
    WiimoteHomeUI.h
  )


find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

FetchContent_Declare(
  inputtino
  GIT_REPOSITORY https://github.com/games-on-whales/inputtino.git
  GIT_TAG f4ce2b0df536ef309e9ff318f75b460f7097d7c1
)
FetchContent_MakeAvailable(inputtino)
target_link_libraries(wmm inputtino::libinputtino)

target_link_libraries(wmm -lwiiuse)
target_link_libraries(wmm SDL3::SDL3)
target_link_libraries(wiimotemouse-linux PRIVATE wmm)
target_link_libraries(wiimotemouse-linux PUBLIC inputtino::libinputtino)


install(TARGETS wiimotemouse-linux RUNTIME DESTINATION bin)
