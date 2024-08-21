cmake_minimum_required(VERSION 3.27)
project(Hexxagon)

set(CMAKE_CXX_STANDARD 23)

add_executable(Hexxagon main.cpp
        Board.h
        Board.h
        Board.cpp
        EnumsAndGlobals.h
        EnumsAndGlobals.cpp
        EnumsAndGlobals.cpp
        Hexxagon.cpp
        Hexxagon.h
        Menu.cpp
        Menu.h
        Tile.cpp
        Tile.h
        WinScreen.cpp
        WinScreen.h)

set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FetchContent_MakeAvailable(SFML)

target_link_libraries(Hexxagon PRIVATE sfml-graphics)