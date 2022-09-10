cmake_minimum_required(VERSION 3.23)
project(Backgammon)

#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_CXX_STANDARD 20)

add_library(backgammon STATIC game/backgammon/backgammon.cpp)
add_library(player STATIC game/player/player.cpp)

add_executable(backgammon_game main.cpp)
target_link_libraries(backgammon_game backgammon player)