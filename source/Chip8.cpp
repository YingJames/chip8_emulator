//
// Created by james on 5/8/24.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <random>
#include "Chip8.h"
#include "fontset.h"
#include "keypad_map.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

void Chip8::initialize() {
    // initialize registers and memory once
    opcode = 0;
    I = 0;
    pc = 0x200;
    sp = 0;
    instruction_counter = 0;
    target_frequency = 5000000.0;
    delay_timer = 60;

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
    for (int i = 0; i < 0x50; i++) {
        memory[i] = fontset[i];
    }
}

void Chip8::loadROM(std::string filename) {
    // open in binary mode
    std::ifstream file(filename, std::ios::binary);

    if (file.fail()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        std::cerr << "Failure reason: " << (file.rdstate() & std::ifstream::failbit ? "Logical error" : "Other error")
                  << std::endl;
    }

    // find size of file and then add the contents to memory at 0x200
    std::vector<uint8_t> rom_buffer(std::istreambuf_iterator<char>(file), {});
    for (int i = 0; i < rom_buffer.size(); ++i) {
        memory[0x200 + i] = rom_buffer[i];
    }
}

void Chip8::emulateCycle() {

    // update keyboard state
    SDL_PumpEvents();

    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode opcode
    void (Chip8::*opcode_function)() = nullptr;
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
            case 0xC000:
                opcode_function = &Chip8::execOpcode0xCXNN;
                break;
            case 0xD000:
                opcode_function = &Chip8::execOpcode0xDXYN;
                break;

        }

        // find opcode where most and least sig digit matters
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
                break;

        }
        switch (opcode & 0xF0FF) {
            case 0xE09E:
                opcode_function = &Chip8::execOpcode0xEX9E;
                break;
            case 0xE0A1:
                opcode_function = &Chip8::execOpcode0xEXA1;
                break;
            case 0xF007:
                opcode_function = &Chip8::execOpcode0xFX07;
                break;
            case 0xF00A:
                opcode_function = &Chip8::execOpcode0xFX0A;
                break;
            case 0xF015:
                opcode_function = &Chip8::execOpcode0xFX15;
                break;
            case 0xF018:
                opcode_function = &Chip8::execOpcode0xFX18;
                break;
            case 0xF01E:
                opcode_function = &Chip8::execOpcode0xFX1E;
                break;
            case 0xF029:
                opcode_function = &Chip8::execOpcode0xFX29;
                break;
            case 0xF033:
                opcode_function = &Chip8::execOpcode0xFX33;
                break;
            case 0xF055:
                opcode_function = &Chip8::execOpcode0xFX55;
                break;
            case 0xF065:
                opcode_function = &Chip8::execOpcode0xFX65;
                break;
        }
    }

    instruction_counter++;

    // execute opcode
//    pc += 2;
    if (opcode_function != nullptr) {
        (this->*opcode_function)();
    }


}

void Chip8::execOpcode0x0NNN() {
    printf("test 0x0NNN opcode\n");
    pc += 2;
}

void Chip8::execOpcode0x00E0() {
    printf("test 0x00E0 opcode\n");
    std::memset(gfx, 0, sizeof(gfx));
    pc += 2;
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
        pc += 2;
    }

    printf("checking if  V%X == NN (%X) - Result: %d\n", X, NN, (VX == NN));
    pc += 2;
}

void Chip8::execOpcode0x4XNN() {
    const uint8_t NN = (opcode & 0x00FF);
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t VX = V[X];

    if (VX != NN) {
        pc += 2;
    }

    printf("checking if  V%X != NN (%X) - Result: %d\n", X, NN, (VX != NN));
    pc += 2;

}

void Chip8::execOpcode0x5XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    const uint8_t VX = V[X];
    const uint8_t VY = V[Y];

    if (VX == VY) {
        pc += 2;
    }

    printf("checking if  VX (%X) == VY (%X) - Result: %d\n", X, Y, (VX != VY));
    pc += 2;
}

void Chip8::execOpcode0x6XNN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t NN = (opcode & 0x00FF);
    V[X] = NN;
    pc += 2;
}

void Chip8::execOpcode0x7XNN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t NN = (opcode & 0x00FF);
    V[X] += NN;
    pc += 2;
}

void Chip8::execOpcode0x8XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[Y];
    pc += 2;
}

void Chip8::execOpcode0x8XY1() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] | V[Y];
    pc += 2;
}

void Chip8::execOpcode0x8XY2() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] & V[Y];
    pc += 2;
}

void Chip8::execOpcode0x8XY3() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    V[X] = V[X] ^ V[Y];
    pc += 2;
}

void Chip8::execOpcode0x8XY4() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a carry
    V[0xF] = ((V[X] + V[Y]) > 0xFF);
    V[X] += V[Y];
    printf("testing 0x8XY4: Sum=0x%X, Carry=%d\n", V[X], V[0xF]);
    pc += 2;
}

void Chip8::execOpcode0x8XY5() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a borrow
    V[0xF] = ((V[X] - V[Y]) < 0x0);
    V[X] -= V[Y];
    printf("testing 0x8XY5: Diff=0x%X, Borrow=%d\n", V[X], V[0xF]);
    pc += 2;
}

void Chip8::execOpcode0x8XY6() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // store least sig digit
    V[0xF] = V[Y] & 0x01;
    V[X] >>= V[Y];
    printf("testing 0x8XY6: >>VY=0x%X, lsdigit=%d\n", V[X], V[0xF]);
    pc += 2;
}

void Chip8::execOpcode0x8XY7() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // eval to 1 or 0 if there is a borrow
    V[0xF] = ((V[Y] - V[X]) < 0x0);
    V[X] = V[Y] - V[X];
    printf("testing 0x8XY7: VX (VY-VX)=0x%X, Borrow=%d\n", V[X], V[0xF]);
    pc += 2;
}

void Chip8::execOpcode0x8XYE() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    // store most sig digit
    V[0xF] = V[Y] & 0x80;
    V[X] <<= V[Y];
    printf("testing 0x8XYE: >>VY=0x%X, msdigit=%d\n", V[X], V[0xF]);
    pc += 2;
}

void Chip8::execOpcode0x9XY0() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;

    if (V[X] != V[Y]) {
        pc += 2;
    }

    printf("checking if  VX (%X) != VY (%X) - Result: %d\n", X, Y, (V[X] != V[Y]));
    pc += 2;
}

void Chip8::execOpcode0xANNN() {
    printf("test 0xANNN opcode\n");
    I = opcode & 0x0FFF;
    pc += 2;
}

void Chip8::execOpcode0xBNNN() {
    printf("test 0xBNNN opcode\n");
    pc = (opcode & 0XFFF) + V[0];
    pc += 2;
}

void Chip8::execOpcode0xCXNN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t NN = (opcode & 0x00FF);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<uint8_t> distribution(0x0, 0xFF);
    V[X] = distribution(generator) & NN;
    pc += 2;
}

void Chip8::execOpcode0xDXYN() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t Y = (opcode & 0x00F0) >> 4;
    const uint8_t N = (opcode & 0x000F);

    const uint8_t x_pos = V[X] % 64;
    const uint8_t y_pos = V[Y] % 32;

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < 8; col++) {
            // clip sprite if it goes off-screen
            if (x_pos + col >= 64 || y_pos + row >= 32) {
                continue;
            }
            const uint8_t sprite_row = memory[(I + col) + (row * 8)];
            const uint8_t sprite_bit_mask = 0x80 >> (7 - col);

            const uint8_t display_px_index = ((y_pos + row) * 64) + (x_pos + col);
            const uint8_t current_sprite_px_value = sprite_row & sprite_bit_mask;
            const uint8_t pixel_brightness = current_sprite_px_value ^ gfx[display_px_index];

            gfx[display_px_index] = pixel_brightness;
            V[0xF] = current_sprite_px_value && gfx[display_px_index];
        }
    }

    pc += 2;
}

void Chip8::execOpcode0xEX9E() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t key = V[X];
    if (isKeyPressed(key)) {
//        printf("huh\n");
        pc += 2;
    }
    pc += 2;
}

void Chip8::execOpcode0xEXA1() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t key = V[X];
    if (!isKeyPressed(key)) {
        printf("key not pressed\n");
        pc += 2;
    }
    pc += 2;
}

void Chip8::execOpcode0xFX07() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    V[X] = delay_timer;
}

void Chip8::execOpcode0xFX0A() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    const uint8_t key = V[X];
    if (isKeyPressed(key)) {
        printf("this works\n");
        pc += 2;
    }
}

void Chip8::execOpcode0xFX15() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    delay_timer = V[X];
}

void Chip8::execOpcode0xFX18() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    sound_timer = V[X];
}

void Chip8::execOpcode0xFX1E() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    I = V[X];
}

void Chip8::execOpcode0xFX29() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    I = (0x50 + (5 * V[X]));
}

void Chip8::execOpcode0xFX33() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    memory[I + 2] = V[X] % 10;
    memory[I + 1] = V[X] % 100;
    memory[I] = V[X] % 1000;
}

void Chip8::execOpcode0xFX55() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= X; i++) {
        memory[I + i] = V[i];
    }
    I = I + X + 1;
}

void Chip8::execOpcode0xFX65() {
    const uint8_t X = (opcode & 0x0F00) >> 8;
    for (int i = 0; i <= X; i++) {
        V[i] = memory[I + i];
    }
    I = I + X + 1;
}

int Chip8::isKeyPressed(uint8_t key) {
    const uint8_t *keyboard_state = SDL_GetKeyboardState(nullptr);
    SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
    scancode = key_to_scancode[key];

    if (scancode != SDL_SCANCODE_UNKNOWN) {
        return keyboard_state[scancode];
    }
    return 0;
}