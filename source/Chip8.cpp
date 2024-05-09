//
// Created by james on 5/8/24.
//

#include <iostream>
#include <cstring>
#include "Chip8.h"
#include "fontset.h"

void Chip8::initialize() {
    // initialize registers and memory once
    opcode = 0;
    I = 0;
    pc = 0x200;
    sp = 0;

    std::memset(memory, 0, sizeof(memory));
    std::memset(V, 0, sizeof(V));
    std::memset(stack, 0, sizeof(stack));
    std::memset(gfx, 0, sizeof(gfx));

    // testing opcode
    memory[pc] = 0x4f;
    memory[pc + 1] = 0xEE;

    // load fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = fontset[i];
    }
}

void Chip8::emulateCycle() {
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode opcode
    void (Chip8::*opcode_function) () = nullptr;
    switch (opcode) {
        case 0x00E0:
            opcode_function = &Chip8::execOpcode0x00E0;
            break;
        case 0x00EE:
            opcode_function = &Chip8::execOpcode0x00EE;
            break;
    }

    if (opcode_function == nullptr) {
        switch (opcode & 0xF000) {
            case 0x1000:
                opcode_function = &Chip8::execOpcode0x1NNN;
                break;
            case 0x2000:
                opcode_function = &Chip8::execOpcode0x2NNN;
                break;
            case 0x3000:
                opcode_function = &Chip8::execOpcode0x3XNN;
                break;
            case 0x4000:
                opcode_function = &Chip8::execOpcode0x4XNN;
                break;
            case 0x5000:
                opcode_function = &Chip8::execOpcode0x5XY0;
                break;
            case 0xA000:
                opcode_function = &Chip8::execOpcode0xANNN;
                break;

            default:
                printf("Unknown opcode: 0x%X\n", opcode);
        }
    }

    // execute opcode
//    pc += 2;
    if (opcode_function != nullptr) {
        (this->*opcode_function) ();
    }

    // update timers
    if (delay_timer > 0) {
        --delay_timer;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            std::cout << "boop\n";
        }
        --sound_timer;
    }
}

void Chip8::execOpcode0x0NNN() {
    printf("test 0x0NNN opcode\n");
}

void Chip8::execOpcode0x00E0() {
    printf("test 0x00E0 opcode\n");
    std::memset(gfx, 0, sizeof(gfx));
}

void Chip8::execOpcode0x00EE() {
    printf("test 0x00EE opcode\n");
    pc = stack[sp];
    --sp;
}

void Chip8::execOpcode0x1NNN() {
    pc = (opcode & 0x0FFF);
    printf("test 0x1NNN opcode\n");
    printf("jumping to address 0x%03x\n", pc);
}

void Chip8::execOpcode0x2NNN() {
    ++sp;
    stack[sp] = pc;

    const unsigned short address = (opcode & 0x0FFF);
    pc = address;
}

void Chip8::execOpcode0x3XNN() {
    const unsigned short NN = (opcode & 0x00FF);
    const unsigned short X = (opcode & 0x0F00) >> 8;
    const unsigned short VX = V[X];

    if (VX == NN) {
        pc += 4;
    }

    printf("checking if  V%X == NN (%X) - Result: %d\n", X, NN, (VX == NN));
}

void Chip8::execOpcode0x4XNN() {
    const unsigned short NN = (opcode & 0x00FF);
    const unsigned short X = (opcode & 0x0F00) >> 8;
    const unsigned short VX = V[X];

    if (VX != NN) {
        pc += 4;
    }

    printf("checking if  V%X != NN (%X) - Result: %d\n", X, NN, (VX != NN));

}

void Chip8::execOpcode0x5XY0() {
    const unsigned short X = (opcode & 0x0F00) >> 8;
    const unsigned short Y = (opcode & 0x00F0) >> 4;
    const unsigned short VX = V[X];
    const unsigned short VY = V[Y];

    if (VX != VY) {
        pc += 4;
    }

    printf("checking if  VX (%X) != VY (%X) - Result: %d\n", X, Y, (VX != VY));
}

void Chip8::execOpcode0xANNN() {
    printf("test 0xANNN opcode\n");
    I = opcode & 0x0FFF;
}
