cmake_minimum_required(VERSION 3.23)
project(Backgammon)

set(CMAKE_CXX_STANDARD 20)

add_library(backgammon STATIC game/backgammon/backgammon.cpp)

add_executable(backgammon_game main.cpp)
target_link_libraries(backgammon_game backgammon)