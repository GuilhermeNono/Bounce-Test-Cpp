cmake_minimum_required(VERSION 3.30)
project(Teste2)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_executable(Teste2 main.cpp)

target_link_libraries(Teste2 PRIVATE raylib)
target_link_libraries(Teste2 PRIVATE $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)