cmake_minimum_required(VERSION 3.26)
project(reversi)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)

add_executable(tests tests.cpp reversi.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(reversi main.cpp reversi.cpp)
