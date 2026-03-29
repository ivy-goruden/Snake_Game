cmake_minimum_required(VERSION 3.16)

project(Tetris LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(Tetris
  main.cpp
  tetris_desktop_render.cpp
  desktop_tetris_controller.h
  tetris_desktop_render.h
  ../../../brick_game/tetris/tetris.cpp
  ../../../brick_game/tetris/tetris_fsm.cpp
  ../../../brick_game/globals.cpp
  ../../../brick_game/globals.h
  key_handlers.cpp
  screen_handlers.cpp
  desktop_tetris_controller.cpp
  ../../../brick_game/models/controller.cpp
  desktop_tetris_controller.cpp
  desktop_tetris_controller.cpp
  key_handlers.cpp
  screen_handlers.cpp
  tetris_desktop_render.cpp
  desktop_tetris_controller.cpp
  key_handlers.cpp
  screen_handlers.cpp
  tetris_desktop_render.ui
)
target_link_libraries(Tetris PRIVATE Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS Tetris
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
