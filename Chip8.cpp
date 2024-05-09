//
// Created by james on 5/8/24.
//

#include <iostream>
#include <cstring>
#include "include/Chip8.h"
#include "include/fontset.h"

void Chip8::initialize() {
    // initialize registers and memory once
    opcode = 0;
    I = 0;
    pc = 0x200;
    sp = 0;
//    memory[pc] = 0xA2;
//    memory[pc + 1] = 0xF0;

    std::memset(memory, 0, sizeof(memory));
    std::memset(V, 0, sizeof(V));
    std::memset(stack, 0, sizeof(stack));
    std::memset(gfx, 0, sizeof(gfx));

    // load fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}

void Chip8::emulateCycle() {
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode opcode
    switch (opcode & 0xF000) {
        // execute opcode
        case 0xA000:
            std::cout << "test A opcode\n";
            I = opcode & 0x0FFF;
            break;

        default:
            std::cout << "Unknown opcode: 0x" << opcode << '\n';
    }

    // update timers
//    if (delay_timer)
}