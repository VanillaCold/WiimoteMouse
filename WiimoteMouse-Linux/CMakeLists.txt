cmake_minimum_required(VERSION 3.10)

project(wiimotemouse-linux)


add_executable(wiimotemouse-linux Entrypoint.cpp)


add_library(wmm
  SHARED
    WiimoteMouse.cpp
    WiimoteMouse.h
  )

target_link_libraries(wmm -lwiiuse)
target_link_libraries(wiimotemouse-linux PRIVATE wmm)


install(TARGETS wiimotemouse-linux RUNTIME DESTINATION bin)
