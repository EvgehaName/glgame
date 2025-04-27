cmake_minimum_required(VERSION 3.16)
project(glgame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

file(GLOB_RECURSE PROJECT_HPP  "src/*.h")
file(GLOB_RECURSE PROJECT_CPP  "src/*.cpp")
file(GLOB_RECURSE PROJECT_UIC  "src/*.ui")
file(GLOB_RECURSE PROJECT_RCC  "src/*.qrc")

source_group(TREE ${CMAKE_SOURCE_DIR}
    FILES
    ${PROJECT_HPP}
    ${PROJECT_CPP}
    ${PROJECT_UIC}
    ${PROJECT_RCC}
)

# https://www.qt.io/blog/versionless-cmake-targets-qt-5.15
find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets)
if (NOT Qt6_FOUND)
  find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets OpenGL)
endif()

add_executable(${PROJECT_NAME}
    ${PROJECT_HPP}
    ${PROJECT_CPP}
    ${PROJECT_UIC}
    ${PROJECT_RCC}
)

#--------------------------------------------------------------------------
# External libraries
#--------------------------------------------------------------------------
find_package(ode CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ODE::ODE)

if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME}
      PRIVATE
      Qt6::OpenGLWidgets
    )
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt::Widgets
    Qt::OpenGL
    Qt::Core
    ${OPENGL_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
