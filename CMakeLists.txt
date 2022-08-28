cmake_minimum_required(VERSION 3.23)
project(Backgammon)

set(CMAKE_CXX_STANDARD 20)

add_library(backgammon_logic STATIC game/backgammon.cpp)
add_executable(backgammon main.cpp)
target_link_libraries(backgammon backgammon_logic)