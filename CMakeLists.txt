cmake_minimum_required(VERSION 3.13)
project(DePoker)

set(CMAKE_CXX_STANDARD 14)

add_executable(DePoker main.cpp src/Menu.cpp src/windows.cpp src/Network.h src/Network.cpp src/board.h
        src/board.cpp src/card.h src/card.cpp src/chips.h src/deck.h src/deck.cpp src/game_defs.h src/hand.h
        src/hand.cpp src/player.h src/player.cpp src/poker.h src/player.cpp src/game.h src/game.cpp src/game_defs.h
        src/gui_engine_defs.h src/input_manager.h src/input_manager.cpp src/Lobby.h src/Lobby.cpp src/state.h
        src/state_machine.h src/state_machine.cpp src/asset_manager.cpp src/asset_manager.h src/game_state.cpp
        src/game_state.h src/mainmenu_state.cpp src/mainmenu_state.h src/splash_state.h src/splash_state.cpp
        src/PlayerContainer.cpp src/PlayerContainer.h)
include_directories(DePoker src assets)
target_link_libraries(DePoker sfml-graphics sfml-window sfml-network sfml-system sfml-audio pthread)