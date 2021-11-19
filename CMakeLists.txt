cmake_minimum_required(VERSION 3.16.3)

project("Quanta" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("Quanta")
add_subdirectory("QuantaPkg")
add_subdirectory("QuantaEditor")
add_subdirectory("Sandbox")