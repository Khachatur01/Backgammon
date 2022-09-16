cmake_minimum_required(VERSION 3.23)
project(Backgammon)

#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_CXX_STANDARD 20)

add_library(server_socket STATIC socket/server/ServerSocket.cpp)
add_library(client_socket STATIC socket/client/ClientSocket.cpp)
add_library(backgammon STATIC game/backgammon/backgammon.cpp)
add_library(player STATIC game/player/player.cpp)
#[[ modes ]]
add_library(local STATIC game/mode/local.cpp)
add_library(network STATIC game/mode/network.cpp)
add_library(bot STATIC game/mode/bot.cpp)

add_executable(client client.cpp)
target_link_libraries(client client_socket)
add_executable(server server.cpp)
target_link_libraries(server server_socket)

add_executable(backgammon_game main.cpp)
target_link_libraries(backgammon_game backgammon player local network bot)