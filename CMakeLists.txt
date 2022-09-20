cmake_minimum_required(VERSION 3.23)
project(Backgammon)

#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_CXX_STANDARD 20)

add_library(socket_exception STATIC socket/exception/socket_exception.cpp)
add_library(server_socket STATIC socket/server/ServerSocket.cpp)
add_library(client_socket STATIC socket/client/ClientSocket.cpp)
add_library(backgammon STATIC game/backgammon/backgammon.cpp)
add_library(player STATIC game/player/player.cpp)
#[[ network ]]
add_library(backgammon_server game/mode/network/server/BackgammonServer.cpp)
add_library(backgammon_client game/mode/network/client/BackgammonClient.cpp)

add_executable(client client.cpp)
target_link_libraries(client client_socket socket_exception)
add_executable(server server.cpp)
target_link_libraries(server server_socket socket_exception)

add_executable(backgammon_game main.cpp)
target_link_libraries(backgammon_game backgammon player backgammon_server backgammon_client client_socket server_socket socket_exception)