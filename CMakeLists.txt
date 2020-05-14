cmake_minimum_required(VERSION 3.5)
project(incoma)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(client)
add_subdirectory(server)



