cmake_minimum_required(VERSION 3.16)

project(Tetris LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(Tetris
  main.cpp
  tetris_desktop_render.cpp
  desktop_tetris_controller.h
  tetris_desktop_render.h
  ../../../brick_game/tetris/tetris.cpp
  ../../../brick_game/tetris/tetris_fsm.cpp
  ../../../brick_game/tetris/helper.cpp
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
  next_figure.h next_figure.cpp next_figure.ui
)
target_link_libraries(Tetris PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS Tetris
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
