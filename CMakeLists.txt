cmake_minimum_required(VERSION 3.23)
project(Backgammon)

#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_CXX_STANDARD 20)

add_library(socket_exception STATIC socket/exception/socket_exception.cpp)
add_library(server_socket STATIC socket/server/Server.cpp)
add_library(client_socket STATIC socket/client/Client.cpp)
add_library(backgammon_logic STATIC game/backgammon/backgammon.cpp)
add_library(player STATIC game/player/player.cpp)
#[[ network ]]
add_library(backgammon_server game/mode/network/server/BackgammonServer.cpp)
add_library(backgammon_event game/mode/network/event/Event.cpp)
add_library(backgammon_client game/mode/network/client/BackgammonClient.cpp)

add_executable(backgammon game.cpp)
add_executable(server server.cpp)

target_link_libraries(backgammon backgammon_logic player backgammon_client backgammon_event client_socket socket_exception)
target_link_libraries(server backgammon_server backgammon_event server_socket socket_exception)