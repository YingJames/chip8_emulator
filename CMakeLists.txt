cmake_minimum_required(VERSION 3.28)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8_emulator main.cpp
        Chip8.cpp
        include/Chip8.h
        include/fontset.h)
