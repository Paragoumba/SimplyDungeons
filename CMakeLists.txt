cmake_minimum_required(VERSION 3.19)
project(SimplyDungeons)

set(CMAKE_CXX_STANDARD 20)

add_executable(SimplyDungeons main.cpp Entity.cpp Floor.cpp Item.cpp)
target_link_libraries(SimplyDungeons SDL2 SDL2_image SDL2_ttf)