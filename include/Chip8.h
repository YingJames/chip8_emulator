//
// Created by james on 5/8/24.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H


class Chip8
{
public:
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];
    unsigned char gfx[64 * 32];
    unsigned short opcode;
    unsigned short I;
    unsigned short pc;
    unsigned short sp;


    unsigned char delay_timer;
    unsigned char sound_timer;

    void initialize();
    void emulateCycle();

    void execOpcode0x0NNN();

    // Clear the screen
    void execOpcode0x00E0();

    // Return from a subroutine
    void execOpcode0x00EE();

    // Jump to address NNN
    void execOpcode0x1NNN();

    // Execute subroutine at address NNN
    void execOpcode0x2NNN();

    // skip the following (or next) instruction if the VX == NN
    void execOpcode0x3XNN();

    // skip the following (or next) instruction if the VX != NN
    void execOpcode0x4XNN();

    // skip the following (or next) instruction if the VX == VY
    void execOpcode0x5XY0();
    void execOpcode0x6XNN();
    void execOpcode0x7XNN();
    void execOpcode0x8XY0();
    void execOpcode0x8XY1();
    void execOpcode0x8XY2();
    void execOpcode0x8XY3();
    void execOpcode0x8XY4();
    void execOpcode0x8XY5();
    void execOpcode0x8XY6();
    void execOpcode0x8XY7();
    void execOpcode0x8XYE();
    void execOpcode0x9XY0();
    void execOpcode0xANNN();
    void execOpcode0xBNNN();
    void execOpcode0xCXNN();
    void execOpcode0xDXYN();
    void execOpcode0xEX9E();
    void execOpcode0xEXA1();
    void execOpcode0xFX07();
    void execOpcode0xFX0A();
    void execOpcode0xFX15();
    void execOpcode0xFX18();
    void execOpcode0xFX1E();
    void execOpcode0xFX29();
    void execOpcode0xFX33();
    void execOpcode0xFX55();
    void execOpcode0xFX65();

};


#endif //CHIP8_EMULATOR_CHIP8_H
