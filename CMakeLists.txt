cmake_minimum_required(VERSION 3.22)
project(chip8_emulator_project CXX)

set(CMAKE_CXX_STANDARD 17)
file (
        GLOB_RECURSE
        Sources
        source/*.cpp
)

add_executable(chip8_emulator ${Sources})
target_include_directories(chip8_emulator
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# libraries
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8_emulator ${SDL2_LIBRARIES})