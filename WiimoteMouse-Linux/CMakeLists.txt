cmake_minimum_required(VERSION 3.10)

project(wiimotemouse-linux)


add_executable(wiimotemouse-linux Entrypoint.cpp)


add_library(wmm
  SHARED
    WiimoteMouse.cpp
    WiimoteMouse.h
  )


find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

target_link_libraries(wmm -lwiiuse)
target_link_libraries(wmm SDL3::SDL3)
target_link_libraries(wiimotemouse-linux PRIVATE wmm)


install(TARGETS wiimotemouse-linux RUNTIME DESTINATION bin)
