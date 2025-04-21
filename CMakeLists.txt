cmake_minimum_required(VERSION 3.16)

project(glgame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets)
if (NOT Qt6_FOUND)
  find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets OpenGL)
endif()

find_package(OpenGL REQUIRED)

set(PROJECT_SOURCES
src/actor.cpp
src/camera/camerabase.cpp
src/camera/fpscamera.cpp

headers/camera/camerabase.h
headers/camera/fpscamera.h
headers/actions.h
headers/actor.h
headers/constants.h

main.cpp
widget.h
widget.cpp

game_console.h
game_console.cpp

widget.ui
resources.qrc
)


add_executable(glgame
    ${PROJECT_SOURCES}

    console_commands.h
    console_commands.cpp
)

target_include_directories(glgame PUBLIC headers)

if(Qt6_FOUND)
    target_link_libraries(glgame PRIVATE Qt6::OpenGLWidgets)
endif()

# https://www.qt.io/blog/versionless-cmake-targets-qt-5.15
target_link_libraries(glgame
    PRIVATE
    Qt::Widgets
    Qt::OpenGL
    Qt::Core
    ${OPENGL_LIBRARIES}
)
