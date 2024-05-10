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
    memory[pc] = 0x80;
    memory[pc + 1] = 0x16;
    V[0] = 0x10;
    V[1] = 0x23;

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

    // for opcodes where most sig digit matters
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
            case 0x6000:
                opcode_function = &Chip8::execOpcode0x6XNN;
                break;
            case 0x7000:
                opcode_function = &Chip8::execOpcode0x7XNN;
                break;
            case 0xA000:
                opcode_function = &Chip8::execOpcode0xANNN;
                break;
            case 0xB000:
                opcode_function = &Chip8::execOpcode0xBNNN;
                break;
        }
    }

    // find opcode where most and least sig digit matters
    if (opcode_function == nullptr) {
        switch (opcode & 0xF00F) {
            case 0x8000:
                opcode_function = &Chip8::execOpcode0x8XY0;
                break;
            case 0x8001:
                opcode_function = &Chip8::execOpcode0x8XY1;
                break;
            case 0x8002:
                opcode_function = &Chip8::execOpcode0x8XY2;
                break;
            case 0x8003:
                opcode_function = &Chip8::execOpcode0x8XY3;
                break;
            case 0x8004:
                opcode_function = &Chip8::execOpcode0x8XY4;
                break;
            case 0x8005:
                opcode_function = &Chip8::execOpcode0x8XY5;
                break;
            case 0x8006:
                opcode_function = &Chip8::execOpcode0x8XY6;
                break;
            case 0x8007:
                opcode_function = &Chip8::execOpcode0x8XY7;
                break;
            case 0x800E:
                opcode_function = &Chip8::execOpcode0x8XYE;
                break;
            case 0x9000:
                opcode_function = &Chip8::execOpcode0x9XY0;

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

    const uint16_t address = (opcode & 0x0FFF);
    pc = address;
}

void Chip8::execOpcode0x3XNN() {
    const uint8_t NN = (opcode & 0x00FF);
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t VX = V[X];

    if (VX == NN) {
        pc += 4;
    }

    printf("checking if  V%X == NN (%X) - Result: %d\n", X, NN, (VX == NN));
}

void Chip8::execOpcode0x4XNN() {
    const uint8_t NN = (opcode & 0x00FF);
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t VX = V[X];

    if (VX != NN) {
        pc += 4;
    }

    printf("checking if  V%X != NN (%X) - Result: %d\n", X, NN, (VX != NN));

}

void Chip8::execOpcode0x5XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    const uint8_t VX = V[X];
    const uint8_t VY = V[Y];

    if (VX == VY) {
        pc += 4;
    }

    printf("checking if  VX (%X) == VY (%X) - Result: %d\n", X, Y, (VX != VY));
}

void Chip8::execOpcode0x6XNN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t NN = (opcode & 0x00FF);
    V[X] = NN;
}

void Chip8::execOpcode0x7XNN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t NN = (opcode & 0x00FF);
    V[X] += NN;
}

void Chip8::execOpcode0x8XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[Y];
}

void Chip8::execOpcode0x8XY1() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] | V[Y];
}

void Chip8::execOpcode0x8XY2() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] & V[Y];
}

void Chip8::execOpcode0x8XY3() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] ^ V[Y];
}

void Chip8::execOpcode0x8XY4() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a carry
    V[0xF] = ((V[X] + V[Y]) > 0xFF);
    V[X] += V[Y];
    printf("testing 0x8XY4: Sum=0x%X, Carry=%d\n", V[X], V[0xF]);
}

void Chip8::execOpcode0x8XY5() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a borrow
    V[0xF] = ((V[X] - V[Y]) < 0x0);
    V[X] -= V[Y];
    printf("testing 0x8XY5: Diff=0x%X, Borrow=%d\n", V[X], V[0xF]);
}

void Chip8::execOpcode0x8XY6() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // store least sig digit
    V[0xF] = V[Y] & 0x01;
    V[X] >>= V[Y];
    printf("testing 0x8XY6: >>VY=0x%X, lsdigit=%d\n", V[X], V[0xF]);
}

void Chip8::execOpcode0x8XY7() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a borrow
    V[0xF] = ((V[Y] - V[X]) < 0x0);
    V[X] = V[Y] - V[X];
    printf("testing 0x8XY7: VX (VY-VX)=0x%X, Borrow=%d\n", V[X], V[0xF]);
}

void Chip8::execOpcode0x8XYE() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // store most sig digit
    V[0xF] = V[Y] & 0x80;
    V[X] <<= V[Y];
    printf("testing 0x8XYE: >>VY=0x%X, msdigit=%d\n", V[X], V[0xF]);
}

void Chip8::execOpcode0x9XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    if (V[X] != V[Y]) {
        pc += 4;
    }

    printf("checking if  VX (%X) != VY (%X) - Result: %d\n", X, Y, (V[X] != V[Y]));
}

void Chip8::execOpcode0xANNN() {
    printf("test 0xANNN opcode\n");
    I = opcode & 0x0FFF;
}

void Chip8::execOpcode0xBNNN() {
    printf("test 0xBNNN opcode\n");
    pc = (opcode & 0XFFF) + V[0];
}
