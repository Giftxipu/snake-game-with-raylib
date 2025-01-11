cmake_minimum_required(VERSION 3.0)
project(SnakeGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find Raylib
find_package(raylib REQUIRED)

# Add executable
add_executable(main main.cpp)

# Link Raylib
target_link_libraries(main raylib)

