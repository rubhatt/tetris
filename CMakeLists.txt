cmake_minimum_required (VERSION 2.6)
set (LIBS sfml-graphics sfml-window sfml-system)

project (Tetris)
add_executable (Tetris tetris.cpp)
target_link_libraries(Tetris ${LIBS})
