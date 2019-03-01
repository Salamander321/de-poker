cmake_minimum_required(VERSION 3.13)
project(DePoker)

set(CMAKE_CXX_STANDARD 14)

add_executable(DePoker server.cpp src/Menu.cpp src/windows.cpp src/Network.h src/Network.cpp src/board.h src/board.cpp src/card.h src/card.cpp src/chips.h src/deck.h src/deck.cpp src/game_defs.h src/hand.h src/hand.cpp src/player.h src/player.cpp src/poker.h src/player.cpp)
include_directories(DePoker src)
target_link_libraries(DePoker sfml-graphics sfml-window sfml-network sfml-system pthread)