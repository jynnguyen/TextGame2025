cmake_minimum_required(VERSION 3.10)

project(MyGameProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCE_FILES source/*.cpp)

add_executable(play ${SOURCE_FILES})
