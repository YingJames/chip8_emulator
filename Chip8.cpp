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

    std::memset(memory, 0, sizeof(memory));
    std::memset(V, 0, sizeof(V));
    std::memset(stack, 0, sizeof(stack));
    std::memset(gfx, 0, sizeof(gfx));

    // testing opcode
    memory[pc] = 0x00;
    memory[pc + 1] = 0xE0;

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
    }

    if (opcode_function != nullptr) {
        switch (opcode & 0xF000) {
            case 0xA000:
                opcode_function = &Chip8::execOpcode0xANNN;
                break;

            default:
                std::cout << "Unknown opcode: 0x" << std::hex << opcode << '\n';
        }
    }

    // execute opcode
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
    std::cout << "test 0x0NNN opcode\n";
}

// clear the screen
void Chip8::execOpcode0x00E0() {
    std::cout << "test 0x00E0 opcode\n";
    std::memset(gfx, 0, sizeof(gfx));
}

void Chip8::execOpcode0xANNN() {
    std::cout << "test 0xANNN opcode\n";
    I = opcode & 0x0FFF;
}
