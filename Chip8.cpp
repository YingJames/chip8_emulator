//
// Created by james on 5/8/24.
//

#include "include/Chip8.h"
#include <iostream>

void Chip8::initialize()
{
    // initialize registers and memory once
    opcode = 0;
    I = 0;
    pc = 0x200;
    memory[pc] = 0xA2;
    memory[pc+1] = 0xF0;
}

void Chip8::emulateCycle()
{
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode opcode
    unsigned short first_hex_digit = (opcode & 0xF000) >> 12;

    if (first_hex_digit == 0xA)
    {
        std::cout << "test A opcode" << std::endl;
    }
    // execute opcode

    // update timers
}